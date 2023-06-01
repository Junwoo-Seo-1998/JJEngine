--[[
Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: premake5.lua
Purpose: script to generate visual studio solution file.
Language: lua - used by premake5
Platform: premake5 Windows
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------
--]]
project "GLFW"
    kind "StaticLib"
    language "C"

    targetdir ("bin/".."%{prj.name}")
    objdir ("bin-int/".."%{prj.name}")

    files
    {
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/internal.h",
        "glfw/src/platform.h",
        "glfw/src/mappings.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/platform.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c",
        "glfw/src/egl_context.c",
        "glfw/src/osmesa_context.c",
        "glfw/src/null_platform.h",
        "glfw/src/null_joystick.h",
        "glfw/src/null_init.c",
        "glfw/src/null_monitor.c",
        "glfw/src/null_window.c",
        "glfw/src/null_joystick.c",

    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        disablewarnings { "4819", "4002", "4005" }
        
        files
        {
            "glfw/src/win32_init.c",
            "glfw/src/win32_module.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_time.h",
            "glfw/src/win32_time.c",
            "glfw/src/win32_thread.h",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
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