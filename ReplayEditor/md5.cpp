#include "stdafx.h"
#include "md5.hpp"

static uint8_t from_hex_char(char hex)
{
	if (hex >= '0' && hex <= '9')
		return hex - '0';
	if (hex >= 'a' && hex <= 'f')
		return hex - 'a' + 10;
	if (hex >= 'A' && hex <= 'F')
		return hex - 'A' + 10;
	fatal("from_hex_char: bad character given");
	return 0;
}

static char to_hex_char(int v)
{
	if (v >= 0 && v <= 9)
		return (char)(v + '0');
	if (v >= 10 && v <= 15)
		return (char)(v - 10 + 'a');
	fatal("to_hex_char: bad character given");
	return '!';
}

void md5_t::set(const std::string &str_val)
{
	if (str_val.empty()) {
		std::memset(h, 0, 16);
		return;
	}
	if (str_val.size() != 32)
		fatal("md5 was incorrect length");
	for (int i = 0, j = 0; i < 16; ++i, j += 2) {
		h[i] = (from_hex_char(str_val[j]) << 4) | from_hex_char(str_val[j + 1]);
	}
}

int md5_t::compare_to(const md5_t &rhs) const
{
	for (int i = 0; i < 16; ++i) {
		int diff = h[i] - rhs.h[i];
		if (diff != 0)
			return diff;
	}
	return 0;
}

std::ostream &operator<<(std::ostream &o, const md5_t &self)
{
	for (int i = 0; i < 16; ++i) {
		o << to_hex_char(self.h[i] >> 4);
		o << to_hex_char(self.h[i] & 0xF);
	}
	return o;
}
