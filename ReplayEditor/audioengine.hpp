#pragma once

#include "stdafx.h"

namespace audioengine {
	bool init();
	bool load(const std::wstring &);
	void play();
	void pause();
	void stop();
	void toggle_pause();
	bool is_playing();
	bool is_paused();
	bool is_stopped();
	void jump_to(SongTime_t ms);
	void rel_jump(SongTime_t ms);
	void set_volume(float);
	float get_volume();
	void set_playback_speed(float);
	float get_playback_speed();
	SongTime_t get_time();
}
