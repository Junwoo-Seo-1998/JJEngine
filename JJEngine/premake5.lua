workspace "JJEngine"
    platforms {"Win64"}
    configurations  { "Debug", "Release" }    
    startproject "Game"

group "Libs"
include "Libs/glfw"
include "Libs/glad"
include "Libs/imgui"
include "Libs/yaml-cpp"
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
project "JJEngine"
    location "JJEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "c++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"
    links {
        "GLFW",
        "GLAD",
        "opengl32.lib",
        "ImGui",
        "yaml-cpp",
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
    }
    files {
        "JJEngine/**.h",
        "JJEngine/**.cpp",
        "JJEngine/**.hpp"
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

group "JJEngine"
project "JJEngine-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8"
    location "JJEngine-ScriptCore"
    targetdir ("RuntimeDependencies/Resources/Scripts")
    --objdir ("..Intermediates")

    files 
    {
        "JJEngine-ScriptCore/**.cs",
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

    filter "configurations:Dist"
        optimize "Full"
        symbols "Off"
    

group "Executable"
project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"
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