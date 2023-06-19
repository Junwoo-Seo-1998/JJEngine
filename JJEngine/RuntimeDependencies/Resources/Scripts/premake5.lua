--need this for vs2022

--[[
require "vstudio"

local function platformsElement(cfg)
  _p(2,'<Platforms>AnyCPU</Platforms')
end

premake.override(premake.vstudio.cs2005.elements, "projectProperties", function (oldfn, cfg)
  return table.join(oldfn(cfg), {
    platformsElement,
  })
end)
]]--

--need this for vs2022

local RootDir = '../../../'
workspace "GameScript"
    architecture "x86_64"
    startproject "GameScript"

    configurations
	{
		"Debug",
		"Release"
	}


project "GameScript"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8"
    targetdir ("./")
    objdir ("Intermediates")

    files 
    {
        "Source/**.cs",
    }

    links
	{
		"JJEngine-ScriptCore"
	}

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "Full"
        symbols "Off"



group "JJEngine"
	include (RootDir .. "JJEngine-ScriptCore")  
group ""  