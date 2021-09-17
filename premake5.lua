workspace "Clap"
    architecture "x86_64"

    disablewarnings { "deprecated" }

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}

    IncludeDir["SDL2"] =        "%{wks.location}/Clap/thirdparty/SDL2/include"  
    IncludeDir["Glad"] =        "%{wks.location}/Clap/thirdparty/Glad/include"  
    IncludeDir["ImGui"] =       "%{wks.location}/Clap/thirdparty/imgui"         
    IncludeDir["glm"] =         "%{wks.location}/Clap/thirdparty/glm"           
    IncludeDir["stb_image"] =   "%{wks.location}/Clap/thirdparty/stb_image/include"     
    IncludeDir["SoLoud"] =      "%{wks.location}/Clap/thirdparty/soloud/include"
    IncludeDir["zlib"] =        "%{wks.location}/Clap/thirdparty/zlib"      
    IncludeDir["libzip"] =      "%{wks.location}/Clap/thirdparty/libzip"
    IncludeDir["entt"] =        "%{wks.location}/Clap/thirdparty/entt/include"          
    IncludeDir["asio"] =        "%{wks.location}/Clap/thirdparty/asio/include"          
    IncludeDir["yaml-cpp"] =    "%{wks.location}/Clap/thirdparty/yaml-cpp/include"
    IncludeDir["json"] =        "%{wks.location}/Clap/thirdparty/json/include"

    group "Dependencies"
        include "Clap/thirdparty/SDL2/SDL2.lua"
        include "Clap/thirdparty/soloud/build"
        include "Clap/thirdparty/Glad"
        include "Clap/thirdparty/imgui"
        include "Clap/thirdparty/zlib"
        include "Clap/thirdparty/libzip"
        include "Clap/thirdparty/yaml-cpp"
        filter "system:windows" 
            include "Clap/thirdparty/SDL2/SDL2main.lua"
    group ""

    include "Clap"
    include "TestGround"