#pragma once
#include "pattern.hpp"

namespace ellohim
{
    class pattern_batch
    {
        struct entry
        {
            std::string name;
            std::string signature;
            std::function<void(pattern)> callback;

            entry(std::string n,
                std::string sig,
                std::function<void(pattern)> cb)
                : name(std::move(n)),
                signature(std::move(sig)),
                callback(std::move(cb))
            {}
        };

        std::string m_module;
        std::vector<entry> m_entries;

    public:
        pattern_batch(std::string module);

        void add(std::string name, std::string signature, std::function<void(pattern)> callback);

        void run();
    };
}