#pragma once

#include "thirdparty/glm/glm.hpp"

namespace ui {
	void draw(SongTime_t ms);
	void mouse_up(const glm::vec2 &p);
	void mouse_down(const glm::vec2 &p);
	void mouse_move(const glm::vec2 &p);
	void mouse_up_right(const glm::vec2 &p);
	void mouse_down_right(const glm::vec2 &p);
	void mouse_wheel(const glm::vec2 &p, bool is_up);
	enum class TrailMode {
		Off, Raw, Hits, num_trail_modes
	};
	extern TrailMode trail_mode;
	extern I64 mark_in;
	extern I64 mark_out;
	extern I64 mark_mid;
	extern SongTime_t trail_length;
	bool is_in_out_mark_consistent();
	bool are_marks_consistent();
	void make_undo_restore_point();
	bool undo();
	bool redo();
}