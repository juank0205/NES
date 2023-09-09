workspace "NES"
architecture "x64"

configurations { "Debug", "Release", "Dist" }

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
system "windows"
cppdialect "C++17"
staticruntime "On"
systemversion "latest"

filter "system:Unix"
system "linux"
cppdialect "C++17"
staticruntime "On"
systemversion "latest"

filter "configurations:Debug"
defines "DEBUG"
symbols "On"

filter "configurations:Debug"
defines "RELEASE"
optimize "On"

filter "configurations:Dist"
defines "DIST"
optimize "On"
