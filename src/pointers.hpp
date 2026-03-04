#pragma once
#include "pointer.hpp"
#include "utility/enums.hpp"

namespace ellohim
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		HWND m_hwnd;
		
	};

	inline pointers* g_pointers;
}