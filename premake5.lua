
hazelClientLocation = debug.getinfo(1).source:match("@?(.*/)")

include "vendor/Hazel"

HazelWorkspace("KSP", "KSP")
SetupHazel()

project "KSP"

	files
	{
		"src/**",
	}

	includedirs
	{
		"src",
	}

	defines
	{

	}

	HazelExecutable()
