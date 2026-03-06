#pragma once

namespace ellohim
{
    template<typename T>
    class Property
    {
    public:
        uintptr_t address;

        Property(uintptr_t addr)
            : address(addr) {}

        operator T() const
        {
            return g_process->read<T>(address);
        }

        T operator=(const T& value)
        {
            g_process->write(address, value);
            return value;
        }
    };
}