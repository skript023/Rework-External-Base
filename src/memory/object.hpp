#pragma once

namespace ellohim
{
    template<typename T>
    class Object
    {
    public:
        uintptr_t address;

        Object(uintptr_t addr)
            : address(addr) {}

        T operator->()
        {
            return T(address);
        }
    };

    template<typename T>
    class ObjectProperty
    {
    public:
        uintptr_t address;

        ObjectProperty(uintptr_t addr)
            : address(addr) {}

        ObjectPtr<T> operator->()
        {
            uintptr_t ptr = g_process->read<uintptr_t>(address);
            return ObjectPtr<T>(ptr);
        }
    };
}