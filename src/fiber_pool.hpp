#pragma once
#include "common.hpp"

namespace ellohim
{
	class fiber_pool
	{
		static fiber_pool& get()
		{
			static fiber_pool instance;
			return instance;
		}
		void init_impl(std::size_t num_fibers);
		void destroy_impl();
		void queue_job_impl(std::function<void()> func);

		void fiber_tick_impl();
		static void fiber_func();
	public:
		static void init(std::size_t num_fibers) { get().init_impl(num_fibers); }
		static void destroy() { get().destroy_impl(); }
		static void queue_job(std::function<void()> func) { get().queue_job_impl(func); }
		static void fiber_tick() { get().fiber_tick_impl(); }
	private:
		std::recursive_mutex m_mutex;
		std::stack<std::function<void()>> m_jobs;
	};

	inline fiber_pool* g_fiber_pool{};
}
