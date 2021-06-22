#pragma once
#include <iostream>

namespace mitama {
	template <class T>
	struct actual {
		bool cond;
		T value;
		operator bool() const { return cond; }
	};

	template <class T>
	actual(bool, T)->actual<T>;

	template <class T>
	inline std::ostream& operator<<(std::ostream& os, actual<T> const& act) {
		return os << act.value;
	}
}
