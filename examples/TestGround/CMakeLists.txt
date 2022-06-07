cmake_minimum_required(VERSION 3.5)

project(TestGround LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE TG_SRC_FILES "src/*.cpp" 
                                 "src/*.h")

add_executable(TestGround ${TG_SRC_FILES})

target_include_directories(TestGround PUBLIC ${CLAP_WORKSPACE_DIR}/Clap/src
                                      PUBLIC ${CLAP_INCLUDE_DIRS})

add_compile_definitions(_CRT_SCURE_NO_WARNINGS)

target_link_directories(TestGround PRIVATE ${CLAP_LINK_DIRS})

target_link_libraries(TestGround PUBLIC Clap SDL2main SDL2-static ImGui yaml-cpp)


