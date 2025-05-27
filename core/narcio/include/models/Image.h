#pragma once

#include "FileReader.h"

namespace narc_io
{
    struct NARC_IO_API Image final
    {
        friend class FileReader;

    public:
        ~Image()
        {
            FileReader::releaseImage(m_data);
        }

        NARC_GETTER(int, getWidth, m_width);
        NARC_GETTER(int, getHeight, m_height);
        NARC_GETTER(int, getChannels, m_channels);
        NARC_GETTER(void*, getData, m_data);

    private:
        Image(int width, int height, int channels, void* data)
            : m_width(width), m_height(height), m_channels(channels), m_data(data)
        {
        }

        int m_width;
        int m_height;
        int m_channels;

        void* m_data;
    };
}
