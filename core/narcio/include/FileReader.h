#pragma once

namespace narc_io
{
    struct Image;

    class NARC_IO_API FileReader
    {
    public:
        FileReader() = delete;
        ~FileReader() = delete;

        static std::vector<char> readFile(const std::string& filename);
        static Image readImage(const std::string& filename);
        static void releaseImage(void* imageData);
    };
}
