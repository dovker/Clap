cmake_minimum_required(VERSION 3.5)

project(libzip LANGUAGES C)

file(GLOB_RECURSE LIBZIP_SRC_FILES "*.c" "*.h")

add_library(libzip STATIC ${LIBZIP_SRC_FILES})
#target_include_directories(libzip PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions(N_FSEEKO)

if(UNIX AND NOT APPLE)
    add_compile_definitions(HAVE_SSIZE_T_LIBZIP HAVE_CONFIG_H)
endif()

if(WIN32)
    add_compile_definitions(_WINDOWS)
endif()

if(MSVC)
    target_include_directories(hello PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vs)
endif()

if(WIN32 AND MINGW)
    add_compile_definitions(HAVE_SSIZE_T_LIBZIP)
endif()

if(APPLE)
    add_compile_definitions(HAVE_SSIZE_T_LIBZIP)
endif()