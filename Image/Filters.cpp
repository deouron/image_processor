#include "Filters.h"

#include <cmath>
#include <cstdlib>

uint8_t GetCorrectColorRange(int color, const uint8_t max_color = 255) {
    if (color < 0) {
        return 0;
    } else if (color > max_color) {
        return max_color;
    }
    return static_cast<uint8_t>(color);
}

template <typename T>
std::vector<T> filters::Filter::GetPixelColors(const std::vector<std::vector<T>>& matrix, const Image& image, size_t i,
                                               size_t j) const {
    T red = 0, green = 0, blue = 0;
    for (size_t x = 0; x < matrix.size(); ++x) {
        for (size_t y = 0; y < matrix.front().size(); ++y) {
            red += image .GetColor(std::clamp(i - 1 + x, static_cast<size_t>(0), image.GetHeight() - 1),
                                 std::clamp(j - 1 + y, static_cast<size_t>(0), image.GetWidth() - 1))
                       .red * matrix[x][y];
            green += image.GetColor(std::clamp(i - 1 + x, static_cast<size_t>(0), image.GetHeight() - 1),
                                   std::clamp(j - 1 + y, static_cast<size_t>(0), image.GetWidth() - 1))
                         .green * matrix[x][y];
            blue += image.GetColor(std::clamp(i - 1 + x, static_cast<size_t>(0), image.GetHeight() - 1),
                                  std::clamp(j - 1 + y, static_cast<size_t>(0), image.GetWidth() - 1))
                        .blue *  matrix[x][y];
        }
    }
    return {red, green, blue};
}

filters::Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

Image filters::Crop::Apply(const Image& img) const {
    Image result(std::min(height_, img.GetHeight()), std::min(width_, img.GetWidth()));
    for (size_t i = img.GetHeight() - std::min(height_, img.GetHeight()); i < img.GetHeight(); ++i) {
        std::vector<Color> row(std::min(width_, img.GetWidth()));
        for (size_t j = 0; j < std::min(width_, img.GetWidth()); ++j) {
            result.SetColor(i - (img.GetHeight() - std::min(height_, img.GetHeight())), j, img.GetData()[i][j]);
        }
    }
    return result;
}

Image filters::Grayscale::Apply(const Image& img) const {
    std::vector<std::vector<Color>> new_image_data;
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        std::vector<Color> row(img.GetWidth());
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            uint8_t grayscale_color =
                img.GetColor(i, j).red * 0.299 + img.GetColor(i, j).blue * 0.114 + img.GetColor(i, j).green * 0.587;
            row[j] = {grayscale_color, grayscale_color, grayscale_color};
        }
        new_image_data.push_back(row);
    }
    return {new_image_data};
}

Image filters::Negative::Apply(const Image& img) const {
    std::vector<std::vector<Color>> new_image_data;
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        std::vector<Color> row(img.GetWidth());
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            row[j] = {static_cast<uint8_t>(255 - img.GetColor(i, j).blue),
                      static_cast<uint8_t>(255 - img.GetColor(i, j).green),
                      static_cast<uint8_t>(255 - img.GetColor(i, j).red)};
        }
        new_image_data.push_back(row);
    }
    return {new_image_data};
}

Image filters::Sharpening::Apply(const Image& img) const {
    std::vector<std::vector<Color>> new_image_data;
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        std::vector<Color> row(img.GetWidth());
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            std::vector<int> colors = GetPixelColors(matrix, img, i, j);
            row[j].red = GetCorrectColorRange(colors[0]);
            row[j].green = GetCorrectColorRange(colors[1]);
            row[j].blue = GetCorrectColorRange(colors[2]);
        }
        new_image_data.push_back(row);
    }
    return {new_image_data};
}

filters::EdgeDetection::EdgeDetection(int threshold) : threshold_(threshold) {
}

Image filters::EdgeDetection::Apply(const Image& img) const {
    const Image gray_img = filters::Grayscale().Apply(img);
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    std::vector<std::vector<Color>> new_image_data;
    for (size_t i = 0; i < gray_img.GetHeight(); ++i) {
        std::vector<Color> row(img.GetWidth());
        for (size_t j = 0; j < gray_img.GetWidth(); ++j) {
            std::vector<int> colors = GetPixelColors(matrix, gray_img, i, j);
            if (colors.front() > threshold_) {
                row[j] = {255, 255, 255};
            } else {
                row[j] = {0, 0, 0};
            }
        }
        new_image_data.push_back(row);
    }
    return {new_image_data};
}

filters::GaussianBlur::GaussianBlur(double sigma) : sigma_(sigma) {
}

