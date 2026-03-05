#include "main_worker.hpp"
#include "script.hpp"
#include "ui/canvas.hpp"

namespace ellohim
{
	void main_worker::run()
	{
		while (g_running)
		{
			canvas::check_for_input(); 
			canvas::handle_input();

			script::get_current()->yield();
		}
	}
}