#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include "thirdparty/glm/glm.hpp"

class ZoomPan
{
   public:
    struct Rect {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };

    void gl_to_osu_pixel(glm::vec2 &v) const;
    void osu_pixel_to_gl(glm::vec2 &v) const;
    void gl_to_osu_pixel_vec(glm::vec2 &v) const;
    void reset();
    void on_resize(int width, int height);
    void set_projection();
    void set_dirty();

    const Rect &playfield() const
    {
        return m_playfield;
    }
    float zoom() const
    {
        return m_zoom;
    }
    float &mut_zoom()
    {
        return m_zoom;
    }
    const glm::vec2 &pan() const
    {
        return m_pan;
    }
    glm::vec2 &mut_pan()
    {
        return m_pan;
    }
    const glm::mat4 &projection()
    {
        return m_projection;
    }

   private:
    bool m_is_dirty = true;
    float m_zoom = 0.f;
    glm::vec2 m_pan{0.f, 0.f};
    glm::vec2 m_stretch{0.f, 0.f};
    int m_stage_height = 0;
    int m_stage_width = 0;
    Rect m_playfield{0, 0, 0, 0};
    float m_left = -1.f;
    float m_right = 1.f;
    float m_bottom = -1.f;
    float m_top = 1.f;
    glm::mat4 m_projection{1.f};
};

extern ZoomPan zoom_pan;
