#include "stdafx.h"
#include "replayengine.hpp"
#include "breader.hpp"
#include "bwriter.hpp"
#include "../LZMA/LzmaDec.h"
#include "../LZMA/LzmaEnc.h"
#include <vector>
#include <algorithm>
#include "texture.hpp"
#include "ui.hpp"
#include <sstream>
#include <memory>
#include "crypto.hpp"

//#define TRACE_REPLAY_LOAD

#ifdef TRACE_REPLAY_LOAD
#include <cstring>
#define trace_if(what) trace(what)
#ifndef _DEBUG
#error "Remove TRACE_REPLAY_LOAD before deploying to prod"
#endif
#else
#define trace_if(what)
#endif

uint8_t replayengine::game_mode;
uint32_t replayengine::version;
md5_t replayengine::beatmap_hash;
std::string replayengine::player_name;
md5_t replayengine::replay_hash;
uint16_t replayengine::num_300;
uint16_t replayengine::num_100;
uint16_t replayengine::num_50;
uint16_t replayengine::num_geki;
uint16_t replayengine::num_katu;
uint16_t replayengine::num_miss;
uint32_t replayengine::total_score;
uint16_t replayengine::max_combo;
bool replayengine::full_combo;
uint32_t replayengine::mods;
std::string replayengine::lifebar_graph;
U64 replayengine::timestamp;
U64 replayengine::online_score_id;

std::vector<replayengine::replayframe_t> replayengine::frames;
I64 replayengine::current_frame_index = 0;

static inline void make_frames(char *str)
{
#ifdef TRACE_REPLAY_LOAD
	{
		const size_t replay_string_len = std::strlen(str);
		const char* const last_100_chars = (replay_string_len <= 100) ? str : (str + (replay_string_len - 100));
		trace_if(last_100_chars);
	}
#endif
	SongTime_t curr = 0;
	for (; ; ) {
		char *ptrs[4];
		ptrs[0] = str;
		char **pptr = ptrs + 1;
		if (*str == '\0')
			goto end_of_string;
		while (*str != ',') {
			if (*str == '\0') {
				goto end_of_string;
			}
			else if (*str == '|') {
				*str = '\0';
				*pptr = str + 1;
				++pptr;
			}
			++str;
		}
		*str = '\0';
		++str;
		replayengine::replayframe_t f;
		f.ms = atoi(ptrs[0]);
		f.p.x = static_cast<float>(atof(ptrs[1]));
		f.p.y = static_cast<float>(atof(ptrs[2]));
		f.keys = atoi(ptrs[3]);
		if (f.ms == -12345)
			continue;
		if (f.ms == 0 && curr == 0)
			continue;
		curr += f.ms;
		if (f.ms < 0)
			continue;
		f.ms = curr;
		replayengine::frames.push_back(f);
	}
	end_of_string:
	std::sort(replayengine::frames.begin(), replayengine::frames.end());
}

static void *simple_alloc(ISzAllocPtr p, size_t size)
{
	((void)(p));
	return new char[size];
}

static void simple_free(ISzAllocPtr p, void *address)
{
	((void)(p));
	if (address)
		delete[] address;
}

static bool interpret_lzma_result(const int res)
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
		not_fatal("LZMA incorrect paramater");
		return false;
	case SZ_ERROR_THREAD:
		not_fatal("LZMA error in multithreaded functions");
		return false;
	default:
		not_fatal("LZMA unknown error");
		return false;
	}
}

bool replayengine::init(const std::wstring &fname)
{
	frames.clear();
	if (fname.empty())
		return true;
	breader_t r(fname);
	if (r.is_closed()) {
		not_fatal("replay cannot be opened");
		return false;
	}
	r >> game_mode;
	trace_if(+game_mode);
	r >> version;
	trace_if(version);
	r >> beatmap_hash;
	trace_if(beatmap_hash);
	r >> player_name;
	trace_if(player_name);
	r >> replay_hash;
	trace_if(replay_hash);
	r >> num_300;
	trace_if(num_300);
	r >> num_100;
	trace_if(num_100);
	r >> num_50;
	trace_if(num_50);
	r >> num_geki;
	trace_if(num_geki);
	r >> num_katu;
	trace_if(num_katu);
	r >> num_miss;
	trace_if(num_miss);
	r >> total_score;
	trace_if(total_score);
	r >> max_combo;
	trace_if(max_combo);
	r >> full_combo;
	trace_if(full_combo);
	r >> mods;
	trace_if(mods);
	r >> lifebar_graph;
	trace_if(lifebar_graph);
	r >> timestamp;
	trace_if(timestamp);
	uint32_t in_size;
	r >> in_size;
	trace_if(in_size);
	if (in_size == 0) {
		r >> online_score_id;
		trace_if(online_score_id);
		not_fatal("replay contains no cursor data");
		return true;
	}
	auto buf = std::make_unique<uint8_t[]>(in_size);
	r.fill_buf(buf.get(), in_size);
	r >> online_score_id;
	trace_if(online_score_id);
	uint8_t *const prop_data = buf.get();
	uint8_t* const out_size_data = buf.get() + 5;
	uint8_t* const in_data = out_size_data + 8;
	uint64_t out_size_64 = 0;
	std::memcpy(&out_size_64, out_size_data, sizeof(SizeT));
	trace_if(out_size_64);
	if (out_size_64 >= std::numeric_limits<uint32_t>::max())
	{
		not_fatal("uncompressed data is too large");
		return false;
	}
	SizeT out_size = static_cast<SizeT>(out_size_64);
	auto out_data = std::make_unique<char[]>(out_size + 1);
	ELzmaStatus status = LZMA_STATUS_NOT_SPECIFIED;
	ISzAlloc my_alloc;
	my_alloc.Alloc = simple_alloc;
	my_alloc.Free = simple_free;
	// since SizeT is bigger than uint32, pointer would not work
	SizeT lzma_in_size = in_size;
	const int res = LzmaDecode((Byte*)out_data.get(), &out_size, (Byte*)in_data, &lzma_in_size, (Byte*)prop_data, 5, LZMA_FINISH_END, &status, &my_alloc);
	trace_if((res == SZ_OK));
	trace_if((status == LZMA_STATUS_FINISHED_WITH_MARK));
	if (!interpret_lzma_result(res))
		return false;
	make_frames(out_data.get());
	return true;
}

