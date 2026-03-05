#pragma once
#include "common.hpp"
#define CHUNK_SIZE 0x1000

namespace ellohim 
{

	class pattern 
	{
		std::vector<std::optional<uint8_t>> compiled;
		std::string name;
		uintptr_t   address = 0;
		std::string module_name;

		uintptr_t   module_base = 0;
		size_t      module_size = 0;

	public:
		pattern(std::string module, std::string name, std::string x) : name(name), module_name(module)
		{
			try
			{
				resolve_module();
				compile(x);
				scan();
			}
			catch(const std::exception& e)
			{
				LOG(WARNING) << name << " not found";
			}
		}

		pattern(std::string name_, uintptr_t addr)
		{
			name = name_;
			address = addr;
		}

		void compile(const std::string& pat);

		void resolve_module();

		pattern& scan();

		inline pattern add(size_t sigOffset)
		{
			return pattern(name, address + sigOffset);
		}

		inline pattern sub(size_t sigOffset)
		{
			return pattern(name, address - sigOffset);
		}

		pattern rip();

		uint64_t get() const
		{
			return address;
		}

		inline operator std::uint64_t() const
		{
			return address;
		}
	};
};