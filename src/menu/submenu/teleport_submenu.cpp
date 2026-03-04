#include "../view.hpp"
#include "../core/submenu.hpp"

namespace ellohim
{
	std::string category = "Default";

	void view::teleport_submenu()
	{
		canvas::add_tab<regular_submenu>("Teleport", SubmenuTeleport, [](regular_submenu* sub)
		{
			sub->add_option<sub_option>("Custom Teleport", nullptr, SubmenuCustomTeleport);
		});

		canvas::add_submenu<regular_submenu>("Custom Teleport", SubmenuCustomTeleport, [](regular_submenu* sub)
		{
			
		});
	}
}