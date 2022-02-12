#pragma once

#include "stdafx.h"
#include "md5.hpp"

/*
This osu!.db only keeps track of beatmaps,
nothing else is going to be implemented
*/

namespace osudb {
	bool init();
	bool get_entry(const md5_t &hash, std::wstring &osu_path, std::wstring &song_path);
}
