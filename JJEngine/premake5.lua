workspace "JJEngine"
    platforms {"Win64"}
    configurations  { "Debug", "Release" }    
    startproject "Game"

group "Libs"
include "Libs/glfw"
include "Libs/glad"
include "Libs/imgui"
include "Libs/yaml-cpp"
include "Libs/ImGuizmo"
include "Libs/box2d"
group ""
--inc
IncludeDir={}
IncludeDir["GLFW"]="Libs/glfw/glfw/include/"
IncludeDir["GLAD"]="Libs/glad/"
IncludeDir["GLM"]="Libs/glm/"
IncludeDir["ENTT"]="Libs/entt/"
IncludeDir["IMGUI"]="Libs/imgui/"
IncludeDir["STB_IMAGE"]="Libs/stb_image/"
IncludeDir["MONO"]="Libs/mono/include"
IncludeDir["SPDLOG"]="Libs/spdlog/include"
IncludeDir["FILEWATCH"]="Libs/filewatch/"
IncludeDir["STDUUID"]="Libs/stduuid/include"
IncludeDir["YAML_CPP"]="Libs/yaml-cpp/include"
IncludeDir["IMGUIZMO"]="Libs/ImGuizmo"
IncludeDir["Box2D"]="Libs/box2d/include"
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

group "JJEngine"

include "JJEngine-ScriptCore"

project "JJEngine"
    location "JJEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "c++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"

    flags
    {
        "MultiProcessorCompile"
    }

    links {
        "GLFW",
        "GLAD",
        "opengl32.lib",
        "ImGui",
        "yaml-cpp",
        "ImGuizmo",
        "Box2D",
    }

    disablewarnings {
        "4819","4996","4005"
    }
    linkoptions {}
    includedirs
    {
        "JJEngine",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.STB_IMAGE}",
        "%{IncludeDir.MONO}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.FILEWATCH}",
        "%{IncludeDir.STDUUID}",
        "%{IncludeDir.YAML_CPP}",
        "%{IncludeDir.IMGUIZMO}",
        "%{IncludeDir.Box2D}",
    }
    files {
        "JJEngine/**.h",
        "JJEngine/**.cpp",
        "JJEngine/**.hpp",
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
        defines { "DEBUG", "JJ_DEBUG" }
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
group ""

group "Executable"
project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"

    flags
    {
        "MultiProcessorCompile"
    }

    links {
        "JJEngine",
        "GLAD",
        "opengl32.lib",
    }
    
    disablewarnings {
        "4819","4996","4005"
    }
    linkoptions {}
    includedirs
    {
        "JJEngine",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.FILEWATCH}",
        "%{IncludeDir.STDUUID}",
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
        defines { "DEBUG", "JJ_DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"

    flags
    {
        "MultiProcessorCompile"
    }

    links {
        "JJEngine",
        "GLAD",
        "opengl32.lib",
        "ImGuizmo"
    }
    
    disablewarnings {
        "4819","4996","4005"
    }
    linkoptions {}
    includedirs
    {
        "JJEngine",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.FILEWATCH}",
        "%{IncludeDir.STDUUID}",
        "%{IncludeDir.IMGUIZMO}"
    }
    files {
        "Editor/**.h",
        "Editor/**.cpp"
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
        defines { "DEBUG", "JJ_DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

        
group ""