project "JJEngine-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8"
    location "JJEngine-ScriptCore"
    targetdir ("../RuntimeDependencies/Resources/Scripts")
    objdir ("../bin/%{cfg.buildcfg}-obj")

    files 
    {
        "JJEngine/**.cs",
        "**.cs",
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "Full"
        symbols "Off"
