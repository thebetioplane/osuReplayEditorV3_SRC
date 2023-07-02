// clang-format off
#include "stdafx.h"
// clang-format on

#include "tools.hpp"

#include "audioengine.hpp"
#include "ui.hpp"

static tool::WhichTool which_tool = tool::WhichTool::Sel;
tool::ToolCallback tool::apply = tool::Sel_apply;
tool::ToolCallback tool::draw = tool::Sel_draw;
float tool::brush_radius = 60.f;

tool::WhichTool tool::current_tool()
{
    return which_tool;
}

void tool::current_tool(tool::WhichTool new_tool)
{
    const tool::WhichTool old_tool = which_tool;
    if (old_tool != new_tool) {
        which_tool = new_tool;
        switch (new_tool) {
            case tool::WhichTool::Sel:
                tool::apply = tool::Sel_apply;
                tool::draw = tool::Sel_draw;
                break;
            case tool::WhichTool::Grab:
                tool::apply = tool::Grab_apply;
                tool::draw = tool::Grab_draw;
                break;
            case tool::WhichTool::Brush:
                tool::apply = tool::Brush_apply;
                tool::draw = tool::Brush_draw;
                break;
        }
    }
}

void tool::ToolState::copy_frames_into_buf()
{
    if (curr_frame_buf_in == ui::mark_in && curr_frame_buf_mid == ui::mark_mid && curr_frame_buf_out == ui::mark_out) {
        return;
    }
    curr_frame_buf_in = ui::mark_in;
    curr_frame_buf_mid = ui::mark_mid;
    curr_frame_buf_out = ui::mark_out;
    frame_buf.resize(ui::mark_out - ui::mark_in + 1);
    for (I64 i = ui::mark_in, k = 0; i <= ui::mark_out; ++i, ++k) {
        frame_buf[k] = replayengine::frames[i];
    }
}

static bool is_between(const float v, const float a, const float b)
{
    return (v >= a && v <= b) || (v >= b && v <= a);
}

void tool::Sel_apply(ToolState& s)
{
    using namespace replayengine;
    const SongTime_t ms = audioengine::get_time();
    auto start = std::lower_bound(frames.begin(), frames.end(), ms - ui::trail_length, CmpMs<replayframe_t>());
    auto end = std::lower_bound(frames.begin(), frames.end(), ms, CmpMs<replayframe_t>());
    if (start == frames.end()) return;
    if (end != frames.end()) ++end;
    I64 new_mark_in = -1;
    I64 new_mark_out = -1;
    for (auto iter = start; iter != end; ++iter) {
        if (is_between(iter->p.x, s.v0.x, s.v1.x) && is_between(iter->p.y, s.v0.y, s.v1.y)) {
            if (new_mark_in == -1) {
                new_mark_in = iter - frames.begin();
            }
            new_mark_out = iter - frames.begin();
        }
    }
    if (new_mark_in == -1 || new_mark_out == -1) return;
    ui::mark_in = new_mark_in;
    ui::mark_out = new_mark_out;
    ui::mark_mid = new_mark_in + (new_mark_out - new_mark_in) / 2;
}

void tool::Sel_draw(ToolState& s)
{
    if (!s.enabled) return;
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_LOOP);
    glColorBlue();
    glVertex2f(s.v0.x, s.v0.y);
    glVertex2f(s.v1.x, s.v0.y);
    glVertex2f(s.v1.x, s.v1.y);
    glVertex2f(s.v0.x, s.v1.y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

static void do_weight(const tool::ToolState& s, const glm::vec2& a, const glm::vec2& b, const I64 start, const I64 end,
                      const I64 k_start, const bool rev)
{
    float d = 0.f;
    for (I64 i = start, k = k_start; i < end; ++i, ++k) {
        const glm::vec2& p0 = s.frame_buf[k - 1].p;
        const glm::vec2& p1 = s.frame_buf[k].p;
        d += glm::distance(p1, p0);
    }
    const glm::vec2 dir = s.v1 - b;
    float d0 = 0.f;
    for (I64 i = start, k = k_start; i < end; ++i, ++k) {
        const glm::vec2& p0 = s.frame_buf[k - 1].p;
        const glm::vec2& p = s.frame_buf[k].p;
        d0 += glm::distance(p, p0);
        const float r = rev ? ((d - d0) / d) : (d0 / d);
        const glm::vec2 pp = p + dir * r;
        replayengine::frames[i].p = pp;
    }
}

void tool::Grab_apply(ToolState& s)
{
    if (!ui::are_marks_consistent()) {
        return;
    }
    s.copy_frames_into_buf();
    const glm::vec2& a = s.frame_buf.front().p;
    const glm::vec2& b = s.frame_buf[ui::mark_mid - ui::mark_in].p;
    const glm::vec2& c = s.frame_buf.back().p;
    replayengine::frames[ui::mark_mid].p = s.v1;
    do_weight(s, a, b, ui::mark_in + 1, ui::mark_mid, 1, false);
    do_weight(s, c, b, ui::mark_mid + 1, ui::mark_out, ui::mark_mid - ui::mark_in + 1, true);
}

void tool::Grab_draw(ToolState& s)
{
    if (!s.enabled) return;
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_LOOP);
    glColorBlue();
    glVertex2f(s.v0.x, s.v0.y);
    glVertex2f(s.v1.x, s.v1.y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void tool::Brush_apply(ToolState& s)
{
    if (!ui::are_marks_consistent()) {
        return;
    }
    for (I64 i = ui::mark_in; i <= ui::mark_out; ++i) {
        glm::vec2& p = replayengine::frames[i].p;
        const float d = glm::distance(p, s.v1);
        if (d <= brush_radius) {
            const float m = 1.f - d / brush_radius;
            p += (s.v1 - s.v0) * m;
        }
    }
    s.v0 = s.v1;
}

void tool::Brush_draw(ToolState& s)
{
    constexpr int N = 9;
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_LOOP);
    glColorBlue();
    constexpr float two_pi = 6.28318f;
    for (int i = 0; i < N; ++i) {
        const float angle = RATIO(i, N) * two_pi;
        glVertex2f(s.v1.x + glm::cos(angle) * brush_radius, s.v1.y + glm::sin(angle) * brush_radius);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}