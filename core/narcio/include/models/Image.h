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

        GETTER int getWidth() const { return m_width; }
        GETTER int getHeight() const { return m_height; }
        GETTER int getChannels() const { return m_channels; }
        GETTER void* getData() const { return m_data; }

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
