cmake_minimum_required(VERSION 3.5)

project(Clap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(thirdparty/cmake_precompiled_header/PrecompiledHeader.cmake)

file(GLOB_RECURSE CLAP_SRC_FILES "src/*.cpp" 
                                 "src/*.h" 
                                 "thirdparty/stb_image/*.cpp"
                                 "thirdparty/stb_image/*.h" 
                                 "thirdparty/glm/*.hpp"
                                 "thirdparty/glm/*.inl")

add_library(Clap STATIC ${CLAP_SRC_FILES})

add_precompiled_header(Clap srd/pch.h SOURCE_CXX src/pch.cpp FORCEINCLUDE)

target_include_directories(Clap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} 
                                PUBLIC ${CLAP_INCLUDE_DIRS})

add_compile_definitions(_CRT_SCURE_NO_WARNINGS)

target_link_directories(Clap PRIVATE thirdparty/SDL2
                             PRIVATE thirdparty/Glad
                             PRIVATE thirdparty/imgui
                             PRIVATE thirdparty/zlib
                             PRIVATE thirdparty/libzip
                             PRIVATE thirdparty/yaml-cpp)

target_link_libraries(Clap PUBLIC SDL2main SDL2-static ImGui Glad zlib libzip YAML_CPP)


