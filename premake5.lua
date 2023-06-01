-- premake5.lua
workspace "Overlord_Engine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Overlord"
    location "Overlord"
    kind "SharedLib"

    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "oldpch.h"
    pchsource "Overlord/src/oldpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "OLD_PLATFORM_WINDOWS",
            "OLD_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "OLD_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "OLD_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "OLD_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Overlord/vendor/spdlog/include",
        "Overlord/src"
    }

    links
    {
        "Overlord"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "OLD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "OLD_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "OLD_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "OLD_DIST"
        optimize "On"