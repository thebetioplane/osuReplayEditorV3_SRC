// clang-format off
#include "stdafx.h"
// clang-format on

#include "zoom_pan.hpp"

#include "thirdparty/glm/glm.hpp"
#include "thirdparty/glm/gtc/matrix_transform.hpp"

ZoomPan zoom_pan;

namespace
{

constexpr float ZOOM_MIN = -200.f;
constexpr float ZOOM_MAX = 90.f;

}  // namespace

void ZoomPan::gl_to_osu_pixel(glm::vec2 &v) const
{
    const float A = 2.f / (m_right - m_left);
    const float B = 2.f / (m_top - m_bottom);
    const float C = (m_right + m_left) / (m_left - m_right);
    const float D = (m_top + m_bottom) / (m_bottom - m_top);
    v.x = (v.x - C) / A;
    v.y = (v.y - D) / B;
}

void ZoomPan::gl_to_osu_pixel_vec(glm::vec2 &v) const
{
    const float A = 2.f / (m_right - m_left);
    const float B = 2.f / (m_top - m_bottom);
    v.x /= A;
    v.y /= B;
}

void ZoomPan::osu_pixel_to_gl(glm::vec2 &v) const
{
    const float A = 2.f / (m_right - m_left);
    const float B = 2.f / (m_top - m_bottom);
    const float C = (m_right + m_left) / (m_left - m_right);
    const float D = (m_top + m_bottom) / (m_bottom - m_top);
    v.x = A * v.x + C;
    v.y = B * v.y + D;
}

void ZoomPan::set_dirty()
{
    if (m_zoom < ZOOM_MIN) {
        m_zoom = ZOOM_MIN;
    }
    if (m_zoom > ZOOM_MAX) {
        m_zoom = ZOOM_MAX;
    }
    const glm::vec2 zoom2(m_zoom * 4.f, m_zoom * 3.f);
    m_left = m_pan.x - 128.f + zoom2.x;
    m_right = m_pan.x + 640.f - zoom2.x;
    m_bottom = m_pan.y + 480.f - zoom2.y;
    m_top = m_pan.y - 96.f + zoom2.y;
    m_is_dirty = true;
}

void ZoomPan::reset()
{
    m_pan = glm::vec2(0.f, 0.f);
    m_zoom = 0.0;
    set_dirty();
}

void ZoomPan::set_projection()
{
    if (m_is_dirty) {
        m_is_dirty = false;
        m_projection = glm::scale(glm::mat4(1.f), glm::vec3(m_stretch, 1.f)) *
                       glm::ortho(m_left, m_right, m_bottom, m_top, 0.f, 2.f);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&m_projection[0][0]);
    }
}

void ZoomPan::on_resize(int width, int height)
{
    if (width < 5 || height < 5) return;
    m_stage_width = width;
    m_stage_height = height;
    m_playfield.x = 0;
    m_playfield.y = 0;
    m_playfield.w = height * 4 / 3;
    m_playfield.h = width * 3 / 4;
    if (m_playfield.w > width) {
        m_playfield.w = width;
        m_playfield.h = m_playfield.w * 3 / 4;
        m_playfield.y = (height - m_playfield.h) / 2;
    } else {
        m_playfield.h = height;
        m_playfield.w = m_playfield.h * 4 / 3;
        m_playfield.x = (width - m_playfield.w) / 2;
    }
    m_stretch.x = RATIO(m_playfield.w, width);
    m_stretch.y = RATIO(m_playfield.h, height);
    glViewport(0, 0, width, height);
    set_dirty();
}