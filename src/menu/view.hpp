#pragma once
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace ellohim
{
	class view
	{
	public:
		static void register_submenu()
		{
			home();
			player_submenu();
			stats_submenu();
			teleport_submenu();
			esp_submenu();
			setting_submenu();
		}
	public:
		static void home();
		static void censor();
		static void draw_input();
		static void draw_overlay();
		static void notifications();
		static void packet_monitor();
		static void packet_replay();
		static void lua_debug();
		static void lua_console();
	public:
		static void esp_submenu();
		static void player_submenu();
		static void stats_submenu();
		static void teleport_submenu();
		static void setting_submenu();
	};
}