#ifndef CPP_PILOT_HSE_WRITER_H
#define CPP_PILOT_HSE_WRITER_H

#include <fstream>
#include "../Image/Image.h"
#include <vector>
#include <cstddef>

namespace writing_and_reading {
    class Writer {
    public:
        Writer(std::string filename);
        void WriteFile(const Image &image);

    private:
        void WriteHeader(unsigned char *file_header, size_t file_size);
        void WriteInformationHeader(unsigned char *information_header, size_t height, size_t width);

        template <typename T>
        void NumberToBytes(T number, unsigned char* bytes);

        std::string filename_;
    };
}

#endif  // CPP_PILOT_HSE_WRITER_H
