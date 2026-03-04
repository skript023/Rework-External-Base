#include "../view.hpp"
#include "../core/submenu.hpp"

namespace ellohim
{
    void view::stats_submenu()
    {
        canvas::add_tab<regular_submenu>("Misc", SubmenuStats, [](regular_submenu* sub)
        {
            
        });
    }
}