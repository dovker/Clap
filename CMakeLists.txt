cmake_minimum_required(VERSION 3.5)
project(ClapWorkspace)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "x64")
else()
set(ARCH_DIR "x86")
endif()

set(SDL_SENSORS OFF CACHE BOOL "Enabling SDL Sensors")
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "DISABLIN BGFX EXAMPLES")
set(ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)


add_compile_definitions(_CRT_SCURE_NO_WARNINGS)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/soloud/contrib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/bgfx)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/lua)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/bgfx/bgfx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/bgfx/bimg/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/bgfx/bx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui   
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/stb_image/include
    #${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/soloud/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib
    # ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/asio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/sol
    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/lua/src)

set(CLAP_LINK_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/SDL
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/Glad
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/bgfx
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/imgui
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/zlib
                #    ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/libzip
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/yaml-cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/Clap/thirdparty/lua)

set(CLAP_WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function( CLAP_COMPILE_SHADER FILE FOLDER )
    if(CLAP_USE_BGFX)
        get_filename_component( FILENAME "${FILE}" NAME_WE )
        string( SUBSTRING "${FILENAME}" 0 2 TYPE )
        if( "${TYPE}" STREQUAL "fs" )
            set( TYPE "FRAGMENT" )
            set( D3D_PREFIX "ps" )
        elseif( "${TYPE}" STREQUAL "vs" )
            set( TYPE "VERTEX" )
            set( D3D_PREFIX "vs" )
        elseif( "${TYPE}" STREQUAL "cs" )
            set( TYPE "COMPUTE" )
            set( D3D_PREFIX "cs" )
        else()
            set( TYPE "" )
        endif()

        if( NOT "${TYPE}" STREQUAL "" )
            set( COMMON FILE ${FILE} ${TYPE} INCLUDES ${BGFX_DIR}/src )
            set( OUTPUTS "" )
            set( OUTPUTS_PRETTY "" )

            if( WIN32 )
                # dx9
                if( NOT "${TYPE}" STREQUAL "COMPUTE" )
                    set( DX9_OUTPUT ${FOLDER}/dx9/${FILENAME}.bin )
                    shaderc_parse( DX9 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_3_0 O 3 OUTPUT ${DX9_OUTPUT} )
                    list( APPEND OUTPUTS "DX9" )
                    set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX9, " )
                endif()

                # dx11
                set( DX11_OUTPUT ${FOLDER}/dx11/${FILENAME}.bin )
                if( NOT "${TYPE}" STREQUAL "COMPUTE" )
                    shaderc_parse( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 3 OUTPUT ${DX11_OUTPUT} )
                else()
                    shaderc_parse( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 1 OUTPUT ${DX11_OUTPUT} )
                endif()
                list( APPEND OUTPUTS "DX11" )
                set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX11, " )
            endif()

            if( APPLE )
                # metal
                set( METAL_OUTPUT ${FOLDER}/metal/${FILENAME}.bin )
                shaderc_parse( METAL ${COMMON} OSX PROFILE metal OUTPUT ${METAL_OUTPUT} )
                list( APPEND OUTPUTS "METAL" )
                set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, " )
            endif()

            # essl
            if( NOT "${TYPE}" STREQUAL "COMPUTE" )
                set( ESSL_OUTPUT ${FOLDER}/essl/${FILENAME}.bin )
                shaderc_parse( ESSL ${COMMON} ANDROID OUTPUT ${ESSL_OUTPUT} )
                list( APPEND OUTPUTS "ESSL" )
                set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}ESSL, " )
            endif()

            # glsl
            set( GLSL_OUTPUT ${FOLDER}/glsl/${FILENAME}.bin )
            if( NOT "${TYPE}" STREQUAL "COMPUTE" )
                shaderc_parse( GLSL ${COMMON} LINUX PROFILE 120 OUTPUT ${GLSL_OUTPUT} )
            else()
                shaderc_parse( GLSL ${COMMON} LINUX PROFILE 430 OUTPUT ${GLSL_OUTPUT} )
            endif()
            list( APPEND OUTPUTS "GLSL" )
            set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}GLSL, " )

            # spirv
            if( NOT "${TYPE}" STREQUAL "COMPUTE" )
                set( SPIRV_OUTPUT ${FOLDER}/spirv/${FILENAME}.bin )
                shaderc_parse( SPIRV ${COMMON} LINUX PROFILE spirv OUTPUT ${SPIRV_OUTPUT} )
                list( APPEND OUTPUTS "SPIRV" )
                set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}SPIRV" )
                set( OUTPUT_FILES "" )
                set( COMMANDS "" )
            endif()

            set( COMMANDS ${DX11})

            foreach( OUT ${OUTPUTS} )
                list( APPEND OUTPUT_FILES ${${OUT}_OUTPUT} )
                list( APPEND COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${${OUT}} )
                get_filename_component( OUT_DIR ${${OUT}_OUTPUT} DIRECTORY )
                file( MAKE_DIRECTORY ${OUT_DIR} )
            endforeach()
            
            
            add_custom_command(
                MAIN_DEPENDENCY
                ${FILE}
                OUTPUT
                ${OUTPUT_FILES}
                ${COMMANDS}
                COMMENT "Compiling shader ${FILE} for ${OUTPUTS_PRETTY}"
            )
        endif()
    endif()
endfunction()

add_subdirectory(Clap)