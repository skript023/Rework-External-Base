#include "pattern.hpp"
#include "process.hpp"

namespace ellohim
{
	void pattern::compile(const std::string& pat)
	{
		int hexchar = 0;
		bool first = true;
		bool lastwaswc = false;

		auto to_hex = [&](char c) -> std::uint8_t
		{
			switch (std::toupper(c))
			{
			case '0':
				return static_cast<std::uint8_t>(0);
			case '1':
				return static_cast<std::uint8_t>(1);
			case '2':
				return static_cast<std::uint8_t>(2);
			case '3':
				return static_cast<std::uint8_t>(3);
			case '4':
				return static_cast<std::uint8_t>(4);
			case '5':
				return static_cast<std::uint8_t>(5);
			case '6':
				return static_cast<std::uint8_t>(6);
			case '7':
				return static_cast<std::uint8_t>(7);
			case '8':
				return static_cast<std::uint8_t>(8);
			case '9':
				return static_cast<std::uint8_t>(9);
			case 'A':
				return static_cast<std::uint8_t>(10);
			case 'B':
				return static_cast<std::uint8_t>(11);
			case 'C':
				return static_cast<std::uint8_t>(12);
			case 'D':
				return static_cast<std::uint8_t>(13);
			case 'E':
				return static_cast<std::uint8_t>(14);
			case 'F':
				return static_cast<std::uint8_t>(15);
			default:
				return 0;
			}
		};

		for (char c : pat)
		{
			if (c == '?' && !lastwaswc)
			{
				lastwaswc = true;
				compiled.push_back(std::nullopt);
			}
			else
			{
				lastwaswc = false;
				if (c == ' ') continue;
				hexchar += first ? to_hex(c) * 0x10 : to_hex(c);

				if (!first) 
				{
					compiled.push_back(hexchar);
					hexchar = 0;
				}
				first = !first;
			}
		}
	}

	void pattern::resolve_module()
	{
		MODULEENTRY32 mod{};
		mod.dwSize = sizeof(mod);

		HANDLE snapshot = CreateToolhelp32Snapshot(
			TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
			GetProcessId(g_process->get_handle()));

		if (Module32First(snapshot, &mod))
		{
			do
			{
				if (!_stricmp(mod.szModule, module_name.c_str()))
				{
					module_base = reinterpret_cast<uintptr_t>(mod.modBaseAddr);
					module_size = mod.modBaseSize;
					CloseHandle(snapshot);
					return;
				}
			} while (Module32Next(snapshot, &mod));
		}

		CloseHandle(snapshot);
		throw std::runtime_error("module not found");
	}

	pattern& pattern::scan()
	{
		if (!module_base || !module_size)
			throw std::runtime_error("module not resolved");

		address = 0;

		std::unique_ptr<uint8_t[]> buffer(new uint8_t[CHUNK_SIZE]);

		uintptr_t end = module_base + module_size;

		for (uintptr_t current = module_base; current < end; current += CHUNK_SIZE)
		{
			size_t bytes_to_read = std::min<size_t>(CHUNK_SIZE, end - current);

			if (!g_process->read_raw_ex(current, bytes_to_read, buffer.get()))
				continue;

			for (size_t i = 0; i < bytes_to_read; ++i)
			{
				bool found = true;

				for (size_t j = 0; j < compiled.size(); ++j)
				{
					if (i + j >= bytes_to_read)
					{
						found = false;
						break;
					}

					if (compiled[j].has_value() && compiled[j].value() != buffer[i + j])
					{
						found = false;
						break;
					}
				}

				if (found)
				{
					address = current + i;
					return *this;
				}
			}
		}

		throw std::runtime_error("cannot find pattern " + name);
	}

	pattern pattern::rip() 
	{
		return add(g_process->read<int>(address)).add(4);
	}
}