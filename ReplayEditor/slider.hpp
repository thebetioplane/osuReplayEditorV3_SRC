#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <utility>

#include "thirdparty/glm/glm.hpp"

struct pos_time_t {
    glm::vec2 pos;
    SongTime_t time;
};

class slider_t
{
   private:
    std::vector<pos_time_t> curve;
    slider_t(int, float, float);
    void s_linear(std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end);
    void s_catmull(const std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end);
    void s_bezier(const std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end);
    void s_circle(const std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end);

   public:
    const int repeat;
    float pixel_length;
    const float velocity;
    float duration() const
    {
        return pixel_length / velocity;
    }
    glm::vec2 end_pos() const;
    static slider_t *from_def(const char *def, glm::vec2 pos, SongTime_t start, SongTime_t end);
    void draw(SongTime_t ms, SongTime_t start, SongTime_t end, float opacity);
    void translate_offset(const glm::vec2 &offset);
    bool ball_position_at_time(SongTime_t ms, SongTime_t start, SongTime_t end, glm::vec2 &out_pos) const;
    bool position_at_time(SongTime_t ms, SongTime_t start, SongTime_t end, glm::vec2 &out_pos) const;
};
