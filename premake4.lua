if os.get() == "windows" then 
	_ACTION = _ACTION or "vs2010"
elseif os.get() == "linux"then
	_ACTION = _ACTION or "gmake"
end

newoption {
	trigger     = "clincdir",
	value       = "PATH",
	description = "Set path to a directory that contains CL/cl.h"
}

newoption {
	trigger     = "cllibdir",
	value       = "PATH",
	description = "Set path to a directory that contains OpenCL.lib or libOpenCL.so"
}

newoption {
	trigger     = "static",
	value       = "no",
	description = "Build as static library?"
}

newoption {
	trigger     = "opencl12",
	value       = "no",
	description = "Enables OpenCL 1.2 features"
}

_OPTIONS["static"] = _OPTIONS["static"] or "no"
_OPTIONS["opencl12"] = _OPTIONS["opencl12"] or "no"

-- Default paths on windows for AMD APP SDK
if os.get() == "windows" then 
	_OPTIONS["clincdir"] = _OPTIONS["clincdir"] or "$(AMDAPPSDKROOT)/include"
	_OPTIONS["cllibdir"] = _OPTIONS["cllibdir"] or "$(AMDAPPSDKROOT)/lib/x86"
end

-- Common settings per project
local function configureProject()
	configuration "Debug"
		targetsuffix "_d"
		defines { "DEBUG", "_DEBUG", }
		flags { 
			"Symbols", 
			"ExtraWarnings"
		}
		
	configuration "Release"
		defines "NDEBUG"
		flags { 
			"OptimizeSpeed",
			"NoEditAndContinue", 
			"NoFramePointer", 
			"ExtraWarnings"
		}
	
	configuration { "linux", "gmake" }
		buildoptions  "-std=c++11"   
		links "stdc++"
end

solution "clw"
	configurations { "Debug", "Release" }

	-- Available in premake4.4
	if vpaths ~= nil then
		vpaths {
			["Header Files"] = { "**.h" },
			["Source Files"] = { "**.cpp" }
		}
	end
	
	--
	-- Library itself
	--
	
	project "clw"
		language "C++"
		location "proj"
		if _OPTIONS["static"] == "yes" then
			kind "StaticLib"
			defines "CLW_STATIC_LIB"
		else
			kind "SharedLib"
			defines "CLW_BUILD_SHARED"
			links "OpenCL"
		end
        targetdir "bin"
		objdir "obj"
		defines {
			-- needed in AMD's OpenCL headers to enable still used functions (such as clCreateImage2D)
			"CL_USE_DEPRECATED_OPENCL_1_1_APIS", 
			"HAVE_OPENCL_1_1"
		}
		if _OPTIONS["opencl12"] == "yes" then
			defines "HAVE_OPENCL_1_2"
		end
		includedirs { _OPTIONS["clincdir"] }
		libdirs { _OPTIONS["cllibdir"] }
		files { 
			"clw/*.cpp",
			"clw/*.h" 
		}

		configureProject()
		
	--
	-- Simple tool to query basic platform and its devices attributes
	--
		
	project "clwinfo"
		language "C++"
		location "proj"
		kind "ConsoleApp"
		targetdir "bin"
		objdir "obj"
		defines "HAVE_OPENCL_1_1"
		if _OPTIONS["opencl12"] == "yes" then
			defines "HAVE_OPENCL_1_2"
		end
		includedirs { 
			".",
			_OPTIONS["clincdir"]
		}
		files "clwinfo/main.cpp"
		links "clw"
		
		if _OPTIONS["static"] == "yes" then
			defines "CLW_STATIC_LIB"
			links "OpenCL"
			libdirs  { _OPTIONS["cllibdir"] }
		end
		
		configureProject()