void replayengine::draw(SongTime_t ms)
{
	auto g = std::lower_bound(frames.begin(), frames.end(), ms, CmpMs<replayframe_t>());
	if (g == frames.end())
		return;
	current_frame_index = g - frames.begin();
	glm::vec2 pos = g->p;
	if (g != frames.begin()) {
		auto f = g - 1;
		const float t = RATIO(ms - f->ms, g->ms - f->ms);
		pos = glm::mix(f->p, g->p, t);
	}
	const glm::vec2 origin(16.0f, 16.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	textures::cursor->draw(pos, origin);
}

bool replayengine::write_to(const std::wstring &fname)
{
	if (fname.empty())
		return true;
	bwriter_t w(fname);
	if (w.is_closed()) {
		not_fatal("file with this name or location cannot be exported");
		return false;
	}
	{
		Hasher hasher;
		hasher(game_mode);
		hasher(version);
		hasher.add_data(player_name.c_str(), static_cast<uint32_t>(player_name.size()));
		hasher(num_300);
		hasher(num_100);
		hasher(num_50);
		hasher(num_geki);
		hasher(num_katu);
		hasher(num_miss);
		hasher(total_score);
		hasher(max_combo);
		hasher(full_combo);
		hasher(mods);
		hasher.add_data(lifebar_graph.c_str(), static_cast<uint32_t>(lifebar_graph.size()));
		hasher(timestamp);
		hasher(frames.size());
		for (auto frame : frames) {
			hasher(frame);
		}
		hasher.to_md5(replay_hash);
	}
	w << game_mode;
	w << version;
	w << beatmap_hash;
	w << player_name;
	w << replay_hash;
	w << num_300;
	w << num_100;
	w << num_50;
	w << num_geki;
	w << num_katu;
	w << num_miss;
	w << total_score;
	w << max_combo;
	w << full_combo;
	w << mods;
	w << lifebar_graph;
	w << timestamp;
	if (frames.empty()) {
		constexpr uint32_t out_size = 0;
		w << out_size;
	} else {
		std::stringstream ss;
		I64 last_action_ms = 0;
		for (auto frame : frames) {
			ss << (frame.ms - last_action_ms) << '|';
			last_action_ms = frame.ms;
			ss << frame.p.x << '|' << frame.p.y << '|' << frame.keys << ',';
		}
		ss << "-12345|0|0|12345,";
		const std::string s = ss.str();
		CLzmaEncProps props;
		LzmaEncProps_Init(&props);
		if (s.size() >= (1 << 20))
			props.dictSize = 1 << 20;
		else
			props.dictSize = static_cast<uint32_t>(s.size());
		props.fb = 40;
		SizeT output_size = s.size();
		uint8_t props_encoded[5];
		SizeT props_size = 5;
		if (output_size < 1024)
			output_size = 1024;
		auto output_buf = std::make_unique<uint8_t[]>(output_size);
		ISzAlloc my_alloc;
		my_alloc.Alloc = simple_alloc;
		my_alloc.Free = simple_free;
		const int res = LzmaEncode(output_buf.get(), &output_size, reinterpret_cast<const Byte *>(s.c_str()), s.size(), &props, props_encoded, &props_size, 0, nullptr, &my_alloc, &my_alloc);
		if (!interpret_lzma_result(res))
			return false;
		w << static_cast<uint32_t>(output_size + 5 + 8);
		w.write_buf(props_encoded, 5);
		w << static_cast<uint32_t>(s.size());
		w << static_cast<uint32_t>(0);
		w.write_buf(output_buf.get(), static_cast<uint32_t>(output_size));
	}
	w << online_score_id;
	return true;
}

void replayengine::invert_replay_frames()
{
	for (size_t i = 0; i < frames.size(); ++i) {
		glm::vec2 &p = frames[i].p;
		p.y = 384.f - p.y;
	}
}