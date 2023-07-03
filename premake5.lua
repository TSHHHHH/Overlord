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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]  = "Overlord/vendor/GLFW/include"
IncludeDir["Glad"]  = "Overlord/vendor/Glad/include"
IncludeDir["ImGui"] = "Overlord/vendor/imgui"
IncludeDir["glm"]   = "Overlord/vendor/glm"

include "Overlord/vendor/GLFW"
include "Overlord/vendor/Glad"
include "Overlord/vendor/imgui"

project "Overlord"
    location "Overlord"
    kind "SharedLib"

    language "C++"

    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "oldpch.h"
    pchsource "Overlord/src/oldpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "dwmapi.lib",
        "gdi32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "OLD_PLATFORM_WINDOWS",
            "OLD_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPYDIR} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
        }

    filter "configurations:Debug"
        defines "OLD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "OLD_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "OLD_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"

    staticruntime "off"

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
        "Overlord/src",
        "Overlord/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Overlord"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "OLD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "OLD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "OLD_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "OLD_DIST"
        runtime "Release"
        optimize "On"