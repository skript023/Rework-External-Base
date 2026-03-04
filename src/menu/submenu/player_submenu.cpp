#include "../view.hpp"
#include "../core/submenu.hpp"

namespace ellohim
{
    void view::player_submenu()
    {
        canvas::add_tab<regular_submenu>("Player", SubmenuPlayer, [](regular_submenu* sub)
        {
			static bool test = false;
            sub->add_option<bool_option<bool>>("Test", nullptr, &test);
        });
    }
}