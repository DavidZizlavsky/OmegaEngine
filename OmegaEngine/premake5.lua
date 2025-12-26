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

    libdirs {
        "vendor/GLFW/lib-vc2022",
        "vendor/GLEW/lib/Release/x64"
    }

    links { 
        "glew32s.lib",
        "glfw3.lib",
        "opengl32.lib"
    }

    defines { "OMEGA_ENGINE_DLL", "GLEW_STATIC" }

    filter "system:windows"
        defines { "WINDOWS" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"