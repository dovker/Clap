project "Clap"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.cpp",
        "src/**.h",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
    }
    includedirs
    {
        "src",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.SoLoud}",
        "%{IncludeDir.zlib}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.json}",
        "%{IncludeDir.libzip}",
        "%{IncludeDir.asio}",
    }
    filter "action:xcode4"
        pchheader "src/pch.h"
        pchsource "src/pch.cpp"

        xcodebuildsettings = { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }
        sysincludedirs
        {
            "src",
            "%{IncludeDir.SDL2}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.SoLoud}",
            "%{IncludeDir.zlib}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.yaml_cpp}",
            "%{IncludeDir.json}",
            "%{IncludeDir.libzip}",
            "%{IncludeDir.asio}",
        }
    filter {}
    
    filter "system:macosx"
        links
        {
            "Cocoa.framework",
            "IOKit.framework",
            "QuartzCore.framework",
            "AudioToolbox.framework",
            "AudioUnit.framework",
            "CoreAudio.framework",
            "CoreFoundation.framework"
        }
    filter {}
    
    links
    {
        "SLD2",
        "Glad",
        "ImGui",
        "SoloudStatic",
        "zlib",
        "libzip",
        "yaml-cpp",
        "opengl32.lib"
    }
    defines
    {
        "_CRT_SCURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
    }
    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        systemversion "latest"


    filter "configurations:Debug"
        defines "CLAP_DEBUG"
        runtime "Debug"
        symbols "On"
    filter "configurations:Release"
        defines "CLAP_RELEASE"
        runtime "Release"
        optimize "On"
    filter "configurations:Distribution"
        defines "CLAP_DIST"
        runtime "Release"
        optimize "On"

    configuration "vs*"
        buildoptions { "/Zm250", "/bigobj" }