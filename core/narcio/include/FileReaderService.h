#pragma once

namespace narc_io {
    class IImageStream;
    struct Model3D;

    class NARC_IO_API FileReaderService
    {
    public:
        FileReaderService() = delete;
        ~FileReaderService() = delete;

        static std::vector<char> readFile(const std::string& filename);
        static Model3D load3DModel(const std::string& filename);
        static std::unique_ptr<IImageStream> readImage(const std::string& filename);
    };
}
