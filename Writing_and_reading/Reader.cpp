#include <cerrno>
#include <fstream>

#include "Reader.h"
#include "Utils.h"

writing_and_reading::Reader::Reader(const std::string &file_name) {
    file_ = file_name;
}

Image writing_and_reading::Reader::ReadFile() {
    unsigned char file_header[image::utils::FILE_HEADER_SIZE];
    unsigned char information_header[image::utils::INFORMATION_HEADER_SIZE];

    std::ifstream image_file;
    image_file.open(file_, std::ios::in | std::ios::binary);
    if (!image_file.is_open()) {
        throw std::invalid_argument("Can't open file (file does not exist): " + std::string(file_));
    }
    if (errno == EACCES) {
        throw std::invalid_argument("No permission to open file: " + std::string(file_));
    }
    image_file.read(reinterpret_cast<char *>(file_header), image::utils::FILE_HEADER_SIZE);
    if (file_header[0] != image::utils::FORMAT_BITES[0] || file_header[1] != image::utils::FORMAT_BITES[1]) {
        throw std::invalid_argument("File format is not .bmp: " + std::string(file_));
    }
    try {
        image_file.read(reinterpret_cast<char *>(information_header), image::utils::INFORMATION_HEADER_SIZE);
        uint32_t width = BytesToNumber(information_header + image::utils::HEADER_WIDTH_OFFSET);
        uint32_t height = BytesToNumber(information_header + image::utils::HEADER_HEIGHT_OFFSET);
        Image image(height, width);

        std::vector<std::vector<Color>> image_data(height);
        std::vector<Color> temp_colors(width);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char pixel[3];
                image_file.read(reinterpret_cast<char *>(pixel), 3);
                temp_colors[j].SetValues(static_cast<uint8_t>(pixel[0]), static_cast<uint8_t>(pixel[1]),
                                         static_cast<uint8_t>(pixel[2]));
            }
            image_data[i] = temp_colors;
            image_file.ignore(GetPaddingSize(width));
        }
        image = Image(image_data);
        image_file.close();
        return image;

    } catch (std::exception &e) {
        throw "Error while reading file: " + static_cast<std::string>(e.what());
    }
}

size_t writing_and_reading::Reader::BytesToNumber(unsigned char *bytes) {
    size_t number = *bytes;
    for (size_t i = 0; i < image::utils::KINDS_OF_BITES.size(); ++i) {
        number += *(bytes + i + 1) << image::utils::KINDS_OF_BITES[i];
    }
    return number;
}

const int writing_and_reading::GetPaddingSize(size_t width) {
    return (image::utils::PADDING_BYTES - (width * image::utils::BYTES_PER_PIXEL) % image::utils::PADDING_BYTES) %
           image::utils::PADDING_BYTES;
}
