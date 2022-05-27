workspace "CS396_ECS"           -- Solution Name
    location "projects"         -- Create Solution in "PotatoEngine" folder
	architecture "x64"          -- build for 64 bits systems
	startproject "CS396_ECS_v1" -- start with this project

	configurations
	{
		"Debug",
		"Release"
	}

-- Type Aliases
outputdir        	= "bin/%{prj.name}/%{cfg.buildcfg}"
objOutputDir     	= "bin-int/%{prj.name}/%{cfg.buildcfg}"
sourceFilesPath  	= "%{prj.location}/../../src"
projectFilesPath 	= "projects"
depFilePath			= "dependencies"

------ Include Directories -------------------------------------------
IncludeDir = {}
IncludeDir["glut"] 	= "%{prj.location}/../../dependencies/glut/include"

------ PotatoEngine --------------------------------------------------
project "CS396_ECS_v1"
	location (projectFilesPath)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
    warnings "Extra"
	staticruntime "on"
    characterset "MBCS"
	editAndContinue "Off"

    targetdir ( outputdir )
    objdir    ( objOutputDir )

    -- Adding Files into project
	files
	{
		sourceFilesPath .. "/**.h",
        sourceFilesPath .. "/**.hpp",
		sourceFilesPath .. "/**.cpp"
	}
    
    -- Adding Filters into project (to be edited)
    vpaths
    {
        ["*"] = { "**.h", "**.hpp", "**.cpp"}
    }

    -- Include Directories under C/C++ General
	includedirs
	{
        -- Internal Source Files
		sourceFilesPath .. "/ECS",
        
        -- External Libraries
		"%{IncludeDir.glut}"
	}
    
    -- Prepocessor defines
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX"
	}
	
    -- Build Flags for compiling / linking
	flags
	{
		"MultiProcessorCompile"
	}

	libdirs
	{ 
		
	}

	links
	{

	}

    -- Properties under "All" Configuration
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		disablewarnings "4201"

    -- Properties under "Debug" Configuration
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		linkoptions{"/NODEFAULTLIB:libcmt.lib"}

    -- Properties under "Release" Configuration
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
        