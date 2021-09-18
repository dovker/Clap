cmake_minimum_required(VERSION 3.5)

project(zlib LANGUAGES C)

file(GLOB_RECURSE ZLIB_SRC_FILES "*.c" "*.h")
add_library(zlib STATIC ${ZLIB_SRC_FILES})
#target_include_directories(zlib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions(N_FSEEKO)

if(WIN32)
    add_compile_definitions(_WINDOWS WIN32)
else()
    add_compile_definitions(HAVE_UNISTD_H unix)
endif()
