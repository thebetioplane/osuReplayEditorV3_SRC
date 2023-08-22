#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <string>

/*
This osu!.db only keeps track of beatmaps,
nothing else is going to be implemented
*/

namespace osudb
{

bool init();
bool get_entry(const std::string &hash, std::wstring &osu_path, std::wstring &song_path);
bool get_hash(const std::wstring &osu_path, std::string &hash);

}  // namespace osudb
