#pragma once

namespace ellohim
{
    template<typename T>
    class Array
    {
    public:
        uintptr_t address;

        Array(uintptr_t addr)
            : address(addr) {}

        T operator[](size_t index)
        {
            return g_process->read<T>(address + index * sizeof(T));
        }
    };

    template<typename T>
    class TArrayView
    {
    public:
        uintptr_t address;

        int size()
        {
            auto arr = g_process->read<UE_TArray<T>>(address);
            return arr.count;
        }

        T operator[](int i)
        {
            auto arr = g_process->read<UE_TArray<T>>(address);
            return g_process->read<T>(arr.data + i * sizeof(T));
        }
    };
}