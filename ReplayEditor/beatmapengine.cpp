// clang-format off
#include "stdafx.h"
// clang-format on
#include "beatmapengine.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string_view>
#include <vector>

#include "config.hpp"
#include "hitobject.hpp"
#include "replayengine.hpp"

std::wstring beatmapengine::path;
float beatmapengine::stack_leniency;
float beatmapengine::hp;
float beatmapengine::cs;
float beatmapengine::od;
float beatmapengine::ar;
float beatmapengine::slider_mult;
float beatmapengine::slider_tick_rate;
SongTime_t beatmapengine::first_hitobject_ms;
SongTime_t beatmapengine::last_hitobject_ms;
bool beatmapengine::hitobjects_inverted;
std::vector<hitobject_t> beatmapengine::hitobjects;

#ifdef _DEBUG
// #define TRACE_TIMING_POINTS
#endif

struct timing_point_t {
    SongTime_t ms;
    float slider_velocity;
    float beat_length;
    bool operator<(const timing_point_t &rhs) const
    {
        return ms < rhs.ms;
    }
};

static std::vector<timing_point_t> timing_points;
static hitobject_t *root_hitobject = nullptr;

static bool timing_point_at(SongTime_t ms, timing_point_t &tp)
{
    if (timing_points.empty()) return false;
    if (timing_points.size() == 1) {
        tp = timing_points.front();
        return true;
    }
    auto item = std::upper_bound(timing_points.begin(), timing_points.end(), ms,
                                 [](const SongTime_t lhs, const timing_point_t &rhs) { return lhs < rhs.ms; });
    // when the start points are equal use found timing point
    // otherwise go to the previous one
    if (item == timing_points.end() || (item != timing_points.begin() && ms != item->ms)) --item;
    tp = *item;
    return true;
}

float beatmapengine::slider_velocity_at(SongTime_t ms)
{
    timing_point_t tp;
    if (timing_point_at(ms, tp))
        return tp.slider_velocity;
    else
        return 1.f;
}

float beatmapengine::beat_length_at(SongTime_t ms)
{
    timing_point_t tp;
    if (timing_point_at(ms, tp))
        return tp.beat_length;
    else
        return 1.f;
}

static uint32_t djb_hash(std::string_view str)
{
    uint32_t ret = 5381;
    for (auto c : str) {
        // ret * 33 + c
        ret = ((ret << 5) + ret) + c;
    }
    return ret;
}

static void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

static void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

static void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

static const char *match_on(const std::string &line, const char *tag)
{
    auto where = line.find(':');
    if (where == std::string::npos) return nullptr;
    for (unsigned i = 0; i < where; ++i) {
        if (tag[i] == '\0') break;
        if (line[i] != tag[i]) return nullptr;
    }
    return line.c_str() + where + 1;
}

static void match_float_full(const std::string &line, const char *tag, float *buf)
{
    const char *rhs = match_on(line, tag);
    if (!rhs) return;
    if (sscanf_s(rhs, "%f", buf) != 1) fatal(".osr bad format");
}

