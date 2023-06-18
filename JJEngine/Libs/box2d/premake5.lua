project "Box2D"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

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
		"include",
		"src"
	}

	flags
    {
        "MultiProcessorCompile"
    }

	filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"