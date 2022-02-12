#pragma once

#include "stdafx.h"
#include "thirdparty/glm/glm.hpp"
#include "slider.hpp"

enum class HitObjectType {
	Circle, Slider, Spinner, SliderTick, SliderEnd
};

static inline std::ostream& operator<<(std::ostream& o, const HitObjectType rhs)
{
	switch (rhs)
	{
	case HitObjectType::Circle:
		o << "Circle";
		break;
	case HitObjectType::Slider:
		o << "Slider";
		break;
	case HitObjectType::Spinner:
		o << "Spinner";
			break;
	case HitObjectType::SliderTick:
		o << "SliderTick";
		break;
	case HitObjectType::SliderEnd:
		o << "SliderEnd";
		break;
	default:
		o << "Unknown";
		break;
	}
	return o;
}

class hitobject_t {
public:
	glm::vec2 pos;
	SongTime_t start, end;
	HitObjectType hitobject_type;
	slider_t *slider;
	bool is_miss;
	int hit_error;
	int stack_count;
	// interval tree stuff
	hitobject_t* left;
	hitobject_t* right;
	SongTime_t max_in_subtree;
	bool is_circle_or_slider() const {
		return hitobject_type == HitObjectType::Circle || hitobject_type == HitObjectType::Slider;
	}
	int get_points(const int window50, const int window100, const int window300) const {
		if (is_miss)
			return 0;
		const int e = std::abs(hit_error);
		if (e < window300)
			return 300;
		if (e < window100)
			return 100;
		if (e < window50)
			return 50;
		return 0;
	}
	SongTime_t animation_start() const;
	SongTime_t animation_end() const;
	hitobject_t(const char *) noexcept;
	hitobject_t(const glm::vec2 &my_pos, SongTime_t my_start, SongTime_t my_end, HitObjectType my_hit_object_type) noexcept;
	void destroy();
	void draw_bg(SongTime_t) const;
	void draw_fg(SongTime_t) const;
	bool operator< (const hitobject_t& rhs) const {
		return animation_start() < rhs.animation_start();
	}
	int duration() const {
		return end - start;
	}
	int animation_duration() const {
		return animation_end() - animation_start();
	}
};