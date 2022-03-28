#include "Utils.h"
#include "Writer.h"
#include "Reader.h"

#include <utility>

writing_and_reading::Writer::Writer(std::string filename) : filename_(std::move(filename)) {
}

void writing_and_reading::Writer::WriteFile(const Image &image_file) {
    std::ofstream output_file;
    output_file.open(filename_, std::ios::out | std::ios::binary);
    if (!output_file.is_open()) {
        throw std::invalid_argument("Can't open file: " + filename_);
    }

    const size_t file_size = image::utils::FILE_HEADER_SIZE + image::utils::INFORMATION_HEADER_SIZE +
                             image_file.GetWidth() * image_file.GetHeight() * 3 +
                             GetPaddingSize(image_file.GetWidth()) * image_file.GetWidth();

    unsigned char file_header[image::utils::FILE_HEADER_SIZE] = {};
    std::fill(file_header, file_header + image::utils::FILE_HEADER_SIZE, 0);
    WriteHeader(file_header, file_size);

    unsigned char information_header[image::utils::INFORMATION_HEADER_SIZE];
    std::fill(information_header, information_header + image::utils::INFORMATION_HEADER_SIZE, 0);
    WriteInformationHeader(information_header, image_file.GetHeight(), image_file.GetWidth());

    output_file.write(reinterpret_cast<char *>(file_header), image::utils::FILE_HEADER_SIZE);
    output_file.write(reinterpret_cast<char *>(information_header), image::utils::INFORMATION_HEADER_SIZE);

    unsigned char empty_bmp[3] = {0, 0, 0};
    for (size_t x = 0; x < image_file.GetHeight(); ++x) {
        for (size_t y = 0; y < image_file.GetWidth(); ++y) {
            auto blue = static_cast<unsigned char>(image_file.GetColor(x, y).blue);
            auto green = static_cast<unsigned char>(image_file.GetColor(x, y).green);
            auto red = static_cast<unsigned char>(image_file.GetColor(x, y).red);
            unsigned char pixel[] = {blue, green, red};
            output_file.write(reinterpret_cast<char *>(pixel), 3);
        }
        output_file.write(reinterpret_cast<char *>(empty_bmp), GetPaddingSize(image_file.GetWidth()));
    }
    output_file.close();
}

void writing_and_reading::Writer::WriteHeader(unsigned char *file_header, size_t file_size) {
    file_header[image::utils::FORMAT_SIZE_FIRST_POSITION] = image::utils::FORMAT_BITES[0];
    file_header[image::utils::FORMAT_SIZE_SECOND_POSITION] = image::utils::FORMAT_BITES[1];
    NumberToBytes(file_size, file_header + image::utils::HEADER_FILE_SIZE_OFFSET);
    file_header[image::utils::HEADERS_SUM_POSITION] =
        image::utils::FILE_HEADER_SIZE + image::utils::INFORMATION_HEADER_SIZE;
}

void writing_and_reading::Writer::WriteInformationHeader(unsigned char *information_header, size_t height,
                                                         size_t width) {
    information_header[image::utils::INFORMATION_HEADER_SIZE_POSITION] = image::utils::INFORMATION_HEADER_SIZE;
    NumberToBytes(height, information_header + image::utils::HEADER_HEIGHT_OFFSET);
    NumberToBytes(width, information_header + image::utils::HEADER_WIDTH_OFFSET);
    information_header[image::utils::NOT_EMPTY_BITE_POSITION] = image::utils::NOT_EMPTY_BITE;
    information_header[image::utils::FILE_HEADER_SIZE] = image::utils::KINDS_OF_BITES[2];
}

template <typename T>
void writing_and_reading::Writer::NumberToBytes(T number, unsigned char *bytes) {
    *bytes = number;
    for (size_t i = 0; i < image::utils::KINDS_OF_BITES.size(); ++i) {
        *(bytes + i + 1) = number >> image::utils::KINDS_OF_BITES[i];
    }
}
