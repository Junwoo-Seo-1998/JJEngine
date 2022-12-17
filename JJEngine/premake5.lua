workspace "JJEngine"
    platforms {"Win64"}
    configurations  { "Debug", "Release" }    
    startproject "Game"

IncludeDir={}



project "JJEngine"
    location "JJEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"
    links {

    }

    disablewarnings {}
    linkoptions {}
    includedirs
    {
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


    -- All of these settings will appear in the Debug configuration
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

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
    
    }
    
    
    -- All of these settings will appear in the Debug configuration
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"