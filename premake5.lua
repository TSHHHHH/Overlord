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

    kind "StaticLib"
    staticruntime "On"

    language "C++"
    cppdialect "C++17"

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

    defines
    {
        "_CRT_SECURE_NO_WARNINGS" -- For functions from ImGui (sscanf, strcat, strcpy)
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
        "opengl32.lib"
        
        -- Uncomment these if using dynamic library
        -- "dwmapi.lib",
        -- "gdi32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "OLD_PLATFORM_WINDOWS",
            "OLD_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "OLD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "OLD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "OLD_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"

    kind "ConsoleApp"
    staticruntime "on"

    language "C++"
    cppdialect "C++17"

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
        systemversion "latest"

        defines
        {
            "OLD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "OLD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "OLD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "OLD_DIST"
        runtime "Release"
        optimize "on"