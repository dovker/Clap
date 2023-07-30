#//TODO: PUT THIS INTO THE GITHUB REPO TOO
cmake_minimum_required(VERSION 3.5)

project(ImGui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(ImGui STATIC imgui.cpp 
                         imgui_draw.cpp 
                         imgui_widgets.cpp 
                         imgui_demo.cpp 
                         imconfig.h 
                         imgui.h 
                         imgui_internal.h 
                         imgui_tables.cpp 
                         imstb_rectpack.h 
                         imstb_textedit.h 
                         imstb_truetype.h)