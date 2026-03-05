#include "pointers.hpp"
#include "memory/pattern_batch.hpp"

namespace ellohim
{
	pointers::pointers()
    {
		pattern_batch main_batch("client.dll");

		main_batch.add("CS2 Input", "48 89 05 ? ? ? ? 0F 57 C0 0F 11 05", [this](pattern ptr) {
			m_csgo_input = ptr.add(3).rip().get();
		});

		main_batch.add("Entity List", "48 89 35 ? ? ? ? 48 85 F6", [this](pattern ptr) {
			m_entity_list = ptr.add(3).rip().get();
		});

		main_batch.add("Local Player Controller", "48 8B 05 ? ? ? ? 41 89 BE", [this](pattern ptr) {
			m_local_player_controller = ptr.add(3).rip().get();
		});

		main_batch.add("Global Vars", "48 89 15 ? ? ? ? 48 89 42", [this](pattern ptr) {
			m_global_vars = ptr.add(3).rip().get();
		});

		main_batch.add("View matrix", "48 8B 05 ? ? ? ? 48 85 C0 74 0A", [this](pattern ptr) {
			m_view_matrix = ptr.add(10).rip().get();
		});

		main_batch.run();

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}
}