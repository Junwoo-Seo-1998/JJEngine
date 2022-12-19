workspace "JJEngine"
    platforms {"Win64"}
    configurations  { "Debug", "Release" }    
    startproject "Game"

include "Libs/glfw"
include "Libs/glad"
include "Libs/imgui"

--inc
IncludeDir={}
IncludeDir["GLFW"]="Libs/glfw/glfw/include/"
IncludeDir["GLAD"]="Libs/glad/"
IncludeDir["GLM"]="Libs/glm/"
IncludeDir["ENTT"]="Libs/entt/"
IncludeDir["IMGUI"]="Libs/imgui/"
IncludeDir["STB_IMAGE"]="Libs/stb_image/"
IncludeDir["MONO"]="Libs/mono/include"
--lib dir
LibraryDir = {}
LibraryDir["Mono_Debug"]="Libs/mono/lib/Debug"
LibraryDir["Mono_Release"]="Libs/mono/lib/Release"
--lib
Library = {}
Library["Mono_Debug"] = "libmono-static-sgen.lib"
Library["Mono_Release"] = "libmono-static-sgen.lib"
--windows lib
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"

project "JJEngine"
    location "JJEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"
    links {
        "GLFW",
        "GLAD",
        "opengl32.lib",
        "ImGui",
    }

    disablewarnings {}
    linkoptions {}
    includedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.STB_IMAGE}",
        "%{IncludeDir.MONO}"
    }
    files {
        "JJEngine/**.h",
        "JJEngine/**.cpp"
    }
    defines{
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    postbuildcommands {

    }

    filter "system:windows"
        linkoptions { "-IGNORE:4006",}
        links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}
    -- All of these settings will appear in the Debug configuration
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        libdirs 
        {
            "%{LibraryDir.Mono_Debug}",
        }
        links
        {
            "%{Library.Mono_Debug}",
        }
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        libdirs 
        {
            "%{LibraryDir.Mono_Release}",
        }
        links
        {
            "%{Library.Mono_Release}",
        }


project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"
    links {
        "JJEngine",
    }
    
    disablewarnings {}
    linkoptions {}
    includedirs
    {
        "JJEngine"
    }
    files {
        "Game/**.h",
        "Game/**.cpp"
    }
    defines{
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    postbuildcommands {
        "{COPYDIR} \"../RuntimeDependencies/Mono\" \"../bin/%{cfg.buildcfg}/Mono\"",
    }

    debugdir "RuntimeDependencies"
    
    -- All of these settings will appear in the Debug configuration
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"