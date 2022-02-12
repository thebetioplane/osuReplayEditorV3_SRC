#pragma once

#include "stdafx.h"
#include "md5.hpp"

class SymetricCipher {
private:
	unsigned i = 0;
public:
	using BLOCK = U64;
	void reset() {
		i = 0;
	}
	BLOCK operator()(BLOCK b) {
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

class Hasher {
private:
	SymetricCipher cipher;
	U64 state[2]{ 0, 0 };
	int toggle = 0;
public:
	void add_data(const void *data, int len) {
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
	void operator()(const T& data) {
		add_data(&data, sizeof(T));
	}
	void to_md5(md5_t &m) const {
		std::memcpy(m.h, state, 16);
	}
};