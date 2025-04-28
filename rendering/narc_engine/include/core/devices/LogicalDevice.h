#pragma once

namespace narc_engine
{
    class EngineBuilder;

    class LogicalDevice
    {
    public:
        LogicalDevice(const EngineBuilder* builder);
        ~LogicalDevice() = default;
    };
}