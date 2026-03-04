#include "../view.hpp"
#include "../core/submenu.hpp"

namespace ellohim
{
    void view::esp_submenu()
    {
        canvas::add_tab<regular_submenu>("ESP", SubmenuESP, [](regular_submenu* sub)
        {
        });
    }
}