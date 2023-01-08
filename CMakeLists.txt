cmake_minimum_required(VERSION 3.5)
project(ClapWorkspace)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "x64")
else()
set(ARCH_DIR "x86")
endif()

set(SDL_SENSORS OFF CACHE BOOL "Enabling SDL Sensors")

set(ENABLE_BZIP2 OFF CACHE BOOL "Enable use of BZip2")
set(ENABLE_LZMA OFF CACHE BOOL "Enable use of LZMA")
set(ENABLE_ZSTD OFF CACHE BOOL "Enable use of Zstandard")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

add_compile_definitions(AL_LIBTYPE_STATIC)

set(ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)


add_compile_definitions(_CRT_SCURE_NO_WARNINGS)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/openal-soft)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/lua)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zip)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/msdf-c)


#Folder Structure
set_target_properties (Glad PROPERTIES
    FOLDER Dependencies
)
set_target_properties (SDL2-static PROPERTIES
    FOLDER Dependencies
)
set_target_properties (SDL2main PROPERTIES
    FOLDER Dependencies
)
set_target_properties (OpenAL PROPERTIES
    FOLDER Dependencies
)
set_target_properties (uninstall PROPERTIES
    FOLDER Dependencies
)
set_target_properties (yaml-cpp PROPERTIES
    FOLDER Dependencies
)
set_target_properties (ImGui PROPERTIES
    FOLDER Dependencies
)
set_target_properties (zip PROPERTIES
    FOLDER Dependencies
)
set_target_properties (msdf-c PROPERTIES
    FOLDER Dependencies
)


set(CLAP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/openal-soft/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui   
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/stb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/msdf-c
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/asio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/sol
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/lua/src
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zip/src)

set(CLAP_LINK_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/openal-soft
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/lua
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zip
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/msdf-c)
                   
set(CLAP_WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Clap)