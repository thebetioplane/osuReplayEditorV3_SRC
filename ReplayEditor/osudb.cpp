// clang-format off
#include "stdafx.h"
// clang-format on

#include "osudb.hpp"

#include <cstring>
#include <map>

#include "breader.hpp"
#include "config.hpp"

#define CHECK_IF_READER_CLOSED(reader) \
    do {                               \
        if ((reader).is_closed()) {    \
            return false;              \
        }                              \
    } while (0);

namespace osudb
{
namespace
{

struct osudb_entry_t {
    std::wstring osu_path;
    std::wstring song_path;
};

std::map<std::string, osudb_entry_t> beatmaps;

}  // namespace

bool init()
{
    breader_t r(config::osu_db_path);
    if (r.is_closed()) {
        fatal("osu!.db not found");
        return false;
    }
    int32_t version;
    r >> version;
    CHECK_IF_READER_CLOSED(r);
    // folder count (4)
    // account lock (1)
    // account unlock date (8)
    r += 13;
    CHECK_IF_READER_CLOSED(r);
    // playername (str)
    r.dummy_string();
    CHECK_IF_READER_CLOSED(r);
    int32_t num_beatmaps;
    r >> num_beatmaps;
    CHECK_IF_READER_CLOSED(r);
    for (int beatmap_i = 0; beatmap_i < num_beatmaps; ++beatmap_i) {
        // artist and song names (6 str)
        for (int i = 0; i < 6; ++i) {
            r.dummy_string();
            CHECK_IF_READER_CLOSED(r);
        }
        std::string hash;
        osudb_entry_t entry;
        std::string partial_song_path;
        std::string partial_osu_path;
        std::string folder_name;
        r >> partial_song_path;
        CHECK_IF_READER_CLOSED(r);
        r >> hash;
        CHECK_IF_READER_CLOSED(r);
        r >> partial_osu_path;
        CHECK_IF_READER_CLOSED(r);
        // ranked status (1)
        // hitcircle count (2)
        // slider count (2)
        // spinner count (2)
        // last modify (8)
        // ar (4)
        // cs (4)
        // hp (4)
        // od (4)
        // velocity (8)
        r += 39;
        CHECK_IF_READER_CLOSED(r);
        for (int i = 0; i < 4; ++i) {
            int32_t count = 0;
            r >> count;
            CHECK_IF_READER_CLOSED(r);
            for (int j = 0; j < count; ++j) {
                if (version >= 20250108) {
                    // star rating (10)
                    r += 10;
                } else {
                    // star rating (14)
                    r += 14;
                }
                CHECK_IF_READER_CLOSED(r);
            }
        }
        // drain time (4)
        // total time (4)
        // preview time (4)
        r += 12;
        CHECK_IF_READER_CLOSED(r);
        {
            int32_t count = 0;
            r >> count;
            CHECK_IF_READER_CLOSED(r);
            for (int j = 0; j < count; ++j) {
                // timing point (17)
                r += 17;
                CHECK_IF_READER_CLOSED(r);
            }
        }
        // beatmap id (4)
        // beatmap set id (4)
        // thread id (4)
        // standard grade (1)
        // taiko grade (1)
        // ctb grade (1)
        // mania grade (1)
        // local offset (2)
        // stack leniency (4)
        // gameplay mode (1)
        r += 23;
        CHECK_IF_READER_CLOSED(r);
        // song source (str)
        r.dummy_string();
        CHECK_IF_READER_CLOSED(r);
        // song tags (str)
        r.dummy_string();
        CHECK_IF_READER_CLOSED(r);
        // online offset (2)
        r += 2;
        CHECK_IF_READER_CLOSED(r);
        // font name (str)
        r.dummy_string();
        CHECK_IF_READER_CLOSED(r);
        // unplayed (1)
        // last played (8)
        // osz2? (1)
        r += 10;
        CHECK_IF_READER_CLOSED(r);
        r >> folder_name;
        CHECK_IF_READER_CLOSED(r);
        // last repo check (8)
        // ignore beatmap sound (1)
        // ignore beatmap skin (1)
        // disable storyboard (1)
        // disable video (1)
        // visual override (1)
        // some int (4)
        // mania scroll speed (1)
        r += 18;
        CHECK_IF_READER_CLOSED(r);
        entry.osu_path = string_to_wstring(folder_name + '/' + partial_osu_path);
        entry.song_path = string_to_wstring(folder_name + '/' + partial_song_path);
        beatmaps.insert({hash, entry});
    }
    return true;
}

bool get_entry(const std::string &hash, std::wstring &osu_path, std::wstring &song_path)
{
    auto entry = beatmaps.find(hash);
    if (entry == beatmaps.end()) return false;
    osu_path = entry->second.osu_path;
    song_path = entry->second.song_path;
    return true;
}

}  // namespace osudb