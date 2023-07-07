// clang-format off
#include "stdafx.h"
// clang-format on

#include "replayengine.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "../LZMA/LzmaDec.h"
#include "../LZMA/LzmaEnc.h"
#include "breader.hpp"
#include "bwriter.hpp"
#include "crypto.hpp"
#include "history_stack.hpp"
#include "texture.hpp"
#include "ui.hpp"

// #define TRACE_REPLAY_LOAD

#ifdef TRACE_REPLAY_LOAD
#include <cstring>
#include <map>
#define trace_if(what) trace(what)
#ifndef _DEBUG
#error "Remove TRACE_REPLAY_LOAD before deploying to prod"
#endif
#else
#define trace_if(what)
#endif

namespace replayengine
{

namespace
{

HistoryStack<Replay, 10> local_history_stack;

std::vector<ReplayFrame> make_frames(char *str)
{
    std::vector<ReplayFrame> frames;
#ifdef TRACE_REPLAY_LOAD
    {
        const size_t replay_string_len = std::strlen(str);
        const char *const last_100_chars = (replay_string_len <= 100) ? str : (str + (replay_string_len - 100));
        trace_if(last_100_chars);
        std::map<char, int> freq;
        for (size_t i = 0; i < replay_string_len; ++i) {
            ++freq[str[i]];
        }
        for (const auto &[character, amount] : freq) {
            TRACE << character << ": " << amount << std::endl;
        }
    }
#endif
    SongTime_t curr = 0;
    for (;;) {
        char *ptrs[4];
        ptrs[0] = str;
        char **pptr = ptrs + 1;
        if (*str == '\0') goto end_of_string;
        while (*str != ',') {
            if (*str == '\0') {
                goto end_of_string;
            } else if (*str == '|') {
                *str = '\0';
                *pptr = str + 1;
                ++pptr;
            }
            ++str;
        }
        *str = '\0';
        ++str;
        replayengine::ReplayFrame f;
        f.ms = atoi(ptrs[0]);
        f.p.x = static_cast<float>(atof(ptrs[1]));
        f.p.y = static_cast<float>(atof(ptrs[2]));
        f.keys = atoi(ptrs[3]);
        if (f.ms == -12345) continue;
        if (f.ms == 0 && curr == 0) continue;
        curr += f.ms;
        if (f.ms < 0) continue;
        f.ms = curr;
        frames.push_back(f);
    }
end_of_string:
    std::sort(frames.begin(), frames.end());
    return frames;
}

void *simple_alloc(ISzAllocPtr p, size_t size)
{
    ((void)(p));
    return new char[size];
}

void simple_free(ISzAllocPtr p, void *address)
{
    ((void)(p));
    if (address) delete[] address;
}

bool interpret_lzma_result(const int res)
{
    switch (res) {
        case SZ_OK:
            return true;
        case SZ_ERROR_DATA:
            not_fatal("LZMA data error");
            return false;
        case SZ_ERROR_MEM:
            not_fatal("LZMA memory allocation error");
            return false;
        case SZ_ERROR_UNSUPPORTED:
            not_fatal("LZMA unsupported properties");
            return false;
        case SZ_ERROR_INPUT_EOF:
            not_fatal("LZMA needed more bytes in input buffer");
            return false;
        case SZ_ERROR_OUTPUT_EOF:
            not_fatal("LZMA output needed more bytes in output buffer");
            return false;
        case SZ_ERROR_PARAM:
            not_fatal("LZMA incorrect parameter");
            return false;
        case SZ_ERROR_THREAD:
            not_fatal("LZMA error in multithreaded functions");
            return false;
        default:
            not_fatal("LZMA unknown error");
            return false;
    }
}

void apply_mutation(std::unique_ptr<Replay> next)
{
    // TODO: If the frame size changes, the markers might be out of bounds
    // if (local_current_view->frames().size() != next->frames().size()) {}
    local_history_stack.push_mutation(std::move(next));
}

}  // namespace

bool Replay::read_from(const std::wstring &fname)
{
    if (fname.empty()) return false;
    breader_t r(fname);
    if (r.is_closed()) {
        not_fatal("replay cannot be opened");
        return false;
    }
    r >> m_metadata.game_mode;
    trace_if(+m_metadata.game_mode);
    r >> m_metadata.version;
    trace_if(m_metadata.version);
    r >> m_metadata.beatmap_hash;
    trace_if(m_metadata.beatmap_hash);
    r >> m_metadata.player_name;
    trace_if(m_metadata.player_name);
    r >> m_metadata.replay_hash;
    trace_if(m_metadata.replay_hash);
    r >> m_metadata.num_300;
    trace_if(m_metadata.num_300);
    r >> m_metadata.num_100;
    trace_if(m_metadata.num_100);
    r >> m_metadata.num_50;
    trace_if(m_metadata.num_50);
    r >> m_metadata.num_geki;
    trace_if(m_metadata.num_geki);
    r >> m_metadata.num_katu;
    trace_if(m_metadata.num_katu);
    r >> m_metadata.num_miss;
    trace_if(m_metadata.num_miss);
    r >> m_metadata.total_score;
    trace_if(m_metadata.total_score);
    r >> m_metadata.max_combo;
    trace_if(m_metadata.max_combo);
    r >> m_metadata.full_combo;
    trace_if(m_metadata.full_combo);
    r >> m_metadata.mods;
    trace_if(m_metadata.mods);
    r >> m_metadata.lifebar_graph;
    trace_if(m_metadata.lifebar_graph);
    r >> m_metadata.play_timestamp;
    trace_if(m_metadata.play_timestamp);
    uint32_t in_size;
    r >> in_size;
    trace_if(in_size);
    if (in_size == 0) {
        r >> m_metadata.online_score_id;
        trace_if(m_metadata.online_score_id);
        not_fatal("replay contains no cursor data");
        return true;
    }
    auto buf = std::make_unique<uint8_t[]>(in_size);
    r.fill_buf(buf.get(), in_size);
    r >> m_metadata.online_score_id;
    trace_if(m_metadata.online_score_id);
    uint8_t *const prop_data = buf.get();
    uint8_t *const out_size_data = buf.get() + 5;
    uint8_t *const in_data = out_size_data + 8;
    uint64_t out_size_64 = 0;
    std::memcpy(&out_size_64, out_size_data, sizeof(SizeT));
    trace_if(out_size_64);
    if (out_size_64 >= std::numeric_limits<uint32_t>::max()) {
        not_fatal("uncompressed data is too large");
        return false;
    }
    SizeT out_size = static_cast<SizeT>(out_size_64);
    auto out_data = std::make_unique<char[]>(out_size + 1);
    ELzmaStatus status = LZMA_STATUS_NOT_SPECIFIED;
    ISzAlloc my_alloc;
    my_alloc.Alloc = simple_alloc;
    my_alloc.Free = simple_free;
    // Since SizeT may not be the same size as uint32, passing a pointer to in_size would be UB.
    SizeT lzma_in_size = in_size;
    const int res = LzmaDecode((Byte *)out_data.get(), &out_size, (Byte *)in_data, &lzma_in_size, (Byte *)prop_data, 5,
                               LZMA_FINISH_END, &status, &my_alloc);
    trace_if((res == SZ_OK));
    trace_if((status == LZMA_STATUS_FINISHED_WITH_MARK));
    if (!interpret_lzma_result(res)) return false;
    m_frames = make_frames(out_data.get());
    return true;
}

bool Replay::write_to(const std::wstring &fname)
{
    if (fname.empty()) return true;
    bwriter_t w(fname);
    if (w.is_closed()) {
        not_fatal("file with this name or location cannot be exported");
        return false;
    }
    {
        Hasher hasher;
        hasher(m_metadata.game_mode);
        hasher(m_metadata.version);
        hasher.add_data(m_metadata.player_name.c_str(), static_cast<uint32_t>(m_metadata.player_name.size()));
        hasher(m_metadata.num_300);
        hasher(m_metadata.num_100);
        hasher(m_metadata.num_50);
        hasher(m_metadata.num_geki);
        hasher(m_metadata.num_katu);
        hasher(m_metadata.num_miss);
        hasher(m_metadata.total_score);
        hasher(m_metadata.max_combo);
        hasher(m_metadata.full_combo);
        hasher(m_metadata.mods);
        hasher.add_data(m_metadata.lifebar_graph.c_str(), static_cast<uint32_t>(m_metadata.lifebar_graph.size()));
        hasher(m_metadata.play_timestamp);
        hasher(m_frames.size());
        for (const auto &frame : m_frames) {
            hasher(frame);
        }
        m_metadata.replay_hash = hasher.to_md5();
    }
    w << m_metadata.game_mode;
    w << m_metadata.version;
    w << m_metadata.beatmap_hash;
    w << m_metadata.player_name;
    w << m_metadata.replay_hash;
    w << m_metadata.num_300;
    w << m_metadata.num_100;
    w << m_metadata.num_50;
    w << m_metadata.num_geki;
    w << m_metadata.num_katu;
    w << m_metadata.num_miss;
    w << m_metadata.total_score;
    w << m_metadata.max_combo;
    w << m_metadata.full_combo;
    w << m_metadata.mods;
    w << m_metadata.lifebar_graph;
    w << m_metadata.play_timestamp;
    if (m_frames.empty()) {
        constexpr uint32_t out_size = 0;
        w << out_size;
    } else {
        std::stringstream ss;
        I64 last_action_ms = 0;
        for (const auto &frame : m_frames) {
            if (std::isnan(frame.p.x) || std::isinf(frame.p.x) || std::isnan(frame.p.y) || std::isinf(frame.p.y)) {
                continue;
            }
            ss << (frame.ms - last_action_ms) << '|';
            last_action_ms = frame.ms;
            ss << frame.p.x << '|' << frame.p.y << '|' << frame.keys << ',';
        }
        ss << "-12345|0|0|12345,";
        const std::string s = ss.str();
        CLzmaEncProps props;
        LzmaEncProps_Init(&props);
        LzmaEncProps_Normalize(&props);
        SizeT output_size = s.size();
        uint8_t props_encoded[5];
        SizeT props_size = 5;
        if (output_size < 1024) output_size = 1024;
        auto output_buf = std::make_unique<uint8_t[]>(output_size);
        ISzAlloc my_alloc;
        my_alloc.Alloc = simple_alloc;
        my_alloc.Free = simple_free;
        const int res = LzmaEncode(output_buf.get(), &output_size, reinterpret_cast<const Byte *>(s.c_str()), s.size(),
                                   &props, props_encoded, &props_size, 0, nullptr, &my_alloc, &my_alloc);
        if (!interpret_lzma_result(res)) return false;
        w << static_cast<uint32_t>(output_size + 5 + 8);
        w.write_buf(props_encoded, 5);
        w << static_cast<uint32_t>(s.size());
        w << static_cast<uint32_t>(0);
        w.write_buf(output_buf.get(), static_cast<uint32_t>(output_size));
    }
    w << m_metadata.online_score_id;
    return true;
}

void Replay::draw(SongTime_t ms)
{
    auto g = std::lower_bound(m_frames.begin(), m_frames.end(), ms, CmpMs<ReplayFrame>());
    if (g == m_frames.end()) return;
    m_current_frame_index = g - m_frames.begin();
    glm::vec2 pos = g->p;
    if (g != m_frames.begin()) {
        auto f = g - 1;
        const float t = RATIO(ms - f->ms, g->ms - f->ms);
        pos = glm::mix(f->p, g->p, t);
    }
    const glm::vec2 origin(16.0f, 16.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    textures::cursor->draw(pos, origin);
}

void Replay::invert_replay_frames()
{
    for (size_t i = 0; i < m_frames.size(); ++i) {
        glm::vec2 &p = m_frames[i].p;
        p.y = 384.f - p.y;
    }
}

bool Replay::is_single_mark_in_range(I64 mark) const
{
    return mark >= 0 && mark < static_cast<I64>(m_frames.size());
}

bool Replay::are_in_out_marks_consistent() const
{
    if (!is_single_mark_in_range(m_mark_in)) return false;
    if (!is_single_mark_in_range(m_mark_out)) return false;
    return m_mark_in <= m_mark_out;
}

bool Replay::are_all_marks_consistent() const
{
    if (!is_single_mark_in_range(m_mark_in)) return false;
    if (!is_single_mark_in_range(m_mark_out)) return false;
    if (!is_single_mark_in_range(m_mark_mid)) return false;
    return m_mark_in <= m_mark_mid && m_mark_mid <= m_mark_out;
}

void Replay::place_mark_in()
{
    m_mark_in = m_current_frame_index;
    place_mark_mid_avg();
}

void Replay::place_mark_out()
{
    m_mark_out = m_current_frame_index;
    place_mark_mid_avg();
}

void Replay::place_mark_mid()
{
    m_mark_mid = m_current_frame_index;
}

void Replay::place_mark_all()
{
    m_mark_in = 0;
    m_mark_out = m_frames.size() - 1;
    place_mark_mid_avg();
}

void Replay::clear_marks()
{
    m_mark_in = -1;
    m_mark_out = -1;
    m_mark_mid = -1;
}

void Replay::place_mark_mid_avg()
{
    if (are_in_out_marks_consistent()) {
        m_mark_mid = m_mark_in + (m_mark_out - m_mark_in) / 2;
    }
}

void Replay::place_marks_at(I64 mark_in, I64 mark_out)
{
    m_mark_in = mark_in;
    m_mark_out = mark_out;
    place_mark_mid_avg();
}

const Replay *CurrentView()
{
    return local_history_stack.current();
}

Replay *MutableCurrentView()
{
    return local_history_stack.current();
}

bool MutateCurrentView(std::function<bool(const Replay &curr, Replay &next)> mutator)
{
    std::unique_ptr<Replay> next = std::make_unique<Replay>();
    const bool result = mutator(*local_history_stack.current(), *next);
    if (result) {
        apply_mutation(std::move(next));
    }
    return result;
}

bool MutateCurrentView(std::function<bool(Replay &replay)> mutator)
{
    std::unique_ptr<Replay> next = std::make_unique<Replay>(*local_history_stack.current());
    const bool result = mutator(*next);
    if (result) {
        apply_mutation(std::move(next));
    }
    return result;
}

void DuplicateCurrentView()
{
    apply_mutation(std::make_unique<Replay>(*local_history_stack.current()));
}

bool Undo()
{
    return local_history_stack.undo_mutation();
}

bool Redo()
{
    return local_history_stack.redo_mutation();
}

bool init()
{
    local_history_stack.push_mutation(std::make_unique<Replay>());
    return true;
}

}  // namespace replayengine
