// clang-format off
#include "stdafx.h"
// clang-format on

#include "slider.hpp"

#include <algorithm>
#include <memory>
#include <utility>

#include "beatmapengine.hpp"
#include "texture.hpp"

#define IS_ODD(n) ((n)&1)
#define IS_EVEN(n) (!IS_ODD(n))

namespace
{

constexpr float PI = 3.14159f;
constexpr float TWOPI = 2.0f * PI;
constexpr float HALFPI = 0.5f * PI;
constexpr float max_target_dist = 1e6f;

struct PosDist {
    const int count;
    std::unique_ptr<glm::vec2[]> pos;
    std::unique_ptr<float[]> dist;
};

float det2(const float a, const float b, const float c, const float d)
{
    return a * d - b * c;
}

float det3(const float a, const float b, const float c, const float d, const float e, const float f, const float g,
           const float h, const float i)
{
    const float d0 = a * det2(e, f, h, i);
    const float d1 = b * det2(d, f, g, i);
    const float d2 = c * det2(d, e, g, h);
    return d0 - d1 + d2;
}

float area2d(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{
    return a.x * (b.y - c.y) - b.x * (a.y - c.y) + c.x * (a.y - b.y);
}

float vec_atan(const glm::vec2 &v)
{
    return std::atan2f(v.y, v.x);
}

glm::vec2 vec_pol(const float angle)
{
    return glm::vec2(std::cos(angle), std::sin(angle));
}

template <typename T>
constexpr T min3(const T &a, const T &b, const T &c)
{
    return glm::min(a, glm::min(b, c));
}

// ccw is with respect to the drawing coords, not the geometric coords
// as such they are "backwards" when using a normal coord system
bool is_ccw(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{
    return area2d(a, b, c) < 0.f;
}

bool is_collinear(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{
    return glm::abs(area2d(a, b, c)) < .001f;
}

std::ostream &operator<<(std::ostream &o, const glm::vec2 &vec)
{
    o << '(' << vec.x << ", " << vec.y << ')';
    return o;
}

glm::vec2 invert_vec(const glm::vec2 &v)
{
    return glm::vec2(v.x, 384.0 - v.y);
}

float catmull_rom(const float v0, const float v1, const float v2, const float v3, float t)
{
    const float t_sq = t * t;
    const float t_cubed = t * t_sq;
    return 0.5f * (2.f * v1 + (-v0 + v2) * t + (2.f * v0 - 5.f * v1 + 4.f * v2 - v3) * t_sq +
                   (-v0 + 3.f * v1 - 3.f * v2 + v3) * t_cubed);
}

void bezier_subdivide(glm::vec2 *const bb, const int deg, const int size, const int step)
{
    const int halfstep = step >> 1;
    const int bigstep = step * deg;
    for (int i = 0; i < size; i += bigstep) {
        for (int level = 0; level < deg; ++level) {
            int h = i + level * halfstep;
            for (int k = level; k < deg; ++k) {
                const int h1 = h + step;
                const int h2 = h + halfstep;
                bb[h2] = glm::mix(bb[h], bb[h1], 0.5f);
                h = h1;
            }
        }
    }
}

PosDist bezier_eval(const std::vector<glm::vec2> &pts, const int begin, const int end)
{
    constexpr int NUM_LEVELS = 4;
    int step = 1 << NUM_LEVELS;
    const int deg = end - begin - 1;
    const int size = step * deg;
    auto bb = std::make_unique<glm::vec2[]>(size + 1);
    for (int i = 0, j = begin; i <= size; i += step, ++j) {
        bb[i] = pts[j];
    }
    for (int i = 0; i < NUM_LEVELS; ++i) {
        bezier_subdivide(bb.get(), deg, size, step);
        step /= 2;
    }
    auto dist = std::make_unique<float[]>(size + 1);
    dist[0] = 0.f;
    for (int i = 1; i <= size; ++i) {
        dist[i] = dist[i - 1] + glm::distance(bb[i - 1], bb[i]);
    }
    return {size + 1, std::move(bb), std::move(dist)};
}

// The point of this is for ill-behaved sliders that have are HUGE but most of the points are far off the screen. If the
// user pans too far they will see the slider disconnect, but this is better than dropping frames. When culling we
// always keep the first and last points to ensure the culled curve has at least two points.
void cull_curve(std::vector<pos_time_t> &curve)
{
    if (curve.size() < 2) {
        return;
    }
    std::vector<pos_time_t> culled_curve;
    culled_curve.emplace_back(curve.front());
    constexpr float cull_distance = 1920.f;
    for (size_t i = 1; i < curve.size() - 1; ++i) {
        if (curve[i].pos.x > -cull_distance && curve[i].pos.x < cull_distance && curve[i].pos.y > -cull_distance &&
            curve[i].pos.y < cull_distance) {
            culled_curve.emplace_back(curve[i]);
        }
    }
    culled_curve.emplace_back(curve.back());
    curve = std::move(culled_curve);
}

}  // namespace

slider_t::slider_t(int _repeat, float _pixel_length, float _velocity)
    : repeat(_repeat), pixel_length(_pixel_length), velocity(_velocity)
{
}

void slider_t::s_linear(std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end)
{
    I64 to = pts.size() - 1;
    for (unsigned i = 1; i < to; ++i) {
        pts.insert(pts.begin() + i, pts[i]);
        ++i;
        ++to;
    }
    s_bezier(pts, start, end);
}

void slider_t::s_catmull(const std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end)
{
    constexpr int DETAIL = 50;
    const int N = static_cast<int>(pts.size());
    const int eval_count = (N - 1) * (DETAIL + 1);
    PosDist eval{eval_count, std::make_unique<glm::vec2[]>(eval_count), std::make_unique<float[]>(eval_count)};
    for (int i = 0; i < N - 1; ++i) {
        const glm::vec2 v0 = i > 0 ? pts[i - 1] : pts[i];
        const glm::vec2 v1 = pts[i];
        const glm::vec2 v2 = (i < N - 1) ? pts[i + 1] : v2 + v2 - v1;
        const glm::vec2 v3 = (i < N - 2) ? pts[i + 2] : v3 + v3 - v2;
        for (int k = 0; k <= DETAIL; ++k) {
            glm::vec2 &p = eval.pos[i * (DETAIL + 1) + k];
            const float t = RATIO(k, DETAIL);
            p.x = catmull_rom(v0.x, v1.x, v2.x, v3.x, t);
            p.y = catmull_rom(v0.y, v1.y, v2.y, v3.y, t);
        }
    }
    eval.dist[0] = 0.f;
    for (int i = 1; i < eval_count; ++i) {
        eval.dist[i] = eval.dist[i - 1] + glm::distance(eval.pos[i], eval.pos[i - 1]);
    }
    curve.push_back({pts[0], start});
    // TODO: Quote from osu!wiki: "If the slider's length is longer than the defined curve, the slider will extend until
    // it reaches the target length. For bezier, catmull, and linear curves, it continues in a straight line from the
    // end of the curve."
    const float target_dist = min3(max_target_dist, eval.dist[eval.count - 1], pixel_length);
    for (float target = 0.0f; target <= target_dist; target += 7.0f) {
        const float *const ptr = std::lower_bound(eval.dist.get(), eval.dist.get() + eval.count, target);
        const int index = static_cast<int>(ptr - eval.dist.get());
        if (index >= eval.count || index <= 0) continue;
        const float t = (target - eval.dist[index - 1]) / (eval.dist[index] - eval.dist[index - 1]);
        const glm::vec2 curr = glm::mix(eval.pos[index - 1], eval.pos[index], t);
        curve.push_back({curr, start + static_cast<SongTime_t>(target / velocity)});
    }
    // TODO: If the pixel_length was used correctly, these would be the same already, but it is not used correctly.
    pixel_length = target_dist;
}

void slider_t::s_bezier(const std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end)
{
    unsigned last_section = 0;
    bool last = false;
    float plength = pixel_length;
    float curve_start_dist = 0.f;
    float measured_length = 0.f;
    for (unsigned i = 1; !last; ++i) {
        last = i == pts.size() - 1;
        if (pts[i] != pts[i - 1] && !last) {
            continue;
        }
        int from = last_section;
        int to = i;
        if (last) {
            ++to;
        }
        const PosDist eval = bezier_eval(pts, from, to);
        curve.push_back({pts[0], start});
        // TODO: Quote from osu!wiki: "If the slider's length is longer than the defined curve, the slider will extend
        // until it reaches the target length. For bezier, catmull, and linear curves, it continues in a straight line
        // from the end of the curve."
        const float target_dist = min3(max_target_dist, eval.dist[eval.count - 1], plength);
        measured_length += target_dist;
        float target = 0.0f;
        for (; target <= target_dist; target += 1.0f) {
            const float *const ptr = std::lower_bound(eval.dist.get(), eval.dist.get() + eval.count, target);
            const I64 index = ptr - eval.dist.get();
            if (index >= eval.count || index <= 0) continue;
            const float t = (target - eval.dist[index - 1]) / (eval.dist[index] - eval.dist[index - 1]);
            const glm::vec2 curr = glm::mix(eval.pos[index - 1], eval.pos[index], t);
            curve.push_back({curr, start + static_cast<SongTime_t>((target + curve_start_dist) / velocity)});
        }
        plength -= target;
        curve_start_dist += target;
        last_section = i;
    }
    // TODO: If the pixel_length was used correctly, these would be the same already, but it is not used correctly.
    pixel_length = measured_length;
}

void slider_t::s_circle(const std::vector<glm::vec2> &pts, SongTime_t start, SongTime_t end)
{
    const float x0 = pts[0].x, x1 = pts[1].x, x2 = pts[2].x;
    const float y0 = pts[0].y, y1 = pts[1].y, y2 = pts[2].y;
    const float z0 = x0 * x0 + y0 * y0, z1 = x1 * x1 + y1 * y1, z2 = x2 * x2 + y2 * y2;
    // form is (x^2 + y^2) + Bx + Cy + D = 0
    const float A = det3(x0, y0, 1, x1, y1, 1, x2, y2, 1);
    const float B = -det3(z0, y0, 1, z1, y1, 1, z2, y2, 1) / A;
    const float C = det3(z0, x0, 1, z1, x1, 1, z2, x2, 1) / A;
    const float D = -det3(z0, x0, y0, z1, x1, y1, z2, x2, y2) / A;
    const glm::vec2 center(B * -0.5f, C * -0.5f);
    const float radius = glm::distance(pts[0], center);
    const float from = vec_atan(pts[0] - center);
    float step = TWOPI / radius;
    if (is_ccw(pts[0], pts[1], pts[2])) step = -step;
    glm::vec2 prev = pts[0];
    int num_segments = 0;
    float distance_so_far = 0.f;
    for (float f = from; glm::distance(prev, pts[2]) > 1.0f || num_segments < 2; f += step) {
        glm::vec2 curr = center + vec_pol(f) * radius;
        distance_so_far += glm::distance(curr, prev);
        curve.push_back({curr, start + static_cast<SongTime_t>(distance_so_far / velocity)});
        prev = curr;
        ++num_segments;
        if (distance_so_far >= pixel_length) break;
    }
}

slider_t *slider_t::from_def(const char *def, glm::vec2 pos, SongTime_t start, SongTime_t end)
{
    std::vector<glm::vec2> pts;
    pts.push_back(pos);
    const char slider_type = def[0];
    ++def;
    for (;;) {
        glm::vec2 pt;
        if (sscanf_s(def, "|%f:%f", &pt.x, &pt.y) != 2) break;
        pts.push_back(pt);
        ++def;
        while (def[0] && def[0] != '|' && def[0] != ',') {
            ++def;
        }
    }
    if (pts.size() <= 1) fatal("slider not enough points defined");
    int repeat;
    float pixel_length;
    if (sscanf_s(def, ",%d,%f", &repeat, &pixel_length) != 2) fatal("bad slider def");
    const float velocity = beatmapengine::slider_mult * beatmapengine::slider_velocity_at(start);
    slider_t *obj = new slider_t(repeat, pixel_length, velocity);
    constexpr float p_dist_thresh = 0.01f;

    //obj->curve.emplace_back(pos, start);
    switch (slider_type) {
        case 'L': {
            obj->s_linear(pts, start, end);
            break;
        }
        case 'B':
            obj->s_bezier(pts, start, end);
            break;
        case 'P':
            if (pts.size() != 3 || is_collinear(pts[0], pts[1], pts[2]) ||
                glm::distance(pts[0], pts[1]) < p_dist_thresh || glm::distance(pts[1], pts[2]) < p_dist_thresh ||
                glm::distance(pts[2], pts[0]) < p_dist_thresh)
                obj->s_bezier(pts, start, end);
            else
                obj->s_circle(pts, start, end);
            break;
        case 'C':
            if (pts.size() == 2)
                obj->s_bezier(pts, start, end);
            else
                obj->s_catmull(pts, start, end);
            break;
        default:
            fatal("bad slider type");
    }
    cull_curve(obj->curve);
    if (obj->curve.empty()) {
        fatal("Slider curve was empty");
    }
    if (obj->curve.size() == 1) {
        pos_time_t fake_end = obj->curve.front();
        fake_end.pos += glm::vec2(0.01f, 0.01f);
        fake_end.time += 1;
        obj->curve.emplace_back(fake_end);
    }
    std::stable_sort(obj->curve.begin(), obj->curve.end(),
                     [](const pos_time_t &lhs, const pos_time_t &rhs) { return lhs.time < rhs.time; });
    return obj;
}

glm::vec2 slider_t::end_pos() const
{
    return curve[curve.size() - 1].pos;
}

SongTime_t last_tick_offset = 36;

void slider_t::draw(SongTime_t ms, SongTime_t start, SongTime_t end, float opacity, bool draw_true_slider_end)
{
    const glm::vec2 size = glm::vec2(beatmapengine::circleradius() * 2.0f);
    glColor4f(opacity, opacity, opacity, 1.0);
    bool draw_slider_ball = false;
    glm::vec2 slider_ball_pos{0.f, 0.f};
    draw_slider_ball = ball_position_at_time(ms, start, end, slider_ball_pos);

    SongTime_t true_slider_end_time = std::max(static_cast<float>(end - last_tick_offset), end - duration() / 2);
    glm::vec2 true_slider_end_pos{0.f, 0.f};
    auto true_slider_end_size = size * 2.4f;

    if (draw_true_slider_end) ball_position_at_time(true_slider_end_time, start, end, true_slider_end_pos);

    if (beatmapengine::hitobjects_inverted) {
        for (const auto &item : curve) {
            textures::slideredge->draw(invert_vec(item.pos), size * 0.5f, size);
        }
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        for (const auto &item : curve) {
            textures::sliderbody->draw(invert_vec(item.pos), size * 0.5f, size);
        }
        if (draw_true_slider_end) {
            glColor4f(0.0f, 1.0f, 1.0f, 0.5f * opacity);
            textures::slideredge->draw(invert_vec(true_slider_end_pos), true_slider_end_size * 0.5f, true_slider_end_size);
        }
        if (draw_slider_ball) {
            glColor4f(0.552941f, 0.752941f, 0.513725f, 1.f);
            textures::slideredge->draw(invert_vec(slider_ball_pos), size * 0.5f, size);
        }
    } else {
        for (const auto &item : curve) {
            textures::slideredge->draw(item.pos, size * 0.5f, size);
        }
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        for (const auto &item : curve) {
            textures::sliderbody->draw(item.pos, size * 0.5f, size);
        }
        if (draw_true_slider_end) {
            glColor4f(0.0f, 1.0f, 1.0f, 0.5f * opacity);
            textures::slideredge->draw(true_slider_end_pos, true_slider_end_size * 0.5f, true_slider_end_size);
        }
        if (draw_slider_ball) {
            glColor4f(0.552941f, 0.752941f, 0.513725f, 1.f);
            textures::slideredge->draw(slider_ball_pos, size * 0.5f, size);
        } 
    }
}

void slider_t::translate_offset(const glm::vec2 &offset)
{
    for (size_t i = 0; i < curve.size(); ++i) {
        curve[i].pos += offset;
    }
}

bool slider_t::ball_position_at_time(SongTime_t ms, SongTime_t start, SongTime_t end, glm::vec2 &out_pos) const
{
    if (ms >= start && ms <= end) {
        const SongTime_t duration_int = glm::max(1, static_cast<SongTime_t>(duration()));
        SongTime_t ball_ms = (ms - start) % duration_int + start;
        if (IS_ODD((ms - start) / duration_int)) {
            ball_ms = duration_int - (ball_ms - start) + start;
        }
        return position_at_time(ball_ms, start, end, out_pos);
    }
    return false;
}

bool slider_t::position_at_time(SongTime_t ms, SongTime_t start, SongTime_t end, glm::vec2 &out_pos) const
{
    if (ms >= start && ms <= end) {
        auto iter =
            std::upper_bound(curve.begin(), curve.end(), ms,
                             [](const SongTime_t ms, const pos_time_t &pos_time) { return pos_time.time > ms; });
        if (iter != curve.begin() && iter != curve.end()) {
            auto prev_iter = iter - 1;
            const float t = RATIO(ms - prev_iter->time, iter->time - prev_iter->time);
            out_pos = glm::mix(prev_iter->pos, iter->pos, t);
            return true;
        }
    }
    return false;
}

bool slider_t::position_at_time(SongTime_t ms, glm::vec2 &out_pos) const
{
    auto iter =
        std::upper_bound(curve.begin(), curve.end(), ms,
                            [](const SongTime_t ms, const pos_time_t &pos_time) { return pos_time.time > ms; });
    if (iter != curve.begin() && iter != curve.end()) {
        auto prev_iter = iter - 1;
        const float t = RATIO(ms - prev_iter->time, iter->time - prev_iter->time);
        out_pos = glm::mix(prev_iter->pos, iter->pos, t);
        return true;
    }
    return false;
}