cmake_minimum_required(VERSION 3.5)

project(bgTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE bgTest_SRC_FILES "src/*.cpp" 
                                 "src/*.h")


add_compile_definitions(CLAP_USE_BGFX CLAP_USE_SDL2 _CRT_SCURE_NO_WARNINGS)

add_subdirectory(../ Clap)

set(CLAP_USE_BGFX on)
set(bgShaders ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/fs_cubes.sc
              ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/varying.def.sc
              ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/vs_cubes.sc)

CLAP_COMPILE_SHADER(${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/fs_cubes.sc ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/compiled )
CLAP_COMPILE_SHADER(${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/varying.def.sc ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/compiled )
CLAP_COMPILE_SHADER(${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/vs_cubes.sc ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/compiled )

add_executable(bgTest ${bgTest_SRC_FILES} ${bgShaders})

target_include_directories(bgTest SYSTEM PUBLIC ${CLAP_WORKSPACE_DIR}/Clap/src
                                      SYSTEM PUBLIC ${CLAP_INCLUDE_DIRS})

target_link_directories(bgTest PRIVATE ${CLAP_LINK_DIRS})

target_link_libraries(bgTest PUBLIC Clap SDL2main bgfx SDL2-static ImGui yaml-cpp)