static void update_stacking(int start_index, int end_index)
{
    constexpr float STACK_LENIENCE = 3.f;
    const float stack_threshold = beatmapengine::preempt() * beatmapengine::stack_leniency;
    const int hitobject_count = static_cast<int>(beatmapengine::hitobjects.size());
    const float stack_offset = beatmapengine::circleradius() / 10.f;
    for (int i = start_index; i <= end_index; ++i) beatmapengine::hitobjects[i].stack_count = 0;
    int extended_end_index = end_index;
    for (int i = end_index; i >= start_index; --i) {
        int stack_base_index = i;
        for (int n = stack_base_index + 1; n < hitobject_count; ++n) {
            const auto &base_obj = beatmapengine::hitobjects[stack_base_index];
            if (!base_obj.is_circle_or_slider()) break;
            auto &obj = beatmapengine::hitobjects[n];
            if (!base_obj.is_circle_or_slider()) continue;
            if (obj.start - base_obj.end > stack_threshold) break;
            if (glm::distance(base_obj.pos, obj.pos) < STACK_LENIENCE ||
                (base_obj.slider && glm::distance(base_obj.slider->end_pos(), obj.pos) < STACK_LENIENCE)) {
                stack_base_index = n;
                obj.stack_count = 0;
            }
        }
        if (stack_base_index > extended_end_index) {
            extended_end_index = stack_base_index;
            if (extended_end_index == hitobject_count - 1) break;
        }
    }
    int extended_start_index = start_index;
    for (int i = extended_end_index; i > start_index; --i) {
        int n = i;
        hitobject_t *obj_i = &beatmapengine::hitobjects[i];
        if (obj_i->stack_count != 0 || !obj_i->is_circle_or_slider()) continue;
        if (obj_i->slider == nullptr) {
            while (--n >= 0) {
                hitobject_t *obj_n = &beatmapengine::hitobjects[n];
                if (!obj_n->is_circle_or_slider()) continue;
                if (obj_i->start - obj_n->end > stack_threshold) break;
                if (n < extended_start_index) {
                    obj_n->stack_count = 0;
                    extended_start_index = n;
                }
                if (obj_n->slider && glm::distance(obj_n->slider->end_pos(), obj_i->pos) < STACK_LENIENCE) {
                    const int offset = obj_i->stack_count - obj_n->stack_count + 1;
                    for (int j = n + 1; j <= i; ++j) {
                        hitobject_t *obj_j = &beatmapengine::hitobjects[j];
                        if (glm::distance(obj_n->slider->end_pos(), obj_j->pos) < STACK_LENIENCE)
                            obj_j->stack_count -= offset;
                    }
                    break;
                }
                if (glm::distance(obj_n->pos, obj_i->pos) < STACK_LENIENCE) {
                    obj_n->stack_count = obj_i->stack_count + 1;
                    obj_i = obj_n;
                }
            }
        } else {
            while (--n >= start_index) {
                hitobject_t *obj_n = &beatmapengine::hitobjects[n];
                if (!obj_n->is_circle_or_slider()) continue;
                if (obj_i->start - obj_n->start > stack_threshold) break;
                // if (glm::distance(obj_n->slider->end_pos(), obj_i->pos) < STACK_LENIENCE) {
                if (glm::distance(obj_n->pos, obj_i->pos) < STACK_LENIENCE) {
                    obj_n->stack_count = obj_i->stack_count + 1;
                    obj_i = obj_n;
                }
            }
        }
    }
    const glm::vec2 stack_offset_vector{stack_offset, stack_offset};
    for (int i = start_index; i <= end_index; ++i) {
        hitobject_t &obj = beatmapengine::hitobjects[i];
        const glm::vec2 offset = -stack_offset_vector * static_cast<float>(obj.stack_count);
        obj.pos += offset;
        if (obj.slider) obj.slider->translate_offset(offset);
    }
}

static void add_slider_ticks()
{
    using namespace beatmapengine;
    const size_t num_objects = hitobjects.size();
    constexpr size_t arbitrary_slider_tick_limit = 10000;
    constexpr SongTime_t legacy_last_tick_offset = 36;
    for (size_t i = 0; i < num_objects; ++i) {
        if (!hitobjects[i].slider) continue;
        constexpr float slider_leniency = 10.f;
        const int dt = ROUND_TO_INT(beat_length_at(hitobjects[i].start) / slider_tick_rate);
        SongTime_t t = hitobjects[i].start + dt;
        glm::vec2 pos;
        std::vector<int> just_added;
        while (hitobjects[i].slider->position_at_time(t, hitobjects[i].start, hitobjects[i].end, pos)) {
            if (glm::abs(t - hitobjects[i].start - hitobjects[i].slider->duration()) <= slider_leniency) break;
            just_added.emplace_back(static_cast<int>(hitobjects.size()));
            if (just_added.size() >= arbitrary_slider_tick_limit) {
                not_fatal("Slider tick limit reached");
                return;
            }
            hitobjects.emplace_back(pos, t, hitobjects[i].start + hitobjects[i].slider->duration(),
                                    HitObjectType::SliderTick);
            if (hitobjects.back().start > hitobjects.back().end) {
                hitobjects.back().start = hitobjects.back().end;
            }
            t += dt;
        }

        const SongTime_t dur = static_cast<SongTime_t>(hitobjects[i].slider->duration());
        SongTime_t this_tick_delay = 0;

        for (int k = 1; k < hitobjects[i].slider->repeat; ++k) {
            this_tick_delay = k * dur;
            const SongTime_t reverse_tick_aug = (k + 1) * dur + 2 * hitobjects[i].start;
            for (auto index : just_added) {
                hitobjects.emplace_back(hitobjects[index]);
                if (k % 2 == 0) {
                    hitobjects.back().start += this_tick_delay;
                    hitobjects.back().end += this_tick_delay;
                } else {
                    hitobjects.back().start = reverse_tick_aug - hitobjects.back().start;
                    hitobjects.back().end += this_tick_delay;
                }
                if (hitobjects.back().start > hitobjects.back().end) {
                    hitobjects.back().start = hitobjects.back().end;
                }
            }
        }
        // sliderend
        if ((hitobjects[i].slider->repeat % 2) == 0) {
            this_tick_delay = this_tick_delay + 0; 
        }
        const SongTime_t slider_end_offset = std::max(dur / 2, dur - legacy_last_tick_offset);
        const SongTime_t slider_end_adjusted_offset =
            (hitobjects[i].slider->repeat % 2) == 0 ? dur - slider_end_offset : slider_end_offset;

        hitobjects[i].slider->position_at_time(hitobjects[i].start + slider_end_adjusted_offset, hitobjects[i].start,
                                               hitobjects[i].end, pos);
        hitobjects.emplace_back(pos, hitobjects[i].start + this_tick_delay + slider_end_offset, hitobjects[i].end,
                                HitObjectType::SliderEnd);
        if (hitobjects.back().start > hitobjects.back().end) {
            hitobjects.back().start = hitobjects.back().end;
        }
    }
}

