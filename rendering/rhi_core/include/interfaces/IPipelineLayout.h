//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    struct VertexLayout;

    class IPipelineLayout : public narc_core::IInitialisable
    {
    public:
        virtual IPipelineLayout* setVertexShader(const std::string& path) = 0;
        virtual IPipelineLayout* setFragmentShader(const std::string& path) = 0;
        virtual IPipelineLayout* setVertexLayout(const VertexLayout& layout) = 0;
        virtual IPipelineLayout* addBinding(const IDescriptorLayout* binding) = 0;
    };
}
