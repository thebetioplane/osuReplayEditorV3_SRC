// clang-format off
#include "stdafx.h"
// clang-format on

#include "bwriter.hpp"

#include <sstream>

template <typename T>
static void slide(std::ofstream &file, const T &value)
{
    file.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

bwriter_t &bwriter_t::operator<<(const int8_t &value)
{
    file.put(value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const uint8_t &value)
{
    write_byte(value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const int16_t &value)
{
    slide(file, value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const uint16_t &value)
{
    slide(file, value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const int32_t &value)
{
    slide(file, value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const uint32_t &value)
{
    slide(file, value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const I64 &value)
{
    slide(file, value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const U64 &value)
{
    slide(file, value);
    return *this;
}

#ifdef BREADER_BWRITER_FLOAT
bwriter_t &bwriter_t::operator<<(const float &value)
{
    slide(file, value);
    return *this;
}
#endif

bwriter_t &bwriter_t::operator<<(const float &value)
{
    slide(file, value);
    return *this;
}

bwriter_t &bwriter_t::operator<<(const bool &value)
{
    file.put(value ? 1 : 0);
    return *this;
}

void bwriter_t::write_uleb128(uint32_t value)
{
    do {
        uint8_t b = value & 0x7f;
        value >>= 7;
        if (value != 0) b |= 0x80;
        file.put(b);
    } while (value != 0);
}

bwriter_t &bwriter_t::operator<<(const std::string &value)
{
    file.put(0x0b);
    write_uleb128(static_cast<uint32_t>(value.size()));
    for (unsigned i = 0; i < value.size(); ++i) {
        file.put(value[i]);
    }
    return *this;
}

void bwriter_t::write_buf(const void *buf, int len)
{
    file.write(reinterpret_cast<const char *>(buf), len);
}