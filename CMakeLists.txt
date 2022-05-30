cmake_minimum_required(VERSION 3.5)
project(ClapWorkspace)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works


# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "x64")
else()
set(ARCH_DIR "x86")
endif()

set(SDL_SENSORS OFF CACHE BOOL "Enabling SDL Sensors")
set(ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/soloud/contrib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/gravity)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp)

#Folder Structure
set_target_properties (Glad PROPERTIES
    FOLDER Dependencies
)
set_target_properties (SDL2-static PROPERTIES
    FOLDER Dependencies
)
set_target_properties (SDL2 PROPERTIES
    FOLDER Dependencies
)
set_target_properties (uninstall PROPERTIES
    FOLDER Dependencies
)
set_target_properties (SDL2main PROPERTIES
    FOLDER Dependencies
)
set_target_properties (zlib PROPERTIES
    FOLDER Dependencies
)
# set_target_properties (zip PROPERTIES
#     FOLDER Dependencies
# )
set_target_properties (yaml-cpp PROPERTIES
    FOLDER Dependencies
)
set_target_properties (ImGui PROPERTIES
    FOLDER Dependencies
)

set(CLAP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui   
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/stb_image/include
    #${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/soloud/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib
    # ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/asio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/json/include
    ${GRAVITY_INCLUDE_DIR})

set(CLAP_LINK_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib
                #    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/gravity)

set(CLAP_WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Clap)
add_subdirectory(TestGround)
add_subdirectory(Game2D)