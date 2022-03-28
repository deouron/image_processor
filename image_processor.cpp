#include "Parsing/argument_parser.h"
#include "Image/Image.h"
#include "Image/Filters.h"
#include "Writing_and_reading/Reader.h"
#include "Writing_and_reading/Writer.h"

#include <sstream>
#include <iostream>

void WriteHelp() {
    std::cerr << "Reference: \n"
        "The first argument is a name of input file, the second argument is a name of output file \n"
        "-crop: Crop the image to the specified width and height. The top left of the image is used. "
        "Parameters: width, height \n"
        "-gs: Converts an image to grayscale. Parameters: -\n"
        "-neg: Converts an image to a negative. Parameters: -\n"
        "-sharp: Sharpening. Parameters: -\n"
        "-edge: Border highlighting. Parameters: threshold\n"
        "-blur: Gaussian blur. Parameters: sigma\n\n";
}

std::vector<ArgumentParser::Argument> GetArguments(int argc, char *argv[]) {
    std::vector<ArgumentParser::Argument> arguments = ArgumentParser::ParseArguments(argc, argv);
    if (arguments.size() <= 2) {
        throw std::invalid_argument("You have not entered enough parameters.");
    }
    return arguments;
}

Image GetInputImage(const std::string& input_file) {
    writing_and_reading::Reader reader(input_file);
    Image image = reader.ReadFile();
    return image;
}

void WriteOutputImage(const std::string& output_file, const Image& image) {
    writing_and_reading::Writer writer(output_file);
    writer.WriteFile(image);
}

Image ApplyFilter(const std::vector<ArgumentParser::Argument>& arguments, Image& image) {
    for (size_t i = 2; i < arguments.size(); ++i) {
        image = filters::CreateFilter(arguments[i])->Apply(image);
    }
    return image;
}

int main(int argc, char *argv[]) {
    try {
        std::vector<ArgumentParser::Argument> arguments = GetArguments(argc, argv);
        Image image = GetInputImage(arguments[0].Name);
        image = ApplyFilter(arguments, image);
        WriteOutputImage(arguments[1].Name, image);
    } catch (const std::exception& e) {
        WriteHelp();
        std::cerr << e.what() << std::endl;
    }
}