static void interval_tree_fill_max(hitobject_t *const root)
{
    root->max_in_subtree = root->animation_end();
    if (root->left) {
        interval_tree_fill_max(root->left);
        root->max_in_subtree = std::max(root->max_in_subtree, root->left->max_in_subtree);
    }
    if (root->right) {
        interval_tree_fill_max(root->right);
        root->max_in_subtree = std::max(root->max_in_subtree, root->right->max_in_subtree);
    }
}

static hitobject_t *build_interval_tree_partial(hitobject_t *const low, hitobject_t *const high)
{
    if (low >= high) return nullptr;
    hitobject_t *const mid = low + (high - low) / 2;
    mid->left = build_interval_tree_partial(low, mid);
    mid->right = build_interval_tree_partial(mid + 1, high);
    return mid;
}

static hitobject_t *build_interval_tree()
{
    using namespace beatmapengine;
    if (hitobjects.empty()) return nullptr;
    for (const auto &obj : hitobjects) {
        if (obj.start > obj.end) {
#ifdef _DEBUG
            trace(obj.start);
            trace(obj.end);
            trace(obj.hitobject_type);
            if (obj.slider) {
                trace(obj.slider->pixel_length);
                trace(obj.slider->repeat);
                trace(obj.slider->velocity);
                trace(obj.slider->duration());
            }
#endif
            not_fatal("Issue while processing beatmap: start time after end time");
            return nullptr;
        }
    }
    // using this "arr" variable because MSVC aborts when you subscript a vector out of range
    // (it didn't happen in GCC)
    hitobject_t *const arr = hitobjects.data();
    hitobject_t *const root = build_interval_tree_partial(arr, arr + hitobjects.size());
    interval_tree_fill_max(root);
    return root;
}

static void interval_tree_search(const hitobject_t *const root, const SongTime_t ms,
                                 std::vector<const hitobject_t *> &found)
{
    if (root == nullptr) return;
    if (root->max_in_subtree < ms) return;
    interval_tree_search(root->left, ms, found);
    if (ms >= root->animation_start()) {
        if (root->animation_end() >= ms) {
            found.emplace_back(root);
        }
        interval_tree_search(root->right, ms, found);
    }
}

/*
static void match_int_full(const std::string &line, const char *tag, int *buf)
{
        const char *rhs = match_on(line, tag);
        if (! rhs)
                return;
        if (sscanf_s(rhs, "%i", buf) != 1)
                fatal(".osr bad format");
}
*/

#define match_float(tag, buf) (match_float_full(line, tag, buf))
#define match_int(tag, buf) (match_int_full(line, tag, buf))

// hashes of various sections
#define S_General 1196426939
#define S_Editor 662553860
#define S_Metadata 2949061118
#define S_Difficulty 3791553680
#define S_Events 1362037106
#define S_TimingPoints 3771252386
#define S_Colours 2484839652
#define S_HitObjects 279667276

