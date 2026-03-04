#include "esp.h"
#include <gui.hpp>

namespace ellohim
{
    void esp::draw_esp()
	{
        /*float width = static_cast<float>(g_pointers->m_resolution->x / 2);
        float height = static_cast<float>(g_pointers->m_resolution->y / 2);

        if (!g_settings.esp.is_active)
            return;

        const auto view = g_settings.actors.view();

        if (!view) return;

        for (const auto& data : *view)
        {
			if (!data.actor) continue;

            Color red = { 255, 0, 0, 255 };
            Color white = { 255, 255, 255, 255 };
            Color green = { 0, 255, 0, 255 };

            Color lineColor = (data.distance < 100.f) ? green : red;
            Color textColor = (data.distance < 100.f) ? green : white;

            if (g_settings.esp.draw_line)
                canvas::draw_line(width, 0, data.screen.X, data.screen.Y, lineColor, 1.f);
            if (g_settings.esp.draw_name)
                canvas::draw_stroke_text(data.screen.X, data.screen.Y, textColor, data.display_text);
        }*/
	}
}