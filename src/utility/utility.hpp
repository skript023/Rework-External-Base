#pragma once
#include "pointers.hpp"
#include "joaat.hpp"
#include "class_grabber.hpp"

namespace ellohim::utility
{
	inline bool is_float_equal(float a, float b, float epsilon  = 1e-6)
	{
		return fabs(a - b) < epsilon;
	}
}

namespace ellohim::memory
{
	inline bool is_bit_set(uintptr_t _value, int _bit)
	{
		auto value = g_process->read<int>(_value);
		if ((value >> _bit) & 1) return true;
		return false;
	}

	inline int set_bit(uintptr_t _value, int _bit)
	{
		auto value = g_process->read<int>(_value);
		return value |= 1 << _bit;
	}

	inline int clear_bit(uintptr_t _value, int _bit)
	{
		auto value = g_process->read<int>(_value);
		return value &= ~(1 << _bit);
	}

	template <typename T>
	inline bool is_bit_set(T _value, int _bit)
	{
		if ((_value >> _bit) & 1LL) return true;
		return false;
	}

	template <typename T>
	inline T set_bit(T _value, int _bit)
	{
		return _value |= 1 << _bit;
	}

	template <typename T>
	inline T clear_bit(T _value, int _bit)
	{
		return _value &= ~(1 << _bit);
	}
}