#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <functional>

#include "thirdparty/glm/glm.hpp"

template <typename T>
class CmpMs
{
   public:
    bool operator()(const T &lhs, const SongTime_t rhs) const
    {
        return lhs.ms < rhs;
    }
};

namespace replayengine
{

struct ReplayFrame {
    glm::vec2 p;
    SongTime_t ms;
    int keys;
    bool operator<(const ReplayFrame &rhs) const
    {
        return ms < rhs.ms;
    }
    bool pressed_mouse1() const
    {
        return keys & 1;
    }
    bool pressed_mouse2() const
    {
        return keys & 2;
    }
    bool pressed_key1() const
    {
        return keys & 4;
    }
    bool pressed_key2() const
    {
        return keys & 8;
    }
    bool pressed_smoke() const
    {
        return keys & 16;
    }
};

struct ReplayMetadata {
    uint8_t game_mode = 0;
    uint32_t version = 0;
    std::string beatmap_hash = "";
    std::string player_name = "";
    std::string replay_hash = "";
    uint16_t num_300 = 0;
    uint16_t num_100 = 0;
    uint16_t num_50 = 0;
    uint16_t num_geki = 0;
    uint16_t num_katu = 0;
    uint16_t num_miss = 0;
    uint32_t total_score = 0;
    uint16_t max_combo = 0;
    bool full_combo = false;
    uint32_t mods = 0;
    std::string lifebar_graph = "";
    U64 play_timestamp = 0;
    U64 online_score_id = 0;
};

class Replay
{
   public:
    // This class is used to make mutating the selected frames easier through a for loop.
    class ReplayFrameSelection
    {
       public:
        ReplayFrameSelection() = default;
        ReplayFrameSelection(ReplayFrame *data, I64 mark_in, I64 mark_out)
            : m_begin(data + mark_in), m_end(data + mark_out + 1)
        {
        }
        ReplayFrame *begin()
        {
            return m_begin;
        }
        ReplayFrame *end()
        {
            return m_end;
        }
        size_t size() const
        {
            return m_end - m_begin;
        }
        bool empty() const
        {
            return size() == 0;
        }

       private:
        ReplayFrame *m_begin = nullptr;
        ReplayFrame *m_end = nullptr;
    };

    bool read_from(const std::wstring &);
    bool write_to(const std::wstring &);
    void invert_replay_frames();
    void draw(SongTime_t);
    bool are_in_out_marks_consistent() const;
    bool are_all_marks_consistent() const;
    void place_mark_in();
    void place_mark_out();
    void place_mark_mid();
    void place_mark_all();
    void clear_marks();
    void place_marks_at(I64 mark_in, I64 mark_out);

    const ReplayMetadata &metadata() const
    {
        return m_metadata;
    }
    ReplayMetadata &mut_metadata()
    {
        return m_metadata;
    }
    const std::vector<ReplayFrame> &frames() const
    {
        return m_frames;
    }
    std::vector<ReplayFrame> &mut_frames()
    {
        return m_frames;
    }
    // Before calling this, the caller must ensure the in and out marks are consistent.
    ReplayFrameSelection mut_selection()
    {
        return ReplayFrameSelection(m_frames.data(), m_mark_in, m_mark_out);
    }
    I64 mark_in() const
    {
        return m_mark_in;
    }
    I64 mark_out() const
    {
        return m_mark_out;
    }
    I64 mark_mid() const
    {
        return m_mark_mid;
    }
    const ReplayFrame &mark_in_frame() const
    {
        return m_frames[m_mark_in];
    }
    const ReplayFrame &mark_out_frame() const
    {
        return m_frames[m_mark_out];
    }
    const ReplayFrame &mark_mid_frame() const
    {
        return m_frames[m_mark_mid];
    }

   private:
    bool is_single_mark_in_range(I64 mark) const;
    void place_mark_mid_avg();

    ReplayMetadata m_metadata{};
    std::vector<ReplayFrame> m_frames{};
    I64 m_current_frame_index = 0;
    I64 m_mark_in;
    I64 m_mark_out;
    I64 m_mark_mid;
};

const Replay *CurrentView();

// MutableCurrentView will edit the replay in place, which will prevent undo information from becoming available.
Replay *MutableCurrentView();

// MutatateCurrentView will take a function pointer which applies its changes into a different replay, allowing the
// change to be undoable. If the function pointer returns false, the change is reverted. This value is propagated to the
// return value of the entire function.
bool MutateCurrentView(std::function<bool(const Replay &curr, Replay &next)> mutator);
bool MutateCurrentView(std::function<bool(Replay &replay)> mutator);
void DuplicateCurrentView();

bool Undo();
bool Redo();

bool init();

}  // namespace replayengine
