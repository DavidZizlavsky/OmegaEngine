project "OmegaEngine"
    kind "SharedLib"
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
        "vendor/GLFW/include",
        "vendor/GLEW/include",
        "vendor/GLM/include"
    }

    links { 
        "vendor/GLFW/lib-vc2022/glfw3.lib", 
        "vendor/GLEW/lib/Release/x64/glew32s.lib" 
    }

    defines { "OMEGA_ENGINE_DLL" }

    filter "system:windows"
        defines { "WINDOWS" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"