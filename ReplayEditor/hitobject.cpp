// clang-format off
#include "stdafx.h"
// clang-format on
#include "hitobject.hpp"

#include <cstdint>

#include "beatmapengine.hpp"
#include "replayengine.hpp"
#include "texture.hpp"
#include "ui.hpp"

#define MIN_SONG_TIME INT32_MIN

#define KIND_IS_CIRCLE (kind & 1)
#define KIND_IS_SLIDER (kind & 2)
#define KIND_IS_SPINNER (kind & 8)

static float approach(const SongTime_t start, const SongTime_t ms)
{
    const SongTime_t pre = beatmapengine::preempt();
    const SongTime_t first_seen = start - pre;
    const float res = RATIO(ms - first_seen, pre);
    return glm::clamp(res, 0.f, 1.f);
}

static float opacity(const SongTime_t start, const SongTime_t end, const SongTime_t ms)
{
    const SongTime_t pre = beatmapengine::preempt();
    const SongTime_t fade = beatmapengine::fadein();
    const SongTime_t hot = start - pre + fade;
    if (ms < start - pre) return 0.0f;
    if (ms < hot) {
        const SongTime_t top = ms - (start - pre);
        return RATIO(top, fade);
    }
    if (ms > end) {
        const SongTime_t top = ms - end;
        return 1.0f - RATIO(top, beatmapengine::fadeout());
    }
    return 1.0f;
}

static glm::vec2 invert_vec(const glm::vec2 &v)
{
    return glm::vec2(v.x, 384.0 - v.y);
}

static unsigned char comma_ahead(const char *&def, int amt)
{
    while (*def) {
        if (amt == 0) return 0;
        if (*def == ',') --amt;
        ++def;
    }
    return 1;
}

hitobject_t::hitobject_t(const char *def) noexcept
    : is_miss(true), hit_error(0), stack_count(0), left(nullptr), right(nullptr), max_in_subtree(MIN_SONG_TIME)
{
    int kind;
    if (sscanf_s(def, "%f,%f,%d,%d", &pos.x, &pos.y, &start, &kind) != 4) fatal(".osr bad format");
    slider = nullptr;
    if (KIND_IS_CIRCLE) {
        end = start;
        hitobject_type = HitObjectType::Circle;
    } else if (KIND_IS_SLIDER) {
        if (comma_ahead(def, 5)) fatal("bad slider def");
        slider = slider_t::from_def(def, pos, start, end);
        end = start + static_cast<SongTime_t>(slider->duration() * slider->repeat);
        hitobject_type = HitObjectType::Slider;
    } else if (KIND_IS_SPINNER) {
        if (comma_ahead(def, 5)) fatal("bad spinner def");
        if (sscanf_s(def, "%d", &end) != 1) fatal(".osr bad format");
        hitobject_type = HitObjectType::Spinner;
        if (start >= end) {
            end = start + 1;
        }
    } else {
        fatal(".osr bad format");
        end = start;
        hitobject_type = HitObjectType::Circle;
    }
}

hitobject_t::hitobject_t(const glm::vec2 &my_pos, SongTime_t my_start, SongTime_t my_end,
                         HitObjectType my_hit_object_type) noexcept
    : pos(my_pos),
      start(my_start),
      end(my_end),
      hitobject_type(my_hit_object_type),
      slider(nullptr),
      is_miss(true),
      hit_error(0),
      stack_count(0),
      left(nullptr),
      right(nullptr),
      max_in_subtree(MIN_SONG_TIME)
{
}

void hitobject_t::draw_bg(SongTime_t ms, bool draw_sliderend_range) const
{
    switch (hitobject_type) {
        case HitObjectType::Circle:
        case HitObjectType::Slider: {
            const float o = opacity(start, end, ms);
            glm::vec2 size = glm::vec2(beatmapengine::circleradius() * 2.0f);
            if (slider) {
                slider->draw(ms, start, end, o, draw_sliderend_range);
                glColor4f(1.0f, 1.0f, 1.0f, o);
                /*if (beatmapengine::hitobjects_inverted)
                    textures::hitcircle->draw(invert_vec(slider->end_pos()), size * 0.5f, size);
                else
                    textures::hitcircle->draw(slider->end_pos(), size * 0.5f, size);*/
            }
            glColor4f(1.0f, 1.0f, 1.0f, o);
            if (beatmapengine::hitobjects_inverted)
                textures::hitcircle->draw(invert_vec(pos), size * 0.5f, size);
            else
                textures::hitcircle->draw(pos, size * 0.5f, size);
            break;
        }
    }
}

