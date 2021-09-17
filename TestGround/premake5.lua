project "TestGround"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.cpp",
        "src/**.hpp",
        "src/**.h",
        "src/**.c",
    }

    includedirs
    {
        "%{wks.location}/Clap/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.json}",
        --TEMPORARY, REMOVE AFTER ABSTRACTIONS
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.GLAD}"
    }
    links
    {
        "Clap",
        "ImGui",
        "yaml-cpp",
        --TEMPORARY, REMOVE AFTER ABSTRACTIONS
        "SDL2",
        "Glad"
    }

    filter "action:xcode4"
        xcodebuildsettings = { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }
        sysincludedirs
        {
            "%{wks.location}/Clap/src",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.yaml_cpp}",
            "%{IncludeDir.json}",
            --TEMPORARY, REMOVE AFTER ABSTRACTIONS
            "%{IncludeDir.SDL2}",
            "%{IncludeDir.GLAD}"
        }

    filter "system:windows"
        systemversion "latest"


    filter "system:linux"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        runtime "Release"
        optimize "On"