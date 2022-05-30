cmake_minimum_required(VERSION 3.3)
project(lua VERSION 5.3.2 LANGUAGES C CXX)

file(GLOB_RECURSE LUA_SRC_FILES "*.c" "*.h")

add_library(lua-lib STATIC ${LUA_SRC_FILES})
set_property(TARGET lua-lib PROPERTY OUTPUT_NAME lua)

add_executable(lua-bin src/lua.c)
set_property(TARGET lua-bin PROPERTY OUTPUT_NAME lua)
target_link_libraries(lua-bin lua-lib)

add_executable(luac src/luac.c)
target_link_libraries(luac lua-lib)
