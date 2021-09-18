cmake_minimum_required(VERSION 3.5)

project(Glad LANGUAGES C)

add_library(Glad STATIC src/glad.c include/glad/glad.h include/KHR/khrplatform.h)

target_include_directories(Glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)