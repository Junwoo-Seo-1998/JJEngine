project "yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/".."%{prj.name}")
    objdir ("bin-int/".."%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		
		"include/**.h"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		flags
		{
			"MultiProcessorCompile"
		}
		cppdialect "C++20"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"