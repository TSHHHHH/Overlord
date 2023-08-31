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
IncludeDir["GLFW"]          = "Overlord/vendor/GLFW/include"
IncludeDir["Glad"]          = "Overlord/vendor/Glad/include"
IncludeDir["ImGui"]         = "Overlord/vendor/imgui"
IncludeDir["glm"]           = "Overlord/vendor/glm"
IncludeDir["stb_image"]     = "Overlord/vendor/stb_image"

group "Library"
    --include "Overlord/vendor/GLFW"
    project "GLFW"
        location "Overlord/vendor/GLFW"

        kind "StaticLib"

        language "C"

        targetdir ("Overlord/vendor/GLFW/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("Overlord/vendor/GLFW/bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "Overlord/vendor/GLFW/include/**.h",
            "Overlord/vendor/GLFW/src/**.h",
            "Overlord/vendor/GLFW/src/**.c"
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "on"

            files
            {
                "Overlord/vendor/GLFW/src/**.c"
            }

            defines
            {
                "_GLFW_WIN32",
                "_CRT_SECURE_NO_WARNINGS"
            }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

    --include "Overlord/vendor/Glad"
    project "Glad"
        location "Overlord/vendor/Glad"

        kind "StaticLib"
        staticruntime "on"

        language "C"

        targetdir ("Overlord/vendor/Glad/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("Overlord/vendor/Glad/bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "Overlord/vendor/Glad/include/glad/glad.h",
            "Overlord/vendor/Glad/include/KHR/khrplatform.h",
            "Overlord/vendor/Glad/src/glad.c"
        }

        includedirs
        {
            "Overlord/vendor/Glad/include"
        }

        filter "system:windows"
            systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"
    --include "Overlord/vendor/imgui"
    project "ImGui"
        location "Overlord/vendor/imgui"

        kind "StaticLib"
        staticruntime "on"
        
        language "C++"
        cppdialect "C++17"

        targetdir ("Overlord/vendor/imgui/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("Overlord/vendor/imgui/bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            -- "Overlord/vendor/imgui/imconfig.h",
            -- "Overlord/vendor/imgui/imgui.h",
            -- "Overlord/vendor/imgui/imgui.cpp",
            -- "Overlord/vendor/imgui/imgui_draw.cpp",
            -- "Overlord/vendor/imgui/imgui_internal.h",
            -- "Overlord/vendor/imgui/imgui_widgets.cpp",
            -- "Overlord/vendor/imgui/imgui_tables.cpp",
            -- "Overlord/vendor/imgui/imstb_rectpack.h",
            -- "Overlord/vendor/imgui/imstb_textedit.h",
            -- "Overlord/vendor/imgui/imstb_truetype.h",
            -- "Overlord/vendor/imgui/imgui_demo.cpp"
            "Overlord/vendor/imgui/*.h",
            "Overlord/vendor/imgui/*.cpp"
        }

        filter "system:windows"
            systemversion "latest"

        filter "system:linux"
            pic "On"
            systemversion "latest"
            cppdialect "C++17"
            staticruntime "On"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"
group ""

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
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
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
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
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

project "Overlord-Editor"
    location "Overlord-Editor"

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
