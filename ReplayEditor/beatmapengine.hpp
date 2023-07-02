#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include "hitobject.hpp"

namespace beatmapengine
{

bool init(const std::wstring &f);
float slider_velocity_at(SongTime_t);
float beat_length_at(SongTime_t);
void draw(SongTime_t);
extern std::vector<hitobject_t> hitobjects;
extern float stack_leniency;
extern float hp;
extern float cs;
extern float od;
extern float ar;
extern float slider_mult;
extern float slider_tick_rate;
static constexpr inline float map_difficulty_range(float value, float low, float mid, float high)
{
    if (value > 5) return mid + (high - mid) * (value - 5.f) / 5.f;
    if (value < 5) return mid - (mid - low) * (5.f - value) / 5.f;
    return mid;
}
static inline float circleradius()
{
    return 54.4f - 4.48f * cs;
}
static inline float spinnerratio()
{
    return map_difficulty_range(od, 3.f, 5.f, 7.5f);
}
static inline SongTime_t hitwindow50()
{
    return static_cast<SongTime_t>(map_difficulty_range(od, 200.f, 150.f, 100.f));
}
static inline SongTime_t hitwindow100()
{
    return static_cast<SongTime_t>(map_difficulty_range(od, 140.f, 100.f, 60.f));
}
static inline SongTime_t hitwindow300()
{
    return static_cast<SongTime_t>(map_difficulty_range(od, 80.f, 50.f, 20.f));
}
static inline SongTime_t preempt()
{
    return static_cast<SongTime_t>(map_difficulty_range(ar, 1800.f, 1200.f, 450.f));
}
static inline SongTime_t fadein()
{
    return static_cast<SongTime_t>(map_difficulty_range(ar, 1200.f, 800.f, 300.f));
}
static constexpr inline SongTime_t fadeout()
{
    return 200;
}
extern SongTime_t first_hitobject_ms;
extern SongTime_t last_hitobject_ms;
extern bool hitobjects_inverted;

};  // namespace beatmapengine
