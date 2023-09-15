// clang-format off
#include "stdafx.h"
// clang-format on
#include "accuracy_analyzer.hpp"

#include <algorithm>
#include <fstream>
#include <functional>

#include "audioengine.hpp"
#include "beatmapengine.hpp"
#include "replayengine.hpp"

template <typename T>
class Averager
{
   private:
    T sum = 0;
    int count = 0;

   public:
    void add(T val)
    {
        sum += val;
        ++count;
    }
    float avg() const
    {
        if (count == 0) return 0.f;
        return RATIO(sum, count);
    }
};

void accuracy_analyzer::analyze(Stats* stats, bool do_trace)
{
    std::ofstream log;
    if (do_trace) log.open("accuracy_analyzer.log");
    if (!log.good()) do_trace = false;
    const float radius = beatmapengine::circleradius();
    const SongTime_t window50 = beatmapengine::hitwindow50();
    const SongTime_t window100 = beatmapengine::hitwindow100();
    const SongTime_t window300 = beatmapengine::hitwindow300();
    if (do_trace) {
        log << "++++++++++++++++++++++++++++++++" << std::endl;
        log << "Circle radius = " << radius << std::endl;
        log << "Window50 = " << window50 << std::endl;
        log << "Window100 = " << window100 << std::endl;
        log << "Window300 = " << window300 << std::endl;
        log << "Number of hitobjects = " << beatmapengine::hitobjects.size() << std::endl;
        log << "Number of replay frames = " << replayengine::CurrentView()->frames().size() << std::endl;
        log << "++++++++++++++++++++++++++++++++" << std::endl;
        log << "SldTk means slider tick; bitmap is prev frame pressed, curr frame pressed, inside slider ball"
            << std::endl;
        log << "++++++++++++++++++++++++++++++++" << std::endl;
    }
    size_t replayframe_index = 1;
    size_t hitobject_index = 0;
    for (size_t i = 0; i < beatmapengine::hitobjects.size(); ++i) {
        beatmapengine::hitobjects[i].is_miss = true;
    }

    bool is_holding;
    const slider_t* curr_slider = nullptr;
    SongTime_t curr_slider_start = 0;

    while (replayframe_index < replayengine::CurrentView()->frames().size() &&
           hitobject_index < beatmapengine::hitobjects.size()) {
        const auto& curr_frame = replayengine::CurrentView()->frames()[replayframe_index];
        const auto& prev_frame = replayengine::CurrentView()->frames()[replayframe_index - 1];
        const int is_press = IS_PRESSED_INT(prev_frame.pressed_mouse1(), prev_frame.pressed_mouse2(),
                                         curr_frame.pressed_mouse1(), curr_frame.pressed_mouse2());
        auto& curr_obj = beatmapengine::hitobjects[hitobject_index];
        if (curr_obj.hitobject_type == HitObjectType::Slider) {
            is_holding = true;
            curr_slider = curr_obj.slider;
            curr_slider_start = curr_obj.start;
        }

        if (curr_obj.hitobject_type == HitObjectType::SliderTick ||
            curr_obj.hitobject_type == HitObjectType::SliderEnd) {
            
            //const float ball_radius = beatmapengine::circleradius() * (is_holding ? 2.4f : 1.0f);
            // This is not working even though the algo seems correct
            const float ball_radius = beatmapengine::circleradius() * 2.4f;

            glm::vec2 sliderball_pos;
            bool inside_slider_ball;

            const bool prev_frame_pressed = prev_frame.pressed_mouse1() || prev_frame.pressed_mouse2();
            const bool curr_frame_pressed = curr_frame.pressed_mouse1() || curr_frame.pressed_mouse2();

            // check is inside slidercircle

            if (curr_slider_start > curr_frame.ms)
            {
                ++replayframe_index;
                continue;
            }

            curr_slider->position_at_time(curr_frame.ms, sliderball_pos);

            if (beatmapengine::hitobjects_inverted) sliderball_pos.y = 384.f - sliderball_pos.y;
            inside_slider_ball = glm::distance(sliderball_pos, curr_frame.p) <= ball_radius;

            is_holding = prev_frame_pressed && curr_frame_pressed && inside_slider_ball;

            if (curr_frame.ms < curr_obj.start) {
                // if we are not yet at the tick interval then wait until we are
                ++replayframe_index;
                continue;
            }

            float interpolation = float(curr_obj.start - prev_frame.ms) / (curr_frame.ms - prev_frame.ms);
            glm::vec2 frame_pos = glm::mix(prev_frame.p, curr_frame.p, interpolation);
            glm::vec2 hitobj_pos = curr_obj.pos;
            if (beatmapengine::hitobjects_inverted) hitobj_pos.y = 384.f - hitobj_pos.y;

            inside_slider_ball = glm::distance(hitobj_pos, frame_pos) <= ball_radius;
            if (do_trace)
                log << "SldTk: Object #" << hitobject_index << "; Frame #" << replayframe_index << "; "
                    << prev_frame_pressed << curr_frame_pressed << inside_slider_ball << std::endl;
            if (prev_frame_pressed && curr_frame_pressed && inside_slider_ball) {
                // all conditions met for good slider tick
                curr_obj.is_miss = false;
                curr_obj.hit_error = 0;
            }
            else
            {
                curr_obj.is_miss = true;
            }
            ++hitobject_index;
        } 
        else
        {
            if (curr_frame.ms > curr_obj.start + window50) {
                if (do_trace)
                    log << "Press: Object #" << hitobject_index << "; Frame #" << replayframe_index
                        << "; Miss - Past hit window" << std::endl;
                // if it has already passed the hit window then count as miss
                ++hitobject_index;
                continue;
            }

            if (is_press) {
                if (curr_obj.hitobject_type == HitObjectType::Spinner) {
                    if (do_trace)
                        log << "Press: Object #" << hitobject_index << "; Frame #" << replayframe_index
                            << "; Spinner skipped" << std::endl;
                    // spinners right now count as perfect
                    curr_obj.is_miss = false;
                    curr_obj.hit_error = 0;
                    ++hitobject_index;
                    continue;
                }
                if (curr_frame.ms < curr_obj.animation_start()) {
                    // if it is not on screen then ignore this hit
                    ++replayframe_index;
                    continue;
                }

                glm::vec2 hitobj_pos = curr_obj.pos;
                if (beatmapengine::hitobjects_inverted) hitobj_pos.y = 384.f - hitobj_pos.y;
                if (glm::distance(hitobj_pos, curr_frame.p) > radius) {
                    if (do_trace)
                        log << "Press: Object #" << hitobject_index << "; Frame #" << replayframe_index
                            << "; Ignore - missed hitcircle" << std::endl;
                    // if this hit misses the circle then ignore it
                    ++replayframe_index;
                    continue;
                }
                // we have a hit
                int error = curr_frame.ms - curr_obj.start;
                if (std::abs(error) <= window50) {
                    if (do_trace)
                        log << "Press: Object #" << hitobject_index << "; Frame #" << replayframe_index << "; Hit ("
                            << error << " ms)" << std::endl;
                    curr_obj.is_miss = false;
                    curr_obj.hit_error = error;
                }
                ++hitobject_index;

                // if doubletapped
                if (is_press == 2) {
                    if (hitobject_index == beatmapengine::hitobjects.size()) continue;

                    auto& next_obj = beatmapengine::hitobjects[hitobject_index];

                    if (curr_frame.ms < next_obj.animation_start()) {
                        // if it is not on screen then ignore this hit
                        ++replayframe_index;
                        continue;
                    }

                    glm::vec2 hitobj_pos = next_obj.pos;
                    if (beatmapengine::hitobjects_inverted) hitobj_pos.y = 384.f - hitobj_pos.y;
                    if (glm::distance(hitobj_pos, curr_frame.p) > radius) {
                        if (do_trace)
                            log << "Press: Object #" << hitobject_index << "; Frame #" << replayframe_index
                                << "; Ignore - missed hitcircle" << std::endl;
                        // if this hit misses the circle then ignore it
                        ++replayframe_index;
                        continue;
                    }

                    error = curr_frame.ms - next_obj.start;
                    if (std::abs(error) <= window50) {
                        if (do_trace)
                            log << "Press: Object #" << hitobject_index << "; Frame #" << replayframe_index << "; Hit ("
                                << error << " ms)" << std::endl;
                        next_obj.is_miss = false;
                        next_obj.hit_error = error;
                    }
                    ++hitobject_index;
                }
            }
            ++replayframe_index;
        }

        
    }
    if (stats == nullptr) return;
    Averager<int> avg_all, avg_neg, avg_pos;
    Averager<float> variance;
    for (const auto& obj : beatmapengine::hitobjects) {
        const int pts = obj.get_points(window50, window100, window300);
        switch (pts) {
            case 300:
                ++stats->num_300;
                break;
            case 100:
                ++stats->num_100;
                break;
            case 50:
                ++stats->num_50;
                break;
            default:
                ++stats->num_miss;
                break;
        }
        if (!obj.is_miss) {
            avg_all.add(obj.hit_error);
            if (obj.hit_error >= 0) {
                avg_pos.add(obj.hit_error);
            } else {
                avg_neg.add(obj.hit_error);
            }
        }
    }
    const float avg = avg_all.avg();
    for (const auto& obj : beatmapengine::hitobjects) {
        if (!obj.is_miss) variance.add(std::pow(obj.hit_error - avg, 2.f));
    }
    stats->accuracy =
        100.f * RATIO(stats->num_50 + 2 * stats->num_100 + 6 * stats->num_300, 6 * beatmapengine::hitobjects.size());
    stats->avg_early = avg_neg.avg();
    stats->avg_late = avg_pos.avg();
    stats->unstable_rate = 10.f * std::sqrt(variance.avg());
}

int accuracy_analyzer::next_hitobject(std::function<bool(const hitobject_t&)> func)
{
    auto iter = std::lower_bound(beatmapengine::hitobjects.begin(), beatmapengine::hitobjects.end(),
                                 audioengine::handle->get_time(),
                                 [](const hitobject_t& lhs, double rhs) { return lhs.start < rhs; });
    for (; iter != beatmapengine::hitobjects.end(); ++iter) {
        if (func(*iter)) {
            audioengine::handle->jump_to(iter->start + 1);
            return static_cast<int>(iter - beatmapengine::hitobjects.begin());
        }
    }
    return -1;
}
