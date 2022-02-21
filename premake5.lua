workspace "Soren"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}

IncludeDir["Base"] = "./"

-- Set working direcctory to where sandbox exports to or something

project "Soren"
	location "Soren"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	warnings "Extra"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "sorenpch.h"
	pchsource "Soren/src/sorenpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{

	}

	includedirs
	{
		"Soren/Vendor/spdlog/include",
		"Soren/Vendor/json/include",
		"Soren/Vendor/eigen",
		"%{prj.name}/src"
	}

	links
	{

	}

	flags
	{
		"FatalWarnings"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SOREN_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "SOREN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SOREN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SOREN_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	debugdir ("bin/" .. outputdir .. "/%{prj.name}")

	warnings "Extra"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Soren/Vendor/spdlog/include",
		"Soren/Vendor/json/include",
		"Soren/Vendor/eigen",
		"%{prj.name}/src",
		"Soren/src"
	}

	links
	{
		"Soren"
	}

	flags
	{
		"FatalWarnings"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SOREN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SOREN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SOREN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SOREN_DIST"
		runtime "Release"
		optimize "on"