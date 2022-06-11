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

add_library(Clap STATIC ${CLAP_SRC_FILES} src/pch.h src/pch.cpp)

if(MSVC)
add_precompiled_header(Clap pch.h FORCEINCLUDE
                        SOURCE_CXX src/pch.cpp)
else()
add_precompiled_header(Clap src/pch.h SOURCE_CXX src/pch.cpp FORCEINCLUDE)
endif()

target_include_directories(Clap SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src 
                                SYSTEM PUBLIC ${CLAP_INCLUDE_DIRS})

add_compile_definitions(_CRT_SCURE_NO_WARNINGS)
add_compile_definitions(CLAP_USE_OPENGL CLAP_OPENGL_4_5 CLAP_USE_SDL2 CLAP_DEBUG) #Add automatic checking for Opengl later
# IF YOU CAN SUPPORT OPENGL4.5, DEFINE MC_OPENGL_4_5

target_link_directories(Clap PRIVATE ${CLAP_LINK_DIRS})
# TODO: MAKE IT SO THE USER DECIDES GRAPHICS LIBRARY

target_link_libraries(Clap PUBLIC SDL2main SDL2-static bgfx ImGui Glad zlib lua-lib yaml-cpp)


