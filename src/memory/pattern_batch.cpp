#include "pattern_batch.hpp"

namespace ellohim
{
	pattern_batch::pattern_batch(std::string module) : m_module(std::move(module))
	{}

	void pattern_batch::add(std::string name, std::string signature, std::function<void(pattern)> callback)
	{
		m_entries.emplace_back(std::move(name), std::move(signature), std::move(callback));
	}
	void pattern_batch::run()
	{
        for (auto& e : m_entries)
        {
            try
            {
                pattern ptr(m_module, e.name, e.signature);

                e.callback(ptr);

                LOGF(VERBOSE, "Found '{}' [{:X}]", e.name, ptr.get());
            }
            catch (...)
            {
                LOG(WARNING) << e.name << " failed";
            }
        }
	}
}