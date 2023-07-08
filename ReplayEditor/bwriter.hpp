#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <fstream>
#include <string_view>

class bwriter_t
{
   public:
    std::ofstream file;
    void write_uleb128(uint32_t);

   public:
    bwriter_t(const std::wstring &fname) : file(fname, std::ios::binary)
    {
    }
    bool is_open() const
    {
        return file.good();
    }
    bool is_closed() const
    {
        return !file.good();
    }
    void write_byte(uint8_t b)
    {
        file.put(b);
    }
    bwriter_t &operator<<(const int8_t &);
    bwriter_t &operator<<(const uint8_t &);
    bwriter_t &operator<<(const int16_t &);
    bwriter_t &operator<<(const uint16_t &);
    bwriter_t &operator<<(const int32_t &);
    bwriter_t &operator<<(const uint32_t &);
    bwriter_t &operator<<(const I64 &);
    bwriter_t &operator<<(const U64 &);
#ifdef BREADER_BWRITER_FLOAT
    bwriter_t &operator<<(const float &);
#endif
    bwriter_t &operator<<(const float &);
    bwriter_t &operator<<(const bool &);
    bwriter_t &operator<<(std::string_view);
    void write_buf(const void *, int);
};
