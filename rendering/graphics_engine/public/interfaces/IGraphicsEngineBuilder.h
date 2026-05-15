//
// Created by theo on 5/10/26.
//


#pragma once

namespace narc_engine {
    class IGraphicsEngineBuilder
    {
    public:
        virtual ~IGraphicsEngineBuilder() = default;

        virtual IGraphicsEngineBuilder& useVulkan()= 0;

        virtual std::unique_ptr<IGraphicsEngine> build() = 0;
    };
}