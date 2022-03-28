#ifndef CPP_PILOT_HSE_READER_H
#define CPP_PILOT_HSE_READER_H

#include "../Image/Image.h"

namespace writing_and_reading {
class Reader {
public:
    Reader(const std::string& file_name);
    Image ReadFile();

private:
    std::string file_;
    size_t BytesToNumber(unsigned char* bytes);
};

const int GetPaddingSize(size_t width);
}

#endif  // CPP_PILOT_HSE_READER_H