Image filters::GaussianBlur::Apply(const Image& img) const {
    double gauss_radius = sigma_ * 2;
    double sum = 0;
    std::vector<std::vector<double>> gauss_matrix(2 * gauss_radius + 1, std::vector<double>(2 * gauss_radius + 1));
    for (size_t i = 0; i < gauss_matrix.size(); ++i) {
        for (size_t j = 0; j < gauss_matrix.front().size(); ++j) {
            double current_value = std::exp(-0.5 * (static_cast<double>(i) - gauss_radius) / sigma_ *
                                            (static_cast<double>(i) - gauss_radius) / sigma_) *
                                   std::exp(-0.5 * (static_cast<double>(j) - gauss_radius) / sigma_ *
                                            (static_cast<double>(j) - gauss_radius) / sigma_);
            gauss_matrix[i][j] = current_value;
            sum += current_value;
        }
    }

    for (size_t i = 0; i < gauss_matrix.size(); ++i) {
        for (size_t j = 0; j < gauss_matrix.front().size(); ++j) {
            gauss_matrix[i][j] /= sum;
        }
    }

    std::vector<std::vector<Color>> new_image_data;
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        std::vector<Color> row(img.GetWidth());
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            std::vector<double> colors = GetPixelColors(gauss_matrix, img, i, j);
            row[j].red = static_cast<uint8_t>(colors[0]);
            row[j].green = static_cast<uint8_t>(colors[1]);
            row[j].blue = static_cast<uint8_t>(colors[2]);
        }
        new_image_data.push_back(row);
    }

    return {new_image_data};
}

Image filters::Sobel::Apply(const Image& img) const {
    const Image gray_image = filters::Grayscale().Apply(img);
    std::vector<std::vector<Color>> new_image_data;
    const std::vector<std::vector<int>> matrix_x = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const std::vector<std::vector<int>> matrix_y = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        std::vector<Color> row(img.GetWidth());
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            std::vector<int> colors_x = GetPixelColors(matrix_x, gray_image, i, j);
            std::vector<int> colors_y = GetPixelColors(matrix_y, gray_image, i, j);
            row[j].red = static_cast<uint8_t>(std::sqrt(std::pow(GetCorrectColorRange(colors_x[0]), 2) +
                                                        std::pow(GetCorrectColorRange(colors_y[0]), 2)));
            row[j].green = static_cast<uint8_t>(std::sqrt(std::pow(GetCorrectColorRange(colors_x[1]), 2) +
                                                          std::pow(GetCorrectColorRange(colors_y[1]), 2)));
            row[j].blue = static_cast<uint8_t>(std::sqrt(std::pow(GetCorrectColorRange(colors_x[2]), 2) +
                                                         std::pow(GetCorrectColorRange(colors_y[2]), 2)));
        }
        new_image_data.push_back(row);
    }
    return {new_image_data};
}

std::unique_ptr<filters::Filter> filters::CreateFilter(const ArgumentParser::Argument& argument) {
    const std::string& filter = argument.Name;
    if (filter == "-crop") {
        if (argument.Parameters.size() != 2) {
            throw std::invalid_argument("Invalid parameters for -crop");
        }
        try {
            size_t height = std::stoull(argument.Parameters[0]);
            size_t width = std::stoull(argument.Parameters[1]);
            auto ptr = std::make_unique<filters::Crop>(width, height);
            return ptr;
        } catch (std::exception &e) {
            throw std::invalid_argument("Entered not a number during -crop: " + argument.Parameters[0] + " or " +
                                        argument.Parameters[1]);
        }
    } else if (filter == "-gs") {
        if (!argument.Parameters.empty()) {
            throw std::invalid_argument("Invalid parameters for -gs");
        }
        auto ptr = std::make_unique<filters::Grayscale>();
        return ptr;
    } else if (filter == "-neg") {
        if (!argument.Parameters.empty()) {
            throw std::invalid_argument("Invalid parameters for -neg");
        }
        auto ptr = std::make_unique<filters::Negative>();
        return ptr;
    } else if (filter == "-sharp") {
        if (!argument.Parameters.empty()) {
            throw std::invalid_argument("Invalid parameters for -sharp");
        }
        auto ptr = std::make_unique<filters::Sharpening>();
        return ptr;
    } else if (filter == "-edge") {
        if (argument.Parameters.size() != 1) {
            throw std::invalid_argument("Invalid parameters for -edge");
        }
        try {
            int threshold = std::stoi(argument.Parameters.front());
            auto ptr = std::make_unique<filters::EdgeDetection>(threshold);
            return ptr;
        } catch (std::exception &e) {
            throw std::invalid_argument("Entered not a number or wrong number type during -edge: " +
                                        argument.Parameters.front());
        }
    } else if (filter == "-blur") {
        if (argument.Parameters.size() != 1) {
            throw std::invalid_argument("Invalid parameters for -blur");
        }
        try {
            double sigma = std::stod(argument.Parameters.front());
            auto ptr = std::make_unique<filters::GaussianBlur>(sigma);
            return ptr;
        } catch (std::exception &e) {
            throw std::invalid_argument("Entered not a number or wrong number type during -blur: " +
                                        argument.Parameters.front());
        }
    } else if (filter == "-sob") {  // доп фильтр
        if (!argument.Parameters.empty()) {
            throw std::invalid_argument("Invalid parameters for -sob");
        }
        auto ptr = std::make_unique<filters::Sobel>();
        return ptr;
    } else {
        throw std::invalid_argument("An unknown parameter " + filter);
    }
}
