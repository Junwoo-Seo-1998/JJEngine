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
project "GLAD"
    kind "StaticLib"
    language "C"

    targetdir ("bin/".."%{prj.name}")
    objdir ("bin-int/".."%{prj.name}")
    staticruntime "On"
    systemversion "latest"
    
    files{
        "**.h",
        "**.c"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"