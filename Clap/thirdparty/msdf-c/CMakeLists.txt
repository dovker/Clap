cmake_minimum_required(VERSION 3.5)

project(msdf-c LANGUAGES C)

add_library(msdf-c STATIC msdf.c msdf.h)

target_include_directories(msdf-c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../stb/include)