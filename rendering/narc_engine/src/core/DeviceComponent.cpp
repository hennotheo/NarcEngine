//
// Created by theoh on 3/15/2025.
//

#include "core/DeviceComponent.h"

#include "Engine.h"

namespace narc_engine {
    DeviceComponent::DeviceComponent()
    {
        m_device = Engine::getInstance()->getDevice();
    }
} // narc_engine