#pragma once
#include <source_location>

namespace ellohim
{
	constexpr auto MAX_POOL_SIZE = 32u;

	struct thread_pool_job
	{
		std::function<void()> m_func;
		std::source_location m_source_location;
	};

	class thread_pool
	{
		std::atomic<bool> m_accept_jobs;
		std::condition_variable m_data_condition;

		std::vector<std::deque<thread_pool_job>> m_job_stack;
		std::mutex m_lock;
		std::vector<std::thread> m_thread_pool;

		std::atomic<size_t> m_allocated_thread_count;
		std::atomic<size_t> m_busy_threads;

		static thread_pool& instance()
		{
			static thread_pool i{};

			return i;
		}
	public:
		thread_pool() = default;
		~thread_pool() noexcept = default;

		static void init(const std::size_t preallocated_thread_count = 2) { instance().init_impl(preallocated_thread_count); };
		static void destroy() { instance().destroy_impl(); };
		static void queue_job(std::function<void()> func, std::source_location location = std::source_location::current()) { instance().queue_job_impl(func, location); }

		std::pair<size_t, size_t> usage() const
		{
			return { m_busy_threads, m_allocated_thread_count };
		}

	private:
		void init_impl(const std::size_t preallocated_thread_count);
		void destroy_impl();
		void queue_job_impl(std::function<void()> func, std::source_location location);

		void run(size_t index);
		void rescale_thread_pool();
	};
}
