#pragma once

namespace narc_io {
    struct Image;

    NARC_IO_API class FileReader
    {
    public:
        FileReader() = delete;

        ~FileReader() = delete;

        static std::vector<char> readFile(const std::string& filename);

        static Image readImage(const std::string& filename);

        static void releaseImage(void* imageData);
    };
}