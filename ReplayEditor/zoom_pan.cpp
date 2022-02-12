#include "stdafx.h"
#include "zoom_pan.hpp"

glm::vec2 zoom_pan::pan;
float zoom_pan::zoom;
static bool is_dirty = true;
zoom_pan::Rect zoom_pan::playfield;
static glm::vec2 stretch;
int zoom_pan::stage_width;
int zoom_pan::stage_height;

constexpr float ZOOM_MIN = -200.f;
constexpr float ZOOM_MAX = 90.f;

static float left = -1.;
static float right = 1.;
static float bottom = -1.;
static float top = 1.;

void zoom_pan::gl_to_osu_pixel(glm::vec2 &v)
{
	const float A = 2.f / (right - left);
	const float B = 2.f / (top - bottom);
	const float C = (right + left) / (left - right);
	const float D = (top + bottom) / (bottom - top);
	v.x = (v.x - C) / A;
	v.y = (v.y - D) / B;
}

void zoom_pan::gl_to_osu_pixel_vec(glm::vec2 &v)
{
	const float A = 2.f / (right - left);
	const float B = 2.f / (top - bottom);
	v.x /= A;
	v.y /= B;
}

void zoom_pan::osu_pixel_to_gl(glm::vec2 &v)
{
	const float A = 2.f / (right - left);
	const float B = 2.f / (top - bottom);
	const float C = (right + left) / (left - right);
	const float D = (top + bottom) / (bottom - top);
	v.x = A * v.x + C;
	v.y = B * v.y + D;
}

void zoom_pan::set_dirty()
{
	if (zoom < ZOOM_MIN)
		zoom = ZOOM_MIN;
	if (zoom > ZOOM_MAX)
		zoom = ZOOM_MAX;
	const glm::vec2 zoom2(zoom * 4.f, zoom * 3.f);
	left = pan.x - 128.f + zoom2.x;
	right = pan.x + 640.f - zoom2.x;
	bottom = pan.y + 480.f - zoom2.y;
	top = pan.y - 96.f + zoom2.y;
	is_dirty = true;
}

void zoom_pan::reset()
{
	pan = glm::vec2(0.f, 0.f);
	zoom = 0.0;
	set_dirty();
}

void zoom_pan::set_projection()
{
	if (is_dirty)
	{
		is_dirty = false;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glScalef(stretch.x, stretch.y, 1.f);
		glOrtho(left, right, bottom, top, 0., 2.);
	}
}

void zoom_pan::on_resize(int width, int height)
{
	if (width < 5 || height < 5)
		return;
	stage_width = width;
	stage_height = height;
	playfield.x = 0;
	playfield.y = 0;
	playfield.w = height * 4 / 3;
	playfield.h = width * 3 / 4;
	if (playfield.w > width) {
		playfield.w = width;
		playfield.h = playfield.w * 3 / 4;
		playfield.y = (height - playfield.h) / 2;
	}
	else {
		playfield.h = height;
		playfield.w = playfield.h * 4 / 3;
		playfield.x = (width - playfield.w) / 2;
	}
	stretch.x = RATIO(playfield.w, width);
	stretch.y = RATIO(playfield.h, height);
	glViewport(0, 0, width, height);
	set_dirty();
}