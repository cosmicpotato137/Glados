
-- workspace settings
workspace "Glados"
	configurations { "Debug", "Release", "Dist" }
	platforms { "x32" } -- only support Win32 for now

	defines { "IMGUI_IMPL_OPENGL_LOADER_GLEW", "GLEW_STATIC" }

	filter "system:Windows"
		cppdialect "C++17"
		systemversion "latest"
		defines "GD_PLATFORM_WINDOWS"

	-- filter for platform and configuration
	filter "platforms:x32"
		defines "GD_WIN32"
		system "windows"
		architecture "x32"

	filter "platforms:x64"
		defines "GD_WIN64"
		system "windows"
		architecture "x64"

	filter "configurations:Debug"
		defines "GD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GD_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GD_DIST"
		optimize "On"

	filter {}

-- output dir for all projects
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to workspace
IncludeDir = {}
IncludeDir["GLFW"] = "Glados/vendor/GLFW/include"

include "Glados/vendor/GLFW"

-- include dirs and link libs for opengl
function glSetup()
	includedirs {
		"Dependencies/glew-2.2.0/include"
	}

	links { "glew32s", "opengl32" }
	libdirs { "Dependencies/glew-2.2.0/lib/Release/Win32" }
end


project "Glados"
	location "Glados"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	-- precompiled headers
	pchheader "gladospch.h"
	pchsource "Glados/src/gladospch.cpp"

	glSetup()

	defines "SPDLOG_COMPILED_LIB"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/imgui/**.h",
		"%{prj.name}/vendor/imgui/**.cpp"
	}

	removefiles
	{
		"%{prj.name}/src/Archives/**",
		"%{prj.name}/src/Tests/**",
		"%{prj.name}/src/Application.cpp",
		"%{prj.name}/vendor/imgui/imgui/misc/**.h",
		"%{prj.name}/vendor/imgui/imgui/misc/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/ObjScripts",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/vendor/imgui",
		"%{IncludeDir.GLFW}"
	}

	links "GLFW"

	filter "system:windows"
		staticruntime "off"
		defines	"GD_BUILD_DLL"

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "Sandbox")
		}


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	glSetup()
	links "Glados"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	removefiles
	{
		"Glados/vendor/imgui/misc/**.cpp",
		"Glados/vendor/imgui/misc/**.h"
	}

	includedirs
	{
		"Glados/src",
		"Glados/vendor"
	}

	filter "system:windows"
		staticruntime "On"