cmake_minimum_required(VERSION 3.5)

project(wren LANGUAGES C)

file(GLOB_RECURSE WREN_SRC_FILES "src/include/*.c" 
                               "src/include/*.h"  
                               "src/vm/*.c"
                               "src/vm/*.h"  
                               "src/optional/*.c"
                               "src/optional/*.h"  )

add_library(wren STATIC ${WREN_SRC_FILES})

target_include_directories(wren PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include ${CMAKE_CURRENT_SOURCE_DIR}/src/vm ${CMAKE_CURRENT_SOURCE_DIR}/src/optional)