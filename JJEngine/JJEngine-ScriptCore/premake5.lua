project "JJEngine-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8"
    location "JJEngine-ScriptCore"
    targetdir ("../RuntimeDependencies/Resources/Scripts")
    objdir ("Intermediates")

    files 
    {
        "JJEngine/**.cs",
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "Full"
        symbols "Off"
