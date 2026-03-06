#include "renderer.hpp"
#include "pointers.hpp"
#include "file_manager.hpp"
#include "thread_pool.hpp"
#include "fiber_pool.hpp"
#include "script_mgr.hpp"
#include "features.hpp"
#include "menu_settings.hpp"

#include "memory/process.hpp"
#include "worker/main_worker.hpp"
#include "settings/settings.hpp"
#include "utility/class_grabber.hpp"

int main()
{
	using namespace ellohim;

	std::filesystem::path base_dir = std::filesystem::current_path();
	base_dir /= "data";

	file_manager::init(base_dir);

	settings::initialize(file_manager::get_project_file("./settings.json"));

	logger::initialize("Quantum", file_manager::get_project_file("./logs/console.log"), true);

	LOG(INFO) << R"kek(
 ______      _                        _   ____                 
|  ____|    | |                      | | |  _ \                
| |__  __  _| |_ ___ _ __ _ __   __ _| | | |_) | __ _ ___  ___ 
|  __| \ \/ / __/ _ \ '__| '_ \ / _` | | |  _ < / _` / __|/ _ \
| |____ >  <| ||  __/ |  | | | | (_| | | | |_) | (_| \__ \  __/
|______/_/\_\\__\___|_|  |_| |_|\__,_|_| |____/ \__,_|___/\___|
 )kek";
	auto pid = process::find_process_id("cs2.exe");

	while (!pid)
	{
		LOG(INFO) << "Waiting for game process...";
		std::this_thread::sleep_for(1s);
		pid = process::find_process_id("cs2.exe");
	}

	auto process_instance = std::make_unique<process>(pid);
	LOG(INFO) << "Process initalized.";

	g_settings.load();

	auto pointers_instance = std::make_unique<pointers>();
	LOG(INFO) << "Pointers initialized.";

	fiber_pool::init(10);
	LOG(INFO) << "Fiber pool initialized.";
	
	thread_pool::init(std::thread::hardware_concurrency() / 2);
	LOG(INFO) << "Thread pool initialized.";

	auto class_grabber_instance = std::make_unique<LoadClass>();
	LOG(INFO) << "Classes initialized.";

	renderer::init();
	LOG(INFO) << "Renderer Initialized.";

	g_script_mgr.add_script(std::make_unique<script>(&features::script_func));
	g_script_mgr.add_script(std::make_unique<script>(&main_worker::run));
	LOG(INFO) << "Scripts registered.";

	LOG(INFO) << "Program is running";
	g_script_mgr.run();

	while (g_running)
	{
		settings::tick();
		g_settings.attempt_save();

		Sleep(1);
	}

	g_script_mgr.remove_all_scripts();
	LOG(INFO) << "Scripts unregistered.";

	renderer::destroy();
	LOG(INFO) << "Renderer Uninitialized";

	process_instance.reset();
	LOG(INFO) << "Process uninitalized.";

	pointers_instance.reset();
	LOG(INFO) << "Pointers uninitialized.";

	class_grabber_instance.reset();
	LOG(INFO) << "Classes uninitialized.";

	fiber_pool::destroy();
	LOG(INFO) << "Fiber pool uninitialized.";

	thread_pool::destroy();
	LOG(INFO) << "Destroyed thread pool.";

	g_settings.attempt_save();

	logger::destroy();

	exit(0);
}