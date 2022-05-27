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
outputdir        = "bin/%{prj.name}/%{cfg.buildcfg}"
objOutputDir     = "bin-int/%{prj.name}/%{cfg.buildcfg}"
sourceFilesPath  = "%{prj.location}/../../src"
projectFilesPath = "projects"

------ Include Directories -------------------------------------------
-- IncludeDir = {}
-- IncludeDir["fmod"]              = "%{prj.name}/Libraries/fmod/include"
-- IncludeDir["FreeImage"]         = "%{prj.name}/Libraries/FreeImage/include"
-- IncludeDir["freetype"]          = "%{prj.name}/Libraries/freetype/include"
-- IncludeDir["glad"]              = "%{prj.name}/Libraries/glad/include"
-- IncludeDir["glm"]               = "%{prj.name}/Libraries/glm/glm"
-- IncludeDir["Mono"]              = "%{prj.name}/Libraries/Mono/include"
-- IncludeDir["rapidjson"]         = "%{prj.name}/Libraries/rapidjson/include"
-- IncludeDir["spdlog"]            = "%{prj.name}/Libraries/spdlog/include"
-- IncludeDir["physx"]             = "%{prj.name}/Libraries/physx/include"
-- IncludeDir["assimp"]            = "%{prj.name}/Libraries/assimp/include"
-- IncludeDir["AwesomeFonts"]      = "%{prj.name}/Libraries/IconFontAwesome"
-- IncludeDir["tracy"]				= "%{prj.name}/Libraries/tracy/"
-- IncludeDir["tinyddsloader"]		= "%{prj.name}/Libraries/tinyddsloader/"

-- IncludeDir["imgui"]             = "PotatoEngine/Libraries/imgui"
-- IncludeDir["imgui_backend"]     = "PotatoEngine/Libraries/imgui/backends"
-- IncludeDir["ImGuizmo"]          = "PotatoEngine/Libraries/ImGuizmo"


------ PotatoEngine -------------------------------------------
project "CS396_ECS_v1"
	location (projectFilesPath)
	kind "WindowedApp"
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
		sourceFilesPath .. "/ECS"
        
        -- External Libraries
		-- "%{IncludeDir.fmod}",
        -- "%{IncludeDir.FreeImage}",
        -- "%{IncludeDir.freetype}",
        -- "%{IncludeDir.glad}",
        -- "%{IncludeDir.glm}",
        -- "%{IncludeDir.Mono}",
        -- "%{IncludeDir.rapidjson}",
        -- "%{IncludeDir.spdlog}",
        -- "%{IncludeDir.AwesomeFonts}",
		-- "%{IncludeDir.tracy}",
		-- "%{IncludeDir.tinyddsloader}"
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

    -- Properties under "All" Configuration
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

    -- Properties under "Debug" Configuration
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		linkoptions{"/NODEFAULTLIB:libcmt.lib"}

    -- Properties under "Release" Configuration
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
        