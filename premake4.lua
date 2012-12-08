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

_OPTIONS["clincdir"] = _OPTIONS["clincdir"] or "$(AMDAPPSDKROOT)/include"
_OPTIONS["cllibdir"] = _OPTIONS["cllibdir"] or "$(AMDAPPSDKROOT)/lib/x86"

solution "clw"
	configurations { "Debug", "Release" }
	
	--
	-- Library itself
	--
	
	project "clw"
		language "C++"
		location "proj"
		kind "StaticLib"
		targetdir "lib"
		objdir "obj"
		defines "CL_USE_DEPRECATED_OPENCL_1_1_APIS"
		includedirs { _OPTIONS["clincdir"] }
		-- libdirs  { _OPTIONS["cllibdir"] }
		files { "clw/*.cpp", "clw/*.h" }

	configuration "Debug"
		targetsuffix "_d"
		defines { "DEBUG", "_DEBUG", }
		flags { "Symbols", "ExtraWarnings" }
	
	configuration "Release"
		defines "NDEBUG"
		flags { "OptimizeSpeed", "NoEditAndContinue", "NoFramePointer", "ExtraWarnings" }
	
	configuration { "linux", "gmake" }
		buildoptions "-std=c++11" 
		
	--
	-- Simple tool to query basic platform and its devices attributes
	--
		
	project "clwinfo"
		language "C++"
		location "proj"
		kind "ConsoleApp"
		targetdir "bin"
		objdir "obj"
		defines "CL_USE_DEPRECATED_OPENCL_1_1_APIS"
		includedirs { _OPTIONS["clincdir"], "." }
		libdirs  { _OPTIONS["cllibdir"] }
		files "clwinfo/main.cpp"
		links { "clw", "OpenCL" }
		
	configuration "Debug"
		targetsuffix "_d"
		defines { "DEBUG", "_DEBUG", }
		flags { "Symbols", "ExtraWarnings" }
		
	configuration "Release"
		defines "NDEBUG"
		flags { "OptimizeSpeed", "NoEditAndContinue", "NoFramePointer", "ExtraWarnings" }
	
	configuration { "linux", "gmake" }
		buildoptions  "-std=c++11"   
