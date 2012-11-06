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

_OPTIONS["clincdir"] = _OPTIONS["clincdir"] or "$(AMDAPPSDKROOT)/include"

solution "clw"
	configurations { "Debug", "Release" }
    project "clw"
    	language "C++"
	    location "proj"
    	kind "StaticLib"
		targetdir "lib"
		objdir "obj"
		defines "CL_USE_DEPRECATED_OPENCL_1_1_APIS"
		
		includedirs { _OPTIONS["clincdir"] }
     	
    	files { "clw/*.cpp", "clw/*.h" }

	configuration "Debug"
		targetsuffix "_d"
		defines { "DEBUG", "_DEBUG", }
		flags "Symbols"
	
	configuration "Release"
		defines "NDEBUG"	
		flags { "OptimizeSpeed", "NoEditAndContinue", "NoFramePointer" }
	
	configuration { "linux", "gmake" }
		buildoptions { "-std=c++0x" }    