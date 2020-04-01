
hazelClientLocation = debug.getinfo(1).source:match("@?(.*/)")

include "vendor/Hazel"

HazelWorkspace("KSP")
SetupHazel()

include "vendor/Calc/project_premake5.lua"

project "KSP"

	files
	{
		"src/**",
	}

	includedirs
	{
		"src",
		"vendor/krpc-cpp/include",
		"vendor/krpc-cpp/protobuf",
		"vendor/protobuf/src",
		"vendor/asio/include",
		"vendor/glm",
	}

	defines
	{

	}

	links
	{
		"protobuf",
		"krpc"
	}

	HazelExecutable()

project "protobuf"
	kind "StaticLib"
	location "vendor/protobuf"

	files
	{
		"vendor/protobuf/src/**.h",
		"vendor/protobuf/src/**.cc",
	}

	excludes
	{
		"vendor/protobuf/src/google/protobuf/compiler/**",
		"vendor/protobuf/src/google/protobuf/testing/**",
		"vendor/protobuf/src/google/protobuf/testdata/**",
		"vendor/protobuf/src/**test.cc",
		"vendor/protobuf/src/**test_*",
	}

	includedirs
	{
		"vendor/protobuf/src",
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


project "krpc"
	kind "StaticLib"
	location "vendor/krpc-cpp"

	files
	{
		"vendor/krpc-cpp/src/**",
		"vendor/krpc-cpp/protobuf/**",
	}

	excludes
	{
		"vendor/krpc-cpp/src/google/protobuf/compiler/main.cc",
	}

	includedirs
	{
		"vendor/krpc-cpp/include",
		"vendor/krpc-cpp/protobuf",
		"vendor/protobuf/src",
		"vendor/asio/include",
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

