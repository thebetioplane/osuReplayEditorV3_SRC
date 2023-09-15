#pragma once

// clang-format off
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <gl/GL.h>
// clang-format on

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

using I64 = int64_t;
using U64 = uint64_t;
using SongTime_t = int;

#define RATIO(a, b) (static_cast<float>(a) / static_cast<float>(b))
#define ROUND_TO_INT(a) static_cast<int>((a) + 0.5f)

#ifdef _DEBUG
void _fatal(const std::string &msg, const char *file, const int line);
#define fatal(msg) (_fatal(msg, __FILE__, __LINE__))
void _not_fatal(const std::string &msg, const char *file, const int line);
#define not_fatal(msg) (_not_fatal(msg, __FILE__, __LINE__))
#define TRACE (std::cout << '[' << __func__ << ':' << __LINE__ << "] ")
#define trace(what) (TRACE << #what " = " << (what) << std::endl)
#else
void fatal(const std::string &msg);
void not_fatal(const std::string &msg);
#endif

extern HWND error_message_owner;

static inline std::wstring string_to_wstring(const std::string &s)
{
    constexpr DWORD flags = 0;
    const int len = MultiByteToWideChar(CP_UTF8, flags, s.c_str(), -1, nullptr, 0);
    if (len <= 0) return std::wstring();
    std::wstring ret(len, 'X');
    MultiByteToWideChar(CP_UTF8, flags, s.c_str(), -1, &ret[0], static_cast<int>(ret.size()));
    return ret;
}

static inline std::ostream &operator<<(std::ostream &o, const std::wstring &s)
{
    const char *ptr = reinterpret_cast<const char *>(&s[0]);
    const size_t len = s.size() * sizeof(wchar_t);
    for (size_t i = 0; i < len; ++i) {
        if (*ptr < ' ' || *ptr == 128)
            o << '.';
        else
            o << *ptr;
        ++ptr;
    }
    return o;
}

#define IS_PRESSED(prevm1, prevm2, currm1, currm2) (!(prevm1) && (currm1) || (!(prevm2) && (currm2)))
#define IS_PRESSED_INT(prevm1, prevm2, currm1, currm2) ((!(prevm1) && (currm1)) + ((!(prevm2) && (currm2))))
#define IS_RELEASE(prevm1, prevm2, currm1, currm2) ((prevm1) || (prevm2)) && (!(currm1) && !(currm2));

#define glColorWhite() glColor3f(1.0f, 1.0f, 1.0f)
#define glColorBlack() glColor3f(0.0f, 0.0f, 0.0f)
#define glColorRed() glColor3f(1.0f, 0.0f, 0.0f)
#define glColorGreen() glColor3f(0.0f, 1.0f, 0.0f)
#define glColorBlue() glColor3f(0.0f, 0.0f, 1.0f)
#define glColorGray() glColor3f(0.537254f, 0.537254f, 0.537254f)
#define glColorMagenta() glColor3f(1.0f, 0.0f, 1.0f)
#define glColorCyan() glColor3f(0.0f, 1.0f, 1.0f)
#define glColorYellow() glColor3f(1.0f, 1.0f, 0.0f)

#define DLLFUN(T) extern "C" __declspec(dllexport) T __stdcall
