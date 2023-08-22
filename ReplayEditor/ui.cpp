// clang-format off
#include "stdafx.h"
// clang-format on

#include "ui.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

#include "audioengine.hpp"
#include "replayengine.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include "zoom_pan.hpp"

#define FLIP_Y(v) (v).y = -(v).y

ui::TrailMode ui::trail_mode = ui::TrailMode::Hits;
SongTime_t ui::trail_length = 1000;

bool ui::draw_frames_on_timeline = true;
bool ui::draw_hitwindows_on_timeline = true;
bool ui::draw_sliderend_range = false;

static glm::vec2 pan_start;
static glm::vec2 pan_mouse_start;
static bool is_panning = false;

static void do_mouse_pan(const glm::vec2 &p);

void ui::mouse_move(const glm::vec2 &p)
{
    if (is_panning) {
        do_mouse_pan(p);
    }
    tool::current_tool->OnMouseMove(p);
}

void ui::mouse_down(const glm::vec2 &p)
{
    bool can_grab = false;
    if (replayengine::CurrentView()->are_all_marks_consistent()) {
        constexpr float RADIUS = 35.f;
        if (glm::distance(replayengine::CurrentView()->mark_mid_frame().p, p) <=
            RADIUS) {
            can_grab = true;
        }
    }
    if (tool::CurrentToolType() == tool::ToolType::Select || tool::CurrentToolType() == tool::ToolType::Grab) {
        tool::CurrentToolType(can_grab ? tool::ToolType::Grab : tool::ToolType::Select);
    }
    tool::current_tool->OnMouseMove(p);
    tool::current_tool->OnMouseDown(p);
}

void ui::mouse_up(const glm::vec2 &p)
{
    tool::current_tool->OnMouseMove(p);
    tool::current_tool->OnMouseUp(p);
}

template <typename T>
static void draw_marks(const T &iter)
{
    const I64 index = iter - replayengine::CurrentView()->frames().begin();
    const bool m_in = index == replayengine::CurrentView()->mark_in();
    const bool m_out = index == replayengine::CurrentView()->mark_out();
    const bool m_mid = index == replayengine::CurrentView()->mark_mid();
    const texture_t *texture = nullptr;
    if (m_in) {
        texture = textures::markerA;
        glColorGreen();
    } else if (m_out) {
        texture = textures::markerB;
        glColorGreen();
    } else if (m_mid) {
        texture = textures::markerA;
        glColorWhite();
    } else {
        return;
    }
    const glm::vec2 mark_size(32.f);
    texture->draw(iter->p, mark_size * 0.5f, mark_size);
}

static void quad_vertex(const glm::vec2 &p, const glm::vec2 &d)
{
    glVertex2f(p.x, p.y);
    glVertex2f(p.x + d.x, p.y);
    glVertex2f(p.x + d.x, p.y + d.y);
    glVertex2f(p.x, p.y + d.y);
}

