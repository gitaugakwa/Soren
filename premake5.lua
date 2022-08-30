workspace "Soren"
	architecture "x86_64"
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
	cppdialect "C++20"
	staticruntime "on"

	warnings "Extra"

	openmp "On"

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
		"Soren/Vendor/oneDNN/cpu_tbb/include",
		"Soren/Vendor/oneMKL/include",
		"Soren/Vendor/oneTBB/include",
		"%{prj.name}/src"
	}

	links
	{
		-- "Soren/Vendor/oneMKL/lib/intel64/mkl_rt.lib",
		-- "Soren/Vendor/oneAPI/windows/lib"
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
	cppdialect "C++20"
	staticruntime "on"

	-- toolset "clang"

	-- vectorextensions "AVX2"

	openmp "On"
	
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
		"Soren/Vendor/oneDNN/cpu_tbb/include",
		"Soren/Vendor/oneMKL/include",
		"Soren/Vendor/oneTBB/include",
		"%{prj.name}/src",
		"Soren/src"
	}

	libdirs
	{
		"Soren/Vendor/oneDNN/cpu_tbb/lib",
		"Soren/Vendor/oneMKL/lib/intel64",
		-- "Soren/Vendor/oneMKL/lib/intel64/**",
		-- "Soren/Vendor/oneMKL/redist/intel64/**",
		"Soren/Vendor/oneTBB/lib/intel64/vc14",
		"Soren/Vendor/oneTBB/redist/intel64/vc14"
	}

	links
	{
		"Soren",
		"dnnl",
		"mkl_rt"
		-- "tbb12",
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

	filter { "Release", "system:windows" }
		defines
		{
			"EIGEN_NO_DEBUG"
		}
		postbuildcommands
		{
			-- ("{COPYDIR} ../Soren/Vendor/oneTBB/redist/intel64/vc14 \"../bin/" .. outputdir .. "/Sandbox/\""),
			("{COPY} ../Soren/Vendor/oneMKL/redist/intel64/mkl_rt.2.dll \"../bin/" .. outputdir .. "/Sandbox/\""),
			("{COPY} ../Soren/Vendor/oneMKL/redist/intel64/mkl_intel_thread.2.dll \"../bin/" .. outputdir .. "/Sandbox/\""),
			-- ("{COPYDIR} ../Soren/Vendor/oneDNN/cpu_tbb/bin \"../bin/" .. outputdir .. "/Sandbox/\"")
		}
		links
		{
			"tbb12"
		}
	filter { "Debug", "system:windows" }
		postbuildcommands
		{
			("{COPYDIR} ../Soren/Vendor/oneTBB/redist/intel64/vc14 \"../bin/" .. outputdir .. "/Sandbox/\""),
			("{COPYDIR} ../Soren/Vendor/oneMKL/redist/intel64 \"../bin/" .. outputdir .. "/Sandbox/\""),
			("{COPYDIR} ../Soren/Vendor/oneDNN/cpu_tbb/bin \"../bin/" .. outputdir .. "/Sandbox/\"")
		}
		links
		{
			"tbb12_debug"
		}
		buildoptions 
		{
			"/bigobj"
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