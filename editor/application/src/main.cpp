#include <csignal>
#include <cstdlib>
#include <execinfo.h>
#include <iostream>
#include <unistd.h>

#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    try
    {
        narc_engine::VulkanInstance instance;
        instance.init();
        instance.shutdown();
    }
    catch (const std::exception& e)
    {
        NARC_LOG_ERROR("Exception caught: {}", e.what());
    }
}

#endif
