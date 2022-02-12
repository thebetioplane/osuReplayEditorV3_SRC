#pragma once

#include "stdafx.h"

class md5_t {
private:
	int compare_to(const md5_t &) const;
public:
	uint8_t h[16];
	void set(const std::string &);
	bool operator< (const md5_t &rhs) const {
		return compare_to(rhs) < 0;
	}
	bool operator> (const md5_t &rhs) const {
		return compare_to(rhs) > 0;
	}
	bool operator<= (const md5_t &rhs) const {
		return compare_to(rhs) <= 0;
	}
	bool operator>= (const md5_t &rhs) const {
		return compare_to(rhs) >= 0;
	}
	bool operator== (const md5_t &rhs) const {
		return compare_to(rhs) == 0;
	}
	bool operator!= (const md5_t &rhs) const {
		return compare_to(rhs) != 0;
	}
	friend std::ostream &operator<<(std::ostream &, const md5_t &);
	bool is_zero() const {
		for (int i = 0; i < 16; ++i) {
			if (h[i] != 0)
				return false;
		}
		return true;
	}
};

