workspace "NES"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NES"
	location "NES"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
	}

	filter "system:windows"
		cppdialect "C++23"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GE_PLATFORM_WINDOWS",
			"GE_BUILD_DLL"
		}

		filter "configurations:Debug"
			defines "GE_DEBUG"
			symbols "On"
		
		filter "configurations:Debug"
			defines "GE_RELEASE"
			optimize "On"

		filter "configurations:Dist"
			defines "GE_DIST"
			optimize "On"
