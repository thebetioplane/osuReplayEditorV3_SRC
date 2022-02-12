#pragma once

#include <string>

namespace config {
		// changing this order breaks existing dbs
	enum class Key {
		UpdateTimeStampOnExit,
		Volume,
		CursorMode
	};
	extern std::wstring osu_db_path;
	extern std::wstring song_path;
	void init();
	void cleanup();
	int get_int(Key);
	void set_int(Key, int);
}