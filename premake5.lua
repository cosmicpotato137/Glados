
-- workspace settings
workspace "Glados"
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
IncludeDir["ImGui"] = "Glados/vendor/imgui"

include "Glados/vendor/GLFW"
include "Glados/vendor/imgui"

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

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	removefiles
	{
		"%{prj.name}/src/Archives/**",
		"%{prj.name}/src/Tests/**",
		"%{prj.name}/src/Glados/Application.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/Glados/ObjScripts",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/stb_image",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}"
	}

	links { "GLFW", "ImGui" }

	filter "system:windows"
		staticruntime "off"
		defines	"GD_BUILD_DLL"

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "Sandbox")
		}

	filter "configurations:Debug"
		defines "GD_ENABLE_ASSERTS"


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
		"%{prj.name}/src/Glados/ObjScripts",
		"Glados/vendor",
		"Glados/vendor/spdlog/include",
		"Glados/vendor/glm",
		"Glados/vendor/stb_image"
	}

	filter "system:windows"
		staticruntime "On"