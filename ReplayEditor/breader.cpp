#include "stdafx.h"
#include "breader.hpp"

template<typename T>
static void slide(std::ifstream &file, T &value)
{
	file.read(reinterpret_cast<char *>(&value), sizeof(T));
}

breader_t &breader_t::operator>> (int8_t &value) {
	value = static_cast<int8_t>(file.get());
	return *this;
}

breader_t &breader_t::operator>> (uint8_t &value) {
	value = read_byte();
	return *this;
}

breader_t &breader_t::operator>> (int16_t &value) {
	slide(file, value);
	return *this;
}

breader_t &breader_t::operator>> (uint16_t &value) {
	slide(file, value);
	return *this;
}

breader_t &breader_t::operator>> (int32_t &value) {
	slide(file, value);
	return *this;
}

breader_t &breader_t::operator>> (uint32_t &value) {
	slide(file, value);
	return *this;
}

breader_t &breader_t::operator>> (I64 &value) {
	slide(file, value);
	return *this;
}

breader_t &breader_t::operator>> (U64 &value) {
	slide(file, value);
	return *this;
}

#ifdef BREADER_BWRITER_FLOAT
breader_t &breader_t::operator>> (float &value) {
	slide(file, value);
	return *this;
}
#endif

breader_t &breader_t::operator>> (float &value) {
	slide(file, value);
	return *this;
}

breader_t &breader_t::operator>> (bool &value) {
	value = file.get() != 0;
	return *this;
}

uint32_t breader_t::read_uleb128() {
	uint8_t b;
	uint32_t value = 0;
	int shift_amt = 0;
	do {
		b = read_byte();
		value |= (b & 0x7f) << shift_amt;
		shift_amt += 7;
	} while (b & 0x80);
	return value;
}

breader_t &breader_t::operator>> (std::string &value) {
	value.clear();
	const uint8_t b = read_byte();
	if (b != 0x0b)
		return *this;
	const uint32_t len = read_uleb128();
	value.resize(len);
	for (unsigned i = 0; i < len; ++i) {
		value[i] = static_cast<char>(file.get());
	}
	return *this;
}

breader_t &breader_t::operator>> (md5_t &value) {
	std::string hash;
	*this >> hash;
	value.set(hash);
	return *this;
}

void breader_t::operator+= (int amt) {
	file.seekg(amt, std::ios_base::cur);
}

void breader_t::fill_buf(void *buf, int count) {
	file.read(reinterpret_cast<char*>(buf), count);
}

void breader_t::dummy_string() {
	const uint8_t b = read_byte();
	if (b != 0x0b)
		return;
	const uint32_t len = read_uleb128();
	file.seekg(len, std::ios_base::cur);
}

