project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin")
    objdir ("bin-int")

    files {
        "**.h",
        "**.cpp"
    }

    includedirs {
        "include",
        "../OmegaEngine/include"
    }

    links { 
        "OmegaEngine"
    }

    filter "system:windows"
        defines { "WINDOWS" }
        postbuildcommands {
            "{COPY} ../OmegaEngine/bin/OmegaEngine.dll %{cfg.targetdir}"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"