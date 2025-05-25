//
// Created by theoh on 24/05/2025.
//

#pragma once

namespace narc_engine
{
#define NARC_VK_GET_INSTANCE_PROC_ADDR(instance, funcName) \
    reinterpret_cast<PFN_##funcName>(vkGetInstanceProcAddr(instance, #funcName));

    template <typename T>
    T* chainExtensions(std::vector<T*>& extensions)
    {
        static_assert(std::is_class_v<T> && requires(T l) { l.pNext; }, "Type must have a member named 'pNext'");

        for (size_t i = 0; i < extensions.size() - 1; ++i)
        {
            extensions[i]->pNext = extensions[i + 1];
        }
        extensions[extensions.size() - 1]->pNext = nullptr;

        return extensions[0];
    }
}
