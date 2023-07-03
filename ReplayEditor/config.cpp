// clang-format off
#include "stdafx.h"
// clang-format on
#include "config.hpp"

#include <array>
#include <thread>
#include <tuple>

#include "../sqlite/sqlite3.h"

std::wstring config::osu_db_path;
std::wstring config::song_path;

static sqlite3 *db = nullptr;
static sqlite3_stmt *select_stmt = nullptr;
static sqlite3_stmt *update_stmt = nullptr;

class StatementLockette
{
   private:
    sqlite3_stmt *const stmt;

   public:
    StatementLockette(sqlite3_stmt *const p_stmt) : stmt(p_stmt)
    {
    }
    ~StatementLockette()
    {
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
};

static void db_close()
{
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

static void db_free_stmt(sqlite3_stmt *&stmt)
{
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
}

static int db_step_all(sqlite3_stmt *stmt)
{
    for (;;) {
        int r = sqlite3_step(stmt);
        if (r == SQLITE_BUSY)
            std::this_thread::yield();
        else
            return r;
    }
}

void config::init()
{
    constexpr const char *DB_NAME = "ore.db3";
    if (sqlite3_open(DB_NAME, &db)) {
        db_close();
        return;
    }
    constexpr const char *sql_create_table =
        "CREATE TABLE IF NOT EXISTS cfg( "
        "key INTEGER PRIMARY KEY, "
        "value "
        ");";
    int ret;
    ret = sqlite3_exec(db, sql_create_table, nullptr, nullptr, nullptr);
    if (ret) return;
    sqlite3_stmt *stmt = nullptr;
    constexpr const char *sql_insert =
        "INSERT OR IGNORE INTO cfg "
        "(key, value) "
        "VALUES (?1, ?2);";
    ret = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, nullptr);
    if (ret || stmt == nullptr) return;
    using Value = int;
    using Pair = std::tuple<Key, Value>;
    std::array<Pair, 3> default_key_value{
        std::make_tuple(Key::UpdateTimeStampOnExit, 0),
        std::make_tuple(Key::Volume, 2),
        std::make_tuple(Key::CursorMode, 2),
    };
    for (auto kv_pair : default_key_value) {
        StatementLockette l(stmt);
        sqlite3_bind_int(stmt, 1, static_cast<int>(std::get<Key>(kv_pair)));
        sqlite3_bind_int(stmt, 2, std::get<Value>(kv_pair));
        db_step_all(stmt);
    }

    db_free_stmt(stmt);
    constexpr const char *sql_select =
        "SELECT value "
        "FROM cfg "
        "WHERE key = ?1;";
    sqlite3_prepare_v2(db, sql_select, -1, &select_stmt, nullptr);
    constexpr const char *sql_update =
        "UPDATE cfg "
        "SET value = ?2 "
        "WHERE key = ?1;";
    sqlite3_prepare_v2(db, sql_update, -1, &update_stmt, nullptr);
}

void config::cleanup()
{
    db_free_stmt(select_stmt);
    db_free_stmt(update_stmt);
    db_close();
}

int config::get_int(Key k)
{
    constexpr int ERR_VAL = -1;
    if (select_stmt == nullptr) return ERR_VAL;
    StatementLockette l(select_stmt);
    sqlite3_bind_int(select_stmt, 1, static_cast<int>(k));
    const int r = db_step_all(select_stmt);
    if (r != SQLITE_ROW) return ERR_VAL;
    if (sqlite3_column_count(select_stmt) != 1) return ERR_VAL;
    return sqlite3_column_int(select_stmt, 0);
}

void config::set_int(Key k, int v)
{
    if (update_stmt == nullptr) return;
    StatementLockette l(update_stmt);
    sqlite3_bind_int(update_stmt, 1, static_cast<int>(k));
    sqlite3_bind_int(update_stmt, 2, v);
    db_step_all(update_stmt);
}