bool beatmapengine::init(const std::wstring &fname)
{
    timing_points.clear();
    for (auto obj : hitobjects) {
        obj.destroy();
    }
    root_hitobject = nullptr;
    hitobjects.clear();
    hitobjects_inverted = false;
    if (fname.empty()) return true;
    const std::wstring full_path = config::song_path + fname;
    std::ifstream r(full_path);
    if (!r.good()) {
        not_fatal("osu file couldn't be opened");
        return false;
    }
    std::string line;
    uint32_t current_section = 0;
    // ignoring version line
    std::getline(r, line);
    timing_point_t last_tp;
    last_tp.ms = 0;
    last_tp.beat_length = 1;
    last_tp.slider_velocity = 1;
    timing_points.clear();
    stack_leniency = 1.f;
    hp = 5.f;
    cs = 5.f;
    od = 5.f;
    ar = 5.f;
    slider_mult = 1.4f;
    slider_tick_rate = 1.f;
    while (std::getline(r, line)) {
        if (line.length() < 2) continue;
        if (line.find("Unicode") != std::string::npos) continue;
        if (std::find_if(line.begin(), line.end(), [](int ch) { return ch < ' ' || ch >= 128; }) != line.end()) {
            continue;
        }
        trim(line);
        if (line.length() < 2) continue;
        if (line[0] == '/' && line[1] == '/') continue;
        if (line[0] == '[') {
            current_section = djb_hash(line);
            continue;
        }
        switch (current_section) {
            case 0:
                break;
            case S_General:
                match_float("StackLeniency", &stack_leniency);
                break;
            case S_Editor:
                break;
            case S_Metadata:
                break;
            case S_Difficulty:
                match_float("HPDrainRate", &hp);
                match_float("CircleSize", &cs);
                match_float("OverallDifficulty", &od);
                match_float("ApproachRate", &ar);
                match_float("SliderMultiplier", &slider_mult);
                match_float("SliderTickRate", &slider_tick_rate);
                break;
            case S_Events:
                break;
            case S_TimingPoints: {
                float beatlength, ms_float;
                int meter, sample_set, sample_index, volume, uninherited, effects;
                sscanf_s(line.c_str(), "%f,%f,%d,%d,%d,%d,%d,%d", &ms_float, &beatlength, &meter, &sample_set,
                         &sample_index, &volume, &uninherited, &effects);
                timing_point_t tp;
                tp.ms = static_cast<SongTime_t>(std::ceil(ms_float));
                if (beatlength > 0) {
                    const float velocity = 100.f / beatlength;
                    tp.slider_velocity = velocity;
                    tp.beat_length = beatlength;
                    last_tp = tp;
                } else {
                    const float mult = -100.0f / beatlength;
                    tp.slider_velocity = mult * last_tp.slider_velocity;
                    tp.beat_length = last_tp.beat_length;
                }
#ifdef TRACE_TIMING_POINTS
                TRACE << "[" << (uninherited ? 'X' : ' ') << "] ms_float = " << ms_float
                      << "; beatlength = " << beatlength << "; slider velocity = " << tp.slider_velocity << std::endl;
#endif
                timing_points.push_back(tp);
                break;
            }
            case S_Colours:
                break;
            case S_HitObjects: {
                hitobjects.emplace_back(line.c_str());
                break;
            }
        }
    }
    std::stable_sort(timing_points.begin(), timing_points.end());
    std::stable_sort(hitobjects.begin(), hitobjects.end());
    if (hitobjects.empty()) {
        first_hitobject_ms = 0;
        last_hitobject_ms = 1;
    } else {
        first_hitobject_ms = hitobjects.front().animation_start();
        last_hitobject_ms = hitobjects.back().animation_end();
    }
    update_stacking(0, static_cast<int>(hitobjects.size() - 1));
    add_slider_ticks();
    std::stable_sort(hitobjects.begin(), hitobjects.end());
    root_hitobject = build_interval_tree();
    return true;
}

void beatmapengine::draw(SongTime_t ms, bool draw_sliderend_range)
{
    static std::vector<const hitobject_t *> objs_to_draw;
    objs_to_draw.clear();
    interval_tree_search(root_hitobject, ms, objs_to_draw);
    for (int i = static_cast<int>(objs_to_draw.size()) - 1; i >= 0; --i) {
        objs_to_draw[i]->draw_bg(ms, draw_sliderend_range);
    }
    for (int i = static_cast<int>(objs_to_draw.size()) - 1; i >= 0; --i) {
        objs_to_draw[i]->draw_fg(ms);
    }
}

const SongTime_t trail_length = 1000;

void beatmapengine::draw_windows(SongTime_t ms)
{
    auto start = std::lower_bound(hitobjects.begin(), hitobjects.end(), ms - trail_length,
                                  [](const hitobject_t &lhs, SongTime_t rhs) { return lhs.start < rhs; });
    auto end = std::lower_bound(hitobjects.begin(), hitobjects.end(), ms,
                                [](const hitobject_t &lhs, SongTime_t rhs) { return lhs.start < rhs; });
    if (start == hitobjects.end()) return;
    if (end != hitobjects.end()) ++end;

    // glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);

    glColor4f(1, 0.5, 0, 1);
    for (auto i = start; i != end; ++i) {
        i->draw_window(ms, hitwindow50());
    }

    glColor4f(0, 1, 0, 1);
    for (auto i = start; i != end; ++i) {
        i->draw_window(ms, hitwindow100());
    }

    glColor4f(0, 0, 1, 1);  
    for (auto i = start; i != end; ++i) {
        i->draw_window(ms, hitwindow300());
    }

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}