#pragma once
#include "pointer.hpp"
#include "utility/enums.hpp"

namespace ellohim
{
	class pointer_data
	{
	public:
		pointer<uintptr_t> m_view_matrix;
		pointer<uintptr_t> m_global_vars;
		pointer<uintptr_t> m_local_player_controller;
		pointer<uintptr_t> m_entity_list;
		pointer<uintptr_t> m_csgo_input;
	};

	class pointers : public pointer_data
	{
	public:
		explicit pointers();
		~pointers();
	public:
		HWND m_hwnd;
	};

	inline pointers* g_pointers;
}