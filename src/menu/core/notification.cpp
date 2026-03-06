#include "../view.hpp"
#include "renderer.hpp"
#include "notification/notification_service.hpp"

namespace ellohim
{
	float draw_notification(float start_pos, ImDrawList* dl, std::string title, std::string message, ImVec4 color)
	{
		auto screen_res = renderer::get_screen_res();;

		int x = screen_res.x;
		int y = screen_res.y;

		ImColor textCol = ImGui::ColorConvertFloat4ToU32({ 1.f, 1.f, 1.f, 1.f });
		color.w = 0.5f;
		ImColor fadeBegin = ImGui::ColorConvertFloat4ToU32(color);
		color.w = 0.f;
		ImColor fadeEnd = ImGui::ColorConvertFloat4ToU32(color);

		int j = 0;
		int prevSpace = 0;
		float total_size = 0.f;
		std::vector<std::string> split_points;
		for (int i = 0; i <= message.size(); i++)
		{
			std::string current_message = message.substr(j, i - j);

			if (message.substr(i, 1) == " ")
			{
				prevSpace = i;
			}

			ImVec2 size = ImGui::CalcTextSize(current_message.c_str());

			if (i == message.size())
			{
				total_size = total_size + size.y;
				split_points.push_back(message.substr(j, i));
			}
			else if (size.x >= 330.f)
			{
				total_size = total_size + size.y;
				split_points.push_back(message.substr(j, prevSpace - j));
				j = prevSpace + 1;
				i = prevSpace;
			}
		}

		dl->AddRectFilled({ (float)x - 360.f, 10.f + start_pos }, { (float)x - 10.f, start_pos + 55.f + total_size }, g_settings.window.color);
		dl->AddRectFilledMultiColor({ (float)x - 360.f, 10.f + start_pos }, { (float)x - 255.f, start_pos + 55.f + total_size }, fadeBegin, fadeEnd, fadeEnd, fadeBegin);

		dl->AddText(g_settings.window.font_sub_title, 22.f, { (float)x - 350.f, 15.f + start_pos }, textCol, title.c_str());
		int i = 0;
		for (std::string txt : split_points)
		{
			dl->AddText({ (float)x - 350.f, 40.f + (i * 20.f) + start_pos }, textCol, txt.c_str());
			i++;
		}

		return start_pos + 55.f + total_size;
	}

	void view::notifications()
	{
		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

		std::vector<notification_data> notifications = notification::get();

		float prev_pos = 0.f;
		for (int i = 0; i < notifications.size(); i++)
		{
			notification_data& n = notifications[i];

			prev_pos = draw_notification(prev_pos, draw_list, n.title, n.message, notification::get_color(n.type));
		}
	}
}