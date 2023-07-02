#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <fstream>

class breader_t
{
   private:
    std::ifstream file;
    uint32_t read_uleb128();

   public:
    breader_t(const std::wstring &fname) : file(fname, std::ios::binary)
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
    uint8_t read_byte()
    {
        return static_cast<uint8_t>(file.get());
    }
    breader_t &operator>>(int8_t &);
    breader_t &operator>>(uint8_t &);
    breader_t &operator>>(int16_t &);
    breader_t &operator>>(uint16_t &);
    breader_t &operator>>(int32_t &);
    breader_t &operator>>(uint32_t &);
    breader_t &operator>>(I64 &);
    breader_t &operator>>(U64 &);
    breader_t &operator>>(float &);
#ifdef BREADER_BWRITER_FLOAT
    breader_t &operator>>(float &);
#endif
    breader_t &operator>>(bool &);
    breader_t &operator>>(std::string &);
    void operator+=(int);
    void dummy_string();
    void fill_buf(void *, int);
};
