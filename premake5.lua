workspace "Anilab"
	architecture "x64"
	configurations { "Release" }

project "Anilab"
	kind          "ConsoleApp"
	language      "C++"
	cppdialect    "C++17"
	staticruntime "off"

	outputdir = "%{cfg.buildcfg}"

	targetdir ("%{wks.location}/x64/%{cfg.buildcfg}")
	objdir    ("%{wks.location}/x64/%{cfg.buildcfg}")

	includedirs{ "include", "include/dpp", "cores" }
	libdirs    { "lib" }
	links      { "dpp.lib", "sqlite3.lib" }
	files      { 
		"src/**.cpp",
        "src/cmd_common/**.cpp",
		"cores/**.h",
		"Anilab.rc",
        "resource.h"
	}

	filter { "system:windows", "toolset:msc" }
		systemversion "latest"

	filter "configurations:Release"
		runtime  "Release"
		optimize "on"
