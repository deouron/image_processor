#ifndef CPP_PILOT_HSE_UTILS_H
#define CPP_PILOT_HSE_UTILS_H

#include <string>
#include <vector>
#include <iostream>

namespace image::utils {
    const int FILE_HEADER_SIZE = 14;
    const int INFORMATION_HEADER_SIZE = 40;
    const int PADDING_BYTES = 4;
    const int BYTES_PER_PIXEL = 3;
    const int HEADER_HEIGHT_OFFSET = 4;
    const int HEADER_WIDTH_OFFSET = 8;
    const int HEADER_FILE_SIZE_OFFSET = 8;
    const int HEADERS_SUM_POSITION = 10;
    const int FORMAT_SIZE_FIRST_POSITION = 0;
    const int FORMAT_SIZE_SECOND_POSITION = 1;
    const int INFORMATION_HEADER_SIZE_POSITION = 0;
    const int NOT_EMPTY_BITE = 1;
    const int NOT_EMPTY_BITE_POSITION = 12;
    const std::vector<int> KINDS_OF_BITES = {8, 16, 24};
    const std::vector<char> FORMAT_BITES = {'B', 'M'};
}

#endif  // CPP_PILOT_HSE_UTILS_H
