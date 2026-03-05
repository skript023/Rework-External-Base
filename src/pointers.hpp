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

		pointer<uintptr_t> m_view_matrix;
		pointer<uintptr_t> m_global_vars;
		pointer<uintptr_t> m_local_player_controller;
		pointer<uintptr_t> m_entity_list;
		pointer<uintptr_t> m_csgo_input;
	public:
		HWND m_hwnd;
		
	};

	inline pointers* g_pointers;
}