#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <string>

class SymetricCipher
{
   private:
    unsigned i = 0;

   public:
    using BLOCK = U64;
    void reset()
    {
        i = 0;
    }
    BLOCK operator()(BLOCK b)
    {
        int ptr[2];
        std::memcpy(ptr, &b, 8);
        for (int k = 0; k < 8; ++k) {
            ptr[0] ^= 1000000007 * ptr[1] + 1 + i;
            std::swap(ptr[0], ptr[1]);
        }
        std::swap(ptr[0], ptr[1]);
        std::memcpy(&b, ptr, 8);
        ++i;
        return b;
    }
};

class Hasher
{
   public:
    void add_data(const void *data, int len)
    {
        int offset = 0;
        while (len > 0) {
            toggle ^= 1;
            U64 b = 0;
            std::memcpy(&b, reinterpret_cast<const char *>(data) + offset, (len % 8 == 0) ? 8 : len % 8);
            b ^= state[toggle];
            state[toggle] = cipher(b);
            len -= 8;
            offset += 8;
        }
    }
    template <typename T>
    void operator()(const T &data)
    {
        add_data(&data, sizeof(T));
    }
    std::string to_md5() const
    {
        using byte = unsigned char;
        std::string md5(32, '0');
        const byte *const state_bytes = reinterpret_cast<const byte *>(state);
        for (int i = 0; i < 16; ++i) {
            md5[i * 2] = to_hex_char(state_bytes[i] >> 4);
            md5[i * 2 + 1] = to_hex_char(state_bytes[i] >> 4 & 0xF);
        }
        return md5;
    }

   private:
    char to_hex_char(int v) const
    {
        if (v >= 0 && v <= 9) return (char)(v + '0');
        if (v >= 10 && v <= 15) return (char)(v - 10 + 'a');
        return '!';
    }

    SymetricCipher cipher;
    U64 state[2]{0, 0};
    int toggle = 0;
};