static void draw_frames(const SongTime_t ms, bool draw_frames_on_timeline)
{
    using namespace replayengine;
    if (ui::trail_mode == ui::TrailMode::Off) return;
    const std::vector<ReplayFrame> &frames = CurrentView()->frames();
    auto start = std::lower_bound(frames.begin(), frames.end(), ms - ui::trail_length, CmpMs<ReplayFrame>());
    auto end = std::lower_bound(frames.begin(), frames.end(), ms, CmpMs<ReplayFrame>());
    if (start == frames.end()) return;
    if (end != frames.end()) ++end;
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_STRIP);
    glColorRed();
    bool is_color_red = true;
    for (auto iter = start; iter != end; ++iter) {
        const I64 index = iter - replayengine::CurrentView()->frames().begin();
        if (index >= CurrentView()->mark_in() && index <= CurrentView()->mark_out()) {
            if (is_color_red) {
                glColor3f(0.4f, 0.f, 1.f);
                is_color_red = false;
            }
        } else {
            if (!is_color_red) {
                glColorRed();
                is_color_red = true;
            }
        }
        glVertex2f(iter->p.x, iter->p.y);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    if (ui::trail_mode == ui::TrailMode::Raw) {
        const glm::vec2 vertex_size(12.0f);
        for (auto iter = start; iter != end; ++iter) {
            if (iter->pressed_mouse1() && iter->pressed_mouse2()) {
                glColorYellow();
            } else if (iter->pressed_mouse1()) {
                glColorCyan();
            } else if (iter->pressed_mouse2()) {
                glColorMagenta();
            } else {
                glColorGray();
            }
            textures::vertex->draw(iter->p, vertex_size * 0.5f, vertex_size);
            draw_marks(iter);
        }
    } else if (ui::trail_mode == ui::TrailMode::Hits) {
        const glm::vec2 vertex_size(16.0f);
        auto prev = start;
        for (auto curr = start + 1; curr != end; ++curr, ++prev) {
            const bool is_press = IS_PRESSED(prev->pressed_mouse1(), prev->pressed_mouse2(), curr->pressed_mouse1(),
                                             curr->pressed_mouse2());
            const bool is_release = IS_RELEASE(prev->pressed_mouse1(), prev->pressed_mouse2(), curr->pressed_mouse1(),
                                               curr->pressed_mouse2());
            draw_marks(curr);
            if (is_press)
                glColorYellow();
            else if (is_release)
                glColorGray();
            else
                continue;
            textures::vertex->draw(curr->p, vertex_size * 0.5f, vertex_size);
        }
    }
    {
        constexpr float bar_top_y = -0.95f;
        constexpr float bar_height = 0.05f;
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        bool is_drawing_m1 = false;
        bool is_drawing_m2 = false;
        float m1_pos = -1.f;
        float m2_pos = -1.f;
        for (auto curr = start; curr != end; ++curr) {
            const float xpos = 2.f * RATIO(curr->ms - ms, ui::trail_length) + 1.f;
            if (curr->pressed_mouse1()) {
                if (!is_drawing_m1) {
                    m1_pos = xpos;
                    is_drawing_m1 = true;
                }
            } else {
                if (is_drawing_m1) {
                    glColorCyan();
                    quad_vertex(glm::vec2(m1_pos, bar_top_y), glm::vec2(xpos - m1_pos, bar_height));
                    is_drawing_m1 = false;
                }
            }
            if (curr->pressed_mouse2()) {
                if (!is_drawing_m2) {
                    m2_pos = xpos;
                    is_drawing_m2 = true;
                }
            } else {
                if (is_drawing_m2) {
                    glColorMagenta();
                    quad_vertex(glm::vec2(m2_pos, bar_top_y - bar_height), glm::vec2(xpos - m2_pos, bar_height));
                    is_drawing_m2 = false;
                }
            }
            if (is_drawing_m1) {
                glColorCyan();
                quad_vertex(glm::vec2(m1_pos, bar_top_y), glm::vec2(xpos - m1_pos, bar_height));
            }
            if (is_drawing_m2) {
                glColorMagenta();
                quad_vertex(glm::vec2(m2_pos, bar_top_y - bar_height), glm::vec2(xpos - m2_pos, bar_height));
            }
        }
        glEnd();
        glBegin(GL_LINES);
        bool prev_mouse1 = false;
        bool prev_mouse2 = false;
        for (auto curr = start; curr != end; ++curr) {
            const bool is_press = IS_PRESSED(prev_mouse1, prev_mouse2, curr->pressed_mouse1(), curr->pressed_mouse2());
            const bool is_release =
                IS_RELEASE(prev_mouse1, prev_mouse2, curr->pressed_mouse1(), curr->pressed_mouse2());
            prev_mouse1 = curr->pressed_mouse1();
            prev_mouse2 = curr->pressed_mouse2();

            const float xpos = 2.f * RATIO(curr->ms - ms, ui::trail_length) + 1.f;

            if (!is_press && !is_release) {
                if (draw_frames_on_timeline) {
                    glColorWhite();
                    glVertex2f(xpos, bar_top_y - bar_height / 4);
                    glVertex2f(xpos, bar_top_y + bar_height / 4);
                }
                continue;
            }
            
            if (is_press) {
                glColorYellow();
            } else {
                glColorGray();
            }
            glVertex2f(xpos, bar_top_y - bar_height);
            glVertex2f(xpos, bar_top_y + bar_height);
        }
        glEnd();

        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void ui::draw_frame_lines(SongTime_t ms)
{
    using namespace replayengine;

    const std::vector<ReplayFrame> &frames = CurrentView()->frames();
    auto start = std::lower_bound(frames.begin(), frames.end(), ms - ui::trail_length, CmpMs<ReplayFrame>());
    auto end = std::lower_bound(frames.begin(), frames.end(), ms, CmpMs<ReplayFrame>());
    if (start == frames.end()) return;
    if (end != frames.end()) ++end;

    
    constexpr float bar_top_y = -0.95f;
    constexpr float bar_height = 0.05f;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);

    glEnd();
    glBegin(GL_LINES);
    bool prev_mouse1 = false;
    bool prev_mouse2 = false;
    for (auto curr = start; curr != end; ++curr) {
        const bool is_press = IS_PRESSED(prev_mouse1, prev_mouse2, curr->pressed_mouse1(), curr->pressed_mouse2());
        const bool is_release =
            IS_RELEASE(prev_mouse1, prev_mouse2, curr->pressed_mouse1(), curr->pressed_mouse2());
        prev_mouse1 = curr->pressed_mouse1();
        prev_mouse2 = curr->pressed_mouse2();

        const float xpos = 2.f * RATIO(curr->ms - ms, ui::trail_length) + 1.f;

        if (!is_press && !is_release) {
            if (draw_frames_on_timeline) {
                glColorWhite();
                glVertex2f(xpos, bar_top_y - bar_height / 4);
                glVertex2f(xpos, bar_top_y + bar_height / 4);
            }
            continue;
        }
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void ui::draw(SongTime_t ms)
{
    draw_frames(ms, draw_frames_on_timeline);
    tool::current_tool->Draw();
}

static void do_mouse_pan(const glm::vec2 &p)
{
    glm::vec2 mouse = p;
    zoom_pan.osu_pixel_to_gl(mouse);
    glm::vec2 diff = pan_mouse_start - mouse;
    zoom_pan.gl_to_osu_pixel_vec(diff);
    zoom_pan.mut_pan() = pan_start + diff;
    zoom_pan.set_dirty();
}

void ui::mouse_down_right(const glm::vec2 &p)
{
    glm::vec2 mouse = p;
    zoom_pan.osu_pixel_to_gl(mouse);
    pan_mouse_start = mouse;
    pan_start = zoom_pan.pan();
    is_panning = true;
}

void ui::mouse_up_right(const glm::vec2 &p)
{
    do_mouse_pan(p);
    is_panning = false;
}

void ui::mouse_wheel(const glm::vec2 &p, bool is_up)
{
    constexpr float amt = 5.0;
    zoom_pan.mut_zoom() += is_up ? amt : -amt;
    zoom_pan.set_dirty();
}