void hitobject_t::draw_fg(SongTime_t ms) const
{
    const float o = opacity(start, end, ms);
    switch (hitobject_type) {
        case HitObjectType::Spinner: {
            glColor4f(0.0f, 0.5f, 0.5f, o);
            const glm::vec2 smallsize(25.0f, 25.0f);
            const glm::vec2 bigsize(350.0f, 350.0f);
            const glm::vec2 middle(256.0f, 192.0);
            const float amt = glm::clamp(RATIO(ms - start, end - start), 0.f, 1.f);
            const glm::vec2 varsize = glm::mix(bigsize, smallsize, amt);
            textures::approachcircle->draw(middle, smallsize * 0.5f, smallsize);
            textures::approachcircle->draw(middle, varsize * 0.5f, varsize);
            break;
        }
        case HitObjectType::Circle:
        case HitObjectType::Slider: {
            glColor4f(1.0f, 1.0f, 1.0f, o);
            glm::vec2 size = glm::vec2(beatmapengine::circleradius() * 2.0f);
            size *= glm::mix(3.5f, 1.0f, approach(start, ms));
            if (beatmapengine::hitobjects_inverted)
                textures::approachcircle->draw(invert_vec(pos), size * 0.5f, size);
            else
                textures::approachcircle->draw(pos, size * 0.5f, size);
            break;
        }
        case HitObjectType::SliderTick: {
            glm::vec2 size = glm::vec2(beatmapengine::circleradius() * 0.5f);
            glColor4f(1.0f, 1.0f, 1.0f, o);
            if (beatmapengine::hitobjects_inverted)
                textures::slidertick->draw(invert_vec(pos), size * 0.5f, size);
            else
                textures::slidertick->draw(pos, size * 0.5f, size);
            break;
        }
        case HitObjectType::SliderEnd: {
            glm::vec2 size = glm::vec2(beatmapengine::circleradius() * 2.0f);
            glColor4f(0.0f, 1.0f, 1.0f, o);
            if (beatmapengine::hitobjects_inverted)
                textures::slideredge->draw(invert_vec(pos), size * 0.5f, size);
            else
                textures::slideredge->draw(pos, size * 0.5f, size);
            break;
        }
    }
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(1, 1);
    glEnd();
}

void _draw_window(SongTime_t object_time, SongTime_t current_time, SongTime_t window)
{
    constexpr float bar_top_y = -0.95f;
    constexpr float bar_height = 0.05f;

    glBegin(GL_QUADS);
    const float xpos1 = 2.f * RATIO(object_time - current_time - window, ui::trail_length) + 1.f;
    const float xpos2 = 2.f * RATIO(object_time - current_time + window, ui::trail_length) + 1.f;
    glVertex2f(xpos1, bar_top_y - bar_height / 8);
    glVertex2f(xpos1, bar_top_y + bar_height / 8);
    glVertex2f(xpos2, bar_top_y + bar_height / 8);
    glVertex2f(xpos2, bar_top_y - bar_height / 8);
    glEnd();
}

void hitobject_t::draw_window(SongTime_t ms, SongTime_t window) const
{
    

    switch (hitobject_type) {
        case HitObjectType::Circle:
        case HitObjectType::Slider:
            _draw_window(this->start, ms, window);
    }
    
    
}

void hitobject_t::destroy()
{
    if (slider) {
        delete slider;
        slider = nullptr;
    }
}

SongTime_t hitobject_t::animation_start() const
{
    return start - beatmapengine::preempt();
}

SongTime_t hitobject_t::animation_end() const
{
    return end + beatmapengine::fadeout();
}
