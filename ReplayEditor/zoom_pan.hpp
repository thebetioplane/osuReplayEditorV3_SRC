#pragma once

#include "stdafx.h"
#include "thirdparty/glm/glm.hpp"

namespace zoom_pan {
	struct Rect {
		int x, y, w, h;
	};
	extern glm::vec2 pan;
	extern float zoom;
	void on_resize(int width, int height);
	extern Rect playfield;
	extern int stage_width;
	extern int stage_height;
	void reset();
	void set_projection();
	void set_dirty();
	void gl_to_osu_pixel(glm::vec2 &v);
	void osu_pixel_to_gl(glm::vec2 &v);
	void gl_to_osu_pixel_vec(glm::vec2 &v);
}