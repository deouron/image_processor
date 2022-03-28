#ifndef CPP_PILOT_HSE_FILTERS_H
#define CPP_PILOT_HSE_FILTERS_H

#include "Image.h"
#include "../Parsing/argument_parser.h"

#include <memory>

namespace filters {

    class Filter {
    public:
        Filter() = default;
        virtual ~Filter() = default;
        virtual Image Apply(const Image& img) const = 0;

    protected:
        template<typename T>
        std::vector<T> GetPixelColors(const std::vector<std::vector<T>>& matrix, const Image& image, size_t i, size_t j) const;
    };

    class Crop : public Filter {
    public:
        Crop(size_t width, size_t height);
        Image Apply(const Image& img) const override;

    private:
        size_t width_;
        size_t height_;
    };

    class Grayscale : public Filter {
    public:
        Image Apply(const Image& img) const override;
    };

    class Negative : public Filter {
    public:
        Image Apply(const Image& img) const override;
    };

    class Sharpening : public Filter {
    public:
        Image Apply(const Image& img) const override;
    };

    class EdgeDetection : public Filter {
    public:
        EdgeDetection(int threshold);
        Image Apply(const Image& img) const override;

    private:
        int threshold_;
    };

    class GaussianBlur : public Filter {
    public:
        GaussianBlur(double sigma);
        Image Apply(const Image& img) const override;
    private:
        double sigma_;
    };

    class Sobel : public Filter {
    public:
        Image Apply(const Image& img) const override;
    };

    std::unique_ptr<filters::Filter> CreateFilter(const ArgumentParser::Argument& argument);

}  // namespace filters

#endif  // CPP_PILOT_HSE_FILTERS_H
