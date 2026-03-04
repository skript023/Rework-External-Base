#include "pointers.hpp"
#include "memory/pattern.hpp"

namespace ellohim
{
	pointers::pointers()
    {
        m_hwnd = FindWindow(nullptr, "Grand Theft Auto V");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}
}