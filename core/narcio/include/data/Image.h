#pragma once

#include "FileReader.h"
#include "Core.h"

namespace narc_io
{
    struct NARC_IO_API Image final
    {
        friend class FileReader;

    public:
        inline ~Image()
        {
            FileReader::releaseImage(m_data);
        }

        inline int getWidth() const { return m_width; }
        inline int getHeight() const { return m_height; }
        inline int getChannels() const { return m_channels; }
        inline void* getData() const { return m_data; }

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
