// clang-format off
#include "stdafx.h"
// clang-format on

#include "ui.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

#include "HistoryStack.hpp"
#include "audioengine.hpp"
#include "replayengine.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include "zoom_pan.hpp"

#define FLIP_Y(v) (v).y = -(v).y

ui::TrailMode ui::trail_mode = ui::TrailMode::Hits;
SongTime_t ui::trail_length = 1000;

static glm::vec2 pan_start;
static glm::vec2 pan_mouse_start;
static bool is_panning = false;

static HistoryStack<10> undo_history;
static HistoryStack<10> redo_history;
static tool::ToolState tool_state;

I64 ui::mark_in = -1;
I64 ui::mark_out = -1;
I64 ui::mark_mid = -1;

template <typename T>
static I64 iter_index(const T &iter)
{
    return iter - replayengine::frames.begin();
}

static bool is_mark_index_in_range(I64 index)
{
    return index >= 0 && index < static_cast<I64>(replayengine::frames.size());
}

static void do_mouse_pan(const glm::vec2 &p);

void ui::mouse_move(const glm::vec2 &p)
{
    if (is_panning) {
        do_mouse_pan(p);
    }
    if (tool_state.enabled) {
        tool_state.v1.x = p.x;
        tool_state.v1.y = p.y;
        tool::apply(tool_state);
    } else if (tool::current_tool() == tool::WhichTool::Brush) {
        tool_state.v1.x = p.x;
        tool_state.v1.y = p.y;
    }
}

void ui::mouse_down(const glm::vec2 &p)
{
    tool_state.enabled = true;
    tool_state.v0.x = p.x;
    tool_state.v0.y = p.y;
    constexpr float e = 0.01f;
    tool_state.v1.x = p.x + e;
    tool_state.v1.y = p.y + e;
    bool can_grab = false;
    if (ui::are_marks_consistent()) {
        const glm::vec2 &v = replayengine::frames[ui::mark_mid].p;
        constexpr float RADIUS = 35.f;
        if (glm::distance(v, p) <= RADIUS) can_grab = true;
    }
    const tool::WhichTool current_tool = tool::current_tool();
    if (current_tool == tool::WhichTool::Sel || current_tool == tool::WhichTool::Grab) {
        tool::current_tool(can_grab ? tool::WhichTool::Grab : tool::WhichTool::Sel);
    }
    ui::make_undo_restore_point();
}

void ui::mouse_up(const glm::vec2 &p)
{
    tool_state.enabled = false;
    tool_state.v1.x = p.x;
    tool_state.v1.y = p.y;
    tool::apply(tool_state);
}

template <typename T>
static void draw_marks(const T &iter)
{
    const I64 index = iter_index(iter);
    const bool m_in = index == ui::mark_in;
    const bool m_out = index == ui::mark_out;
    const bool m_mid = index == ui::mark_mid;
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

static void draw_frames(const SongTime_t ms)
{
    using namespace replayengine;
    if (ui::trail_mode == ui::TrailMode::Off) return;
    auto start = std::lower_bound(frames.begin(), frames.end(), ms - ui::trail_length, CmpMs<replayframe_t>());
    auto end = std::lower_bound(frames.begin(), frames.end(), ms, CmpMs<replayframe_t>());
    if (start == frames.end()) return;
    if (end != frames.end()) ++end;
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_STRIP);
    glColorRed();
    bool is_color_red = true;
    for (auto iter = start; iter != end; ++iter) {
        const I64 index = iter_index(iter);
        if (index >= ui::mark_in && index <= ui::mark_out) {
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
            if (iter->pressed_mouse1()) {
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
            if (!is_press && !is_release) {
                continue;
            }
            const float xpos = 2.f * RATIO(curr->ms - ms, ui::trail_length) + 1.f;
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

void ui::draw(SongTime_t ms)
{
    draw_frames(ms);
    tool::draw(tool_state);
}

bool ui::are_marks_consistent()
{
    if (!is_mark_index_in_range(mark_in)) return false;
    if (!is_mark_index_in_range(mark_out)) return false;
    if (!is_mark_index_in_range(mark_mid)) return false;
    return mark_in <= mark_mid && mark_mid <= mark_out;
}

bool ui::undo()
{
    redo_history.create_snapshot();
    return undo_history.restore_snapshot();
}

bool ui::redo()
{
    undo_history.create_snapshot();
    return redo_history.restore_snapshot();
}

bool ui::is_in_out_mark_consistent()
{
    if (!is_mark_index_in_range(mark_in)) return false;
    if (!is_mark_index_in_range(mark_out)) return false;
    return mark_in <= mark_out;
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

void ui::make_undo_restore_point()
{
    undo_history.create_snapshot();
}
