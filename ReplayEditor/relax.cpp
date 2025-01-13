// clang-format off
#include "stdafx.h"
// clang-format on

#include "relax.hpp"

#include <algorithm>
#include <functional>
#include <vector>

#include "beatmapengine.hpp"
#include "replayengine.hpp"

namespace relax
{
namespace
{

using FrameSelection = replayengine::Replay::ReplayFrameSelection;

class HitobjectRange
{
   public:
    explicit HitobjectRange(const hitobject_t *const data, const size_t len) : m_begin(data), m_end(data + len)
    {
    }

    const hitobject_t *begin() const
    {
        return m_begin;
    }

    const hitobject_t *end() const
    {
        return m_end;
    }

   private:
    const hitobject_t *const m_begin = nullptr;
    const hitobject_t *const m_end = nullptr;
};

void reset_all_presses()
{
    replayengine::MutateCurrentView([](replayengine::Replay &replay) {
        for (auto &frame : replay.mut_frames()) {
            frame.keys = 0;
        }
        return true;
    });
}

void reset_all_presses_in_selection()
{
    replayengine::MutateCurrentView([](replayengine::Replay &replay) {
        if (!replay.are_in_out_marks_consistent()) {
            return false;
        }
        for (auto &frame : replay.mut_selection()) {
            frame.keys = 0;
        }
        return true;
    });
}

FrameSelection find_all_frames_between(std::vector<replayengine::ReplayFrame> &frames, const SongTime_t ms_start,
                                       const SongTime_t ms_end)
{
    using ::replayengine::ReplayFrame;

    auto start = std::lower_bound(frames.begin(), frames.end(), ms_start, CmpMs<ReplayFrame>());
    auto end = std::lower_bound(frames.begin(), frames.end(), ms_end, CmpMs<ReplayFrame>());
    return FrameSelection(frames.data(), start - frames.begin(), end - frames.begin() - 1);
}

HitobjectRange find_all_hitobjects_between(const SongTime_t ms_start, const SongTime_t ms_end)
{
    const int hitwindow = beatmapengine::hitwindow300() - 10;
    auto start = std::lower_bound(beatmapengine::hitobjects.begin(), beatmapengine::hitobjects.end(), ms_start,
                                  [hitwindow](const hitobject_t &obj, const SongTime_t ms) { return obj.end < ms; });
    auto end = std::lower_bound(
        beatmapengine::hitobjects.begin(), beatmapengine::hitobjects.end(), ms_end,
        [hitwindow](const hitobject_t &obj, const SongTime_t ms) { return obj.start - hitwindow < ms; });
    if (start > end) {
        return HitobjectRange(&(*start), 0);
    }
    return HitobjectRange(&(*start), end - start);
}

void recalculate_all_hits_in_view(HitobjectRange hitobjects, const SongTime_t *const ms_restriction_low,
                                  const SongTime_t *const ms_restriction_high)
{
    const float radius = beatmapengine::circleradius();
    std::vector<replayengine::ReplayFrame> &frames = replayengine::MutableCurrentView()->mut_frames();
    bool alternate_k1 = false;
    const int hitwindow = beatmapengine::hitwindow300() - 10;
    for (const auto &obj : hitobjects) {
        for (auto &frame : find_all_frames_between(frames, obj.start - hitwindow, obj.end + hitwindow)) {
            if (ms_restriction_low != nullptr && frame.ms < *ms_restriction_low) {
                continue;
            }
            if (ms_restriction_high != nullptr && frame.ms > *ms_restriction_high) {
                break;
            }
            const int hitkey = (alternate_k1) ? 10 : 5;
            switch (obj.hitobject_type) {
                case HitObjectType::Circle:
                    if (glm::distance(obj.pos, frame.p) <= radius) {
                        frame.keys = hitkey;
                    }
                    break;
                case HitObjectType::Slider:
                    if (glm::distance(obj.pos, frame.p) <= radius || frame.ms > obj.start + hitwindow) {
                        frame.keys = hitkey;
                    }
                    break;
                case HitObjectType::Spinner:
                    frame.keys = hitkey;
                    break;
            }
        }
        alternate_k1 = !alternate_k1;
    }
}

}  // namespace

void recalculate_all_hits()
{
    // By mutating the current view, it creates an undo point before continuing.
    reset_all_presses();
    recalculate_all_hits_in_view(HitobjectRange(beatmapengine::hitobjects.data(), beatmapengine::hitobjects.size()),
                                 nullptr, nullptr);
}

void recalculate_hits_in_selection()
{
    if (!replayengine::CurrentView()->are_in_out_marks_consistent()) {
        not_fatal("Not a valid selection");
        return;
    }
    // By mutating the current view, it creates an undo point before continuing.
    reset_all_presses_in_selection();
    const SongTime_t first_selection_time = replayengine::CurrentView()->mark_in_frame().ms;
    const SongTime_t last_selection_time = replayengine::CurrentView()->mark_out_frame().ms;
    recalculate_all_hits_in_view(find_all_hitobjects_between(first_selection_time, last_selection_time),
                                 &first_selection_time, &last_selection_time);
}

}  // namespace relax