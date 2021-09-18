cmake_minimum_required(VERSION 3.5)
project(ClapWorkspace)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "x64")
else()
set(ARCH_DIR "x86")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/soloud/contrib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp)

set(CLAP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui   
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/stb_image/include
    #${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/soloud/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/entt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/asio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/json/include)

set(CLAP_LINK_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp)

set(CLAP_WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Clap)
add_subdirectory(TestGround)