// clang-format off
#include "stdafx.h"
// clang-format on

#include "config.hpp"

#define CFG_SETUP_SET(key)                        \
    DLLFUN(void) CfgSet##key(INT value)           \
    {                                             \
        config::set_int(config::Key::key, value); \
    }
#define CFG_SETUP_GET(key)                        \
    DLLFUN(INT) CfgGet##key()                     \
    {                                             \
        return config::get_int(config::Key::key); \
    }
#define CFG_SETUP(key) CFG_SETUP_SET(key) CFG_SETUP_GET(key)

CFG_SETUP(UpdateTimeStampOnExit)
CFG_SETUP(Volume)
CFG_SETUP(CursorMode)