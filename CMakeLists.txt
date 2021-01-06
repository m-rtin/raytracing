cmake_minimum_required(VERSION 3.17)
project(helloWorld)

set(CMAKE_CXX_STANDARD 14)

add_executable(helloWorld main.cpp stb_image_write.h stb_image.h Vector.cpp Vector.h)