
-- workspace settings
workspace "Glados"
	startproject "Sandbox"

	configurations { "Debug", "Release", "Dist" }
	platforms { "x32" } -- only support Win32 for now

	defines { "GLEW_STATIC" }

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
		runtime "debug"
		symbols "on"

	filter "configurations:Release"
		defines "GD_RELEASE"
		runtime "release"
		optimize "on"

	filter "configurations:Dist"
		defines "GD_DIST"
		runtime "release"
		optimize "on"

	filter {}

-- output dir for all projects
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to workspace
IncludeDir = {}
IncludeDir["GLFW"] = "Glados/vendor/GLFW/include"
IncludeDir["Glad"] = "Glados/vendor/Glad/include"
IncludeDir["ImGui"] = "Glados/vendor/imgui"
IncludeDir["glm"] = "Glados/vendor/glm"
IncludeDir["stb_image"] = "Glados/vendor/stb_image"

include "Glados/vendor/GLFW"
include "Glados/vendor/Glad"
include "Glados/vendor/imgui"

project "Glados"
	location "Glados"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	-- precompiled headers
	pchheader "gladospch.h"
	pchsource "Glados/src/gladospch.cpp"

	links { "ImGui", "GLFW", "Glad", "opengl32" }

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	removefiles
	{
		"%{prj.name}/src/Glados/Archives/**",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/Glados/ObjScripts",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	defines	{ "GLFW_INCLUDE_NONE" }

	filter "configurations:Debug"
		defines "GD_ENABLE_ASSERTS"


project "Glados-Editor"
	location "Glados-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	links { "Glados" }

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Glados/src",
		"Glados/vendor",
		"Glados/vendor/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	links { "Glados" }

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Glados/src",
		"Glados/vendor",
		"Glados/vendor/spdlog/include",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	removefiles 
	{
		"%{prj.name}/src/Archives/**",
	}
