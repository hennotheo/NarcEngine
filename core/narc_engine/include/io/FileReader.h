#pragma once

namespace narc_engine
{
    struct Image;
    
    class FileReader
    {
    public:
        FileReader() = delete;
        ~FileReader() = delete;

        static std::vector<char> readFile(const std::string& filename);
        static Image readImage(const std::string& filename);
        static void releaseImage(void* imageData);
    };
}
