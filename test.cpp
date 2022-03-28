//#define CATCH_CONFIG_MAIN
//#include </Users/dima_sidorov/cpp-base-hse-2022/contrib/catch/catch.hpp>
//#include "Parsing/argument_parser.h"
//#include "Image/Image.h"
//#include "Image/Filters.h"
//#include "Writing_and_reading/Reader.h"
//#include "Writing_and_reading/Writer.h"
//
//#include <iostream>
//
//Image GetInputImage(const std::string& input_file) {
//    writing_and_reading::Reader reader(input_file);
//    Image image = reader.ReadFile();
//    return image;
//}
//
//void WriteOutputImage(const std::string& output_file, const Image& image) {
//    writing_and_reading::Writer writer(output_file);
//    writer.WriteFile(image);
//}
//
//TEST_CASE("Reading valid image") {
//    std::string filename = "example.bmp";
//    try {
//        Image image = GetInputImage(filename);
//    } catch (const std::exception& e) {
//        FAIL("file can be opened");
//    }
//}
//
//TEST_CASE("Reading invalid image") {
//    std::string filename =
//        "not_bmp.bmp";  // получен из example.bmp, но file_header[1] = 'B', а не 'M' (добавил в папку examples)
//    try {
//        Image image = GetInputImage(filename);
//    } catch (const std::exception& e) {
//        std::cout << e.what() << std::endl;  // File format is not .bmp: not_bmp.bmp
//        SUCCEED("file can not be opened");
//    }
//
//    filename = "non-existent.bmp";  // рандомное название файла
//    try {
//        Image image = GetInputImage(filename);
//    } catch (const std::exception& e) {
//        std::cout << e.what() << std::endl;  // Can't open file (file does not exist): non-existent.bmp
//        SUCCEED("file can not be opened");
//    }
//}
//
//TEST_CASE("Writing valid image") {
//    std::string input_filename = "example.bmp";
//    Image image = GetInputImage(input_filename);
//    std::string output_filename = "output.bmp";
//    try {
//        WriteOutputImage(output_filename, image);
//    } catch (const std::exception& e) {
//        FAIL("valid output image");
//    }
//}
//
//TEST_CASE("Writing invalid image") {
//    Image image(0, 0);
//    std::string output_filename = "";
//    try {
//        WriteOutputImage(output_filename, image);
//    } catch (const std::exception& e) {
//        std::cout << e.what() << std::endl;  // Can't open file:
//        SUCCEED("invalid output image");
//    }
//}
//
//TEST_CASE("Crop") {
//    std::string filename = "example.bmp";
//    Image image = GetInputImage(filename);
//    ArgumentParser::Argument argument;
//    argument.Name = "-crop";
//    argument.Parameters.push_back("2048");
//    argument.Parameters.push_back("2048");
//    image = filters::CreateFilter(argument)->Apply(image);
//    Image answer = GetInputImage(filename);
//    REQUIRE(image.GetWidth() == answer.GetWidth());
//    REQUIRE(image.GetHeight() == answer.GetHeight());
//    for (size_t i = 0; i < image.GetHeight(); ++i) {
//        for (size_t j = 0; j < image.GetWidth(); ++j) {
//            REQUIRE(image.GetColor(i, j).red == answer.GetColor(i, j).red);
//            REQUIRE(image.GetColor(i, j).blue == answer.GetColor(i, j).blue);
//            REQUIRE(image.GetColor(i, j).green == answer.GetColor(i, j).green);
//        }
//    }
//
//    argument.Parameters[0] = "9999999";
//    argument.Parameters[1] = "9999999";
//    image = filters::CreateFilter(argument)->Apply(image);
//    REQUIRE(image.GetWidth() == answer.GetWidth());
//    REQUIRE(image.GetHeight() == answer.GetHeight());
//
//    argument.Parameters[0] = "1";
//    argument.Parameters[1] = "1";
//    image = filters::CreateFilter(argument)->Apply(image);
//    REQUIRE(image.GetWidth() == 1);
//    REQUIRE(image.GetHeight() == 1);
//}
//
//TEST_CASE("Negative") {
//    std::string filename = "example.bmp";
//    Image image = GetInputImage(filename);
//    ArgumentParser::Argument argument;
//    argument.Name = "-neg";
//    image = filters::CreateFilter(argument)->Apply(image);
//    Image additional_image = GetInputImage(filename);
//    REQUIRE(image.GetWidth() == additional_image.GetWidth());
//    REQUIRE(image.GetHeight() == additional_image.GetHeight());
//    for (size_t i = 0; i < image.GetHeight(); ++i) {
//        for (size_t j = 0; j < image.GetWidth(); ++j) {
//            REQUIRE(image.GetColor(i, j).red + additional_image.GetColor(i, j).red == 255);
//            REQUIRE(image.GetColor(i, j).blue + additional_image.GetColor(i, j).blue == 255);
//            REQUIRE(image.GetColor(i, j).green + additional_image.GetColor(i, j).green == 255);
//        }
//    }
//}
//
//TEST_CASE("Grayscale") {
//    std::string filename = "example.bmp";
//    Image image = GetInputImage(filename);
//    ArgumentParser::Argument argument;
//    argument.Name = "-gs";
//    image = filters::CreateFilter(argument)->Apply(image);
//    Image additional_image = GetInputImage(filename);
//    REQUIRE(image.GetWidth() == additional_image.GetWidth());
//    REQUIRE(image.GetHeight() == additional_image.GetHeight());
//    for (size_t i = 0; i < image.GetHeight(); ++i) {
//        for (size_t j = 0; j < image.GetWidth(); ++j) {
//            REQUIRE(image.GetColor(i, j).red == image.GetColor(i, j).blue);
//            REQUIRE(image.GetColor(i, j).green == image.GetColor(i, j).blue);
//        }
//    }
//}