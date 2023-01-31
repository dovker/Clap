cmake_minimum_required(VERSION 3.5)

project(TestGround LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE TG_SRC_FILES "src/*.cpp" 
                                 "src/*.h")

add_executable(TestGround ${TG_SRC_FILES})

add_compile_definitions(CLAP_USE_OPENGL_3_3 CLAP_USE_OPENGL CLAP_USE_SDL2 _CRT_SCURE_NO_WARNINGS)

add_subdirectory(../../ Clap)

target_include_directories(TestGround SYSTEM PUBLIC ${CLAP_WORKSPACE_DIR}/Clap/src
                                      SYSTEM PUBLIC ${CLAP_INCLUDE_DIRS})

target_link_directories(TestGround PRIVATE ${CLAP_LINK_DIRS})

target_link_libraries(TestGround PUBLIC Clap SDL2main SDL2-static ImGui yaml-cpp)


