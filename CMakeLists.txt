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

add_compile_definitions(_CRT_SCURE_NO_WARNINGS)
add_compile_definitions(CLAP_USE_SDL3)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zip)


#Folder Structure
set_target_properties (Glad PROPERTIES
    FOLDER Dependencies
)
set_target_properties (SDL3-static PROPERTIES
    FOLDER Dependencies
)
set_target_properties (uninstall PROPERTIES
    FOLDER Dependencies
)
set_target_properties (ImGui PROPERTIES
    FOLDER Dependencies
)
set_target_properties (zip PROPERTIES
    FOLDER Dependencies
)


set(CLAP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/miniaudio
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui   
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/stb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/json
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/utf8
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zip/src)

set(CLAP_LINK_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zip)
                   
set(CLAP_WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CLAP_LINK_LIBS SDL3::SDL3 ImGui Glad zip)

add_subdirectory(Clap)