#include "common.hpp"
#include "fiber_pool.hpp"
#include "script.hpp"
#include "script_mgr.hpp"

namespace ellohim
{
	void fiber_pool::init_impl(std::size_t num_fibers)
	{
		for (std::size_t i = 0; i < num_fibers; ++i)
		{
			g_script_mgr.add_script(std::make_unique<script>(&fiber_func));
		}
	}

	void fiber_pool::destroy_impl()
	{
		std::lock_guard lock(m_mutex);

		while (!m_jobs.empty())
			m_jobs.pop();
	}

	void fiber_pool::queue_job_impl(std::function<void()> func)
	{
		if (func)
		{
			std::lock_guard lock(m_mutex);
			m_jobs.push(std::move(func));
		}
	}

	void fiber_pool::fiber_tick_impl()
	{
		std::unique_lock lock(m_mutex);
		if (!m_jobs.empty())
		{
			auto job = std::move(m_jobs.top());
			m_jobs.pop();
			lock.unlock();

			std::invoke(std::move(job));
		}
	}

	void fiber_pool::fiber_func()
	{
		while (true)
		{
			fiber_pool::fiber_tick();
			script::get_current()->yield();
		}
	}
}
