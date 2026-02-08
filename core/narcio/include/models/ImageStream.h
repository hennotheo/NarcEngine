#pragma once

namespace narc_io {
    class ImageStream final : public IImageStream
    {
    public:
        explicit ImageStream(std::string_view path);
        ~ImageStream() override;

        NARC_OVERRIDE_GETTER(int, getWidth, m_width);
        NARC_OVERRIDE_GETTER(int, getHeight, m_height);
        NARC_OVERRIDE_GETTER(int, getChannels, m_channels);
        NARC_OVERRIDE_GETTER(void*, getData, m_pixels);

    private:
        std::string m_path;

        int m_width{};
        int m_height{};
        int m_channels{};

        void* m_pixels;
    };
}
