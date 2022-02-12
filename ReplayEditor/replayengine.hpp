#pragma once

#include "stdafx.h"
#include "md5.hpp"
#include "thirdparty/glm/glm.hpp"

template<typename T>
class CmpMs {
public:
	bool operator()(const T &lhs, const SongTime_t rhs) const {
		return lhs.ms < rhs;
	}
};

namespace replayengine {
	struct replayframe_t {
		glm::vec2 p;
		SongTime_t ms;
		int keys;
		bool operator< (const replayframe_t &rhs) const {
			return ms < rhs.ms;
		}
		bool pressed_mouse1() const {
			return keys & 1;
		}
		bool pressed_mouse2() const {
			return keys & 2;
		}
		bool pressed_key1() const {
			return keys & 4;
		}
		bool pressed_key2() const {
			return keys & 8;
		}
		bool pressed_smoke() const {
			return keys & 16;
		}
	};
	bool init(const std::wstring &);
	bool write_to(const std::wstring &);
	void invert_replay_frames();
	extern uint8_t game_mode;
	extern uint32_t version;
	extern md5_t beatmap_hash;
	extern std::string player_name;
	extern md5_t replay_hash;
	extern uint16_t num_300;
	extern uint16_t num_100;
	extern uint16_t num_50;
	extern uint16_t num_geki;
	extern uint16_t num_katu;
	extern uint16_t num_miss;
	extern uint32_t total_score;
	extern uint16_t max_combo;
	extern bool full_combo;
	extern uint32_t mods;
	extern std::string lifebar_graph;
	extern U64 timestamp;
	extern U64 online_score_id;
	void draw(SongTime_t);
	extern std::vector<replayframe_t> frames;
	extern I64 current_frame_index;
};
