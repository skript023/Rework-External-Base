#include "thread_pool.hpp"

namespace ellohim
{
	void thread_pool::init_impl(const std::size_t preallocated_thread_count)
	{
		m_accept_jobs = true;
		m_allocated_thread_count = preallocated_thread_count;
		m_busy_threads = 0;

		rescale_thread_pool();
	}

	void thread_pool::rescale_thread_pool()
	{
		m_thread_pool.reserve(m_allocated_thread_count);
		m_job_stack.resize(m_allocated_thread_count);

		if (m_thread_pool.size() < m_allocated_thread_count)
		{
			for (auto i = m_thread_pool.size(); i < m_allocated_thread_count; i++)
				m_thread_pool.emplace_back(std::thread(&thread_pool::run, this, i));

			LOG(VERBOSE) << "Resizing thread pool from " << m_thread_pool.size() << " to " << m_allocated_thread_count;
		}
	}

	void thread_pool::destroy_impl()
	{
		{
			std::unique_lock lock(m_lock);
			m_accept_jobs = false;
		}
		m_data_condition.notify_all();

		for (auto& thread : m_thread_pool)
			thread.join();

		m_thread_pool.clear();
		m_job_stack.clear();
	}

	void thread_pool::queue_job_impl(std::function<void()> func, std::source_location location)
	{
		if (func)
		{
			static std::atomic<size_t> next_queue{ 0 };
			auto index = next_queue++ % m_job_stack.size();

			{
				std::unique_lock lock(m_lock);
				m_job_stack[index].push_front({ func, location });

				if (m_busy_threads >= m_job_stack.size()) [[unlikely]]
				{
					LOG(WARNING) << "Thread pool potentially starved, resizing to accommodate for load.";

					if (m_allocated_thread_count >= MAX_POOL_SIZE)
					{
						LOG(FATAL) << "The thread pool limit has been reached, whatever you did this should not occur in production.";
					}
					if (m_accept_jobs && m_allocated_thread_count + 1 <= MAX_POOL_SIZE)
					{
						++m_allocated_thread_count;
						rescale_thread_pool();
					}
				}
			}
			m_data_condition.notify_all();
		}
	}

	void thread_pool::run(size_t index)
	{
		for (;;)
		{
			thread_pool_job job;
			std::unique_lock lock(m_lock);

			m_data_condition.wait(lock, [this, index]() {
				return !m_job_stack[index].empty() || !m_accept_jobs;
				});

			if (!m_accept_jobs) [[unlikely]]
				break;

			if (!m_job_stack[index].empty()) [[unlikely]]
			{
				job = m_job_stack[index].front();
				m_job_stack[index].pop_front();
			}
			else if (m_allocated_thread_count > 1) [[likely]]
			{
				for (size_t i = 0; i < m_job_stack.size(); i++)
				{
					size_t victim = (index + i + 1) % m_job_stack.size();

					if ((m_job_stack[victim].size() >= 4) && victim != index) [[unlikely]]
					{
						job = m_job_stack[victim].back();
						m_job_stack[victim].pop_back();

						break;
					}
				}
			}

			lock.unlock();

			++m_busy_threads;

			try
			{
				std::invoke(job.m_func);
			}
			catch (const std::exception& e)
			{
				LOG(WARNING) << "Exception thrown while executing job in thread:" << e.what();
			}

			--m_busy_threads;
		}
	}
}
