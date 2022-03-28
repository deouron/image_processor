cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(image_processor image_processor.cpp Parsing/argument_parser.h
        Parsing/argument_parser.cpp Image/Image.cpp Image/Image.h Image/Color.cpp
        Image/Color.h "Writing_and_reading/Reader.cpp" "Writing_and_reading/Reader.h" "Writing_and_reading/Writer.cpp"
        "Writing_and_reading/Writer.h" Image/Filters.cpp Image/Filters.h "Writing_and_reading/Utils.h" test.cpp)
