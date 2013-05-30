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
    trigger     = "cllib64dir",
    value       = "PATH",
    description = "Set path to a directory that contains OpenCL.lib or libOpenCL.so for x86_64 architecture"
}

newoption {
    trigger     = "opencl12",
    value       = "no",
    description = "Enables OpenCL 1.2 features"
}

_OPTIONS["opencl12"] = _OPTIONS["opencl12"] or "no"

-- Default paths on windows for AMD APP SDK
if os.get() == "windows" then 
    _OPTIONS["clincdir"] = _OPTIONS["clincdir"] or "$(AMDAPPSDKROOT)/include"
    _OPTIONS["cllibdir"] = _OPTIONS["cllibdir"] or "$(AMDAPPSDKROOT)/lib/x86"
	_OPTIONS["cllib64dir"] = _OPTIONS["cllib64dir"] or "$(AMDAPPSDKROOT)/lib/x86_64"
end

-- Common settings per project
local function configureProject()
	configuration "Debug or DebugLib"
		targetsuffix "_d"
		defines { "DEBUG", "_DEBUG", }
		flags { 
			"Symbols", 
			"ExtraWarnings"
		}
	configuration "Release or ReleaseLib"
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
    configurations { "Debug", "Release", "DebugLib", "ReleaseLib" }
	platforms { "x32", "x64" }

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
        objdir "obj"
		includedirs { _OPTIONS["clincdir"] }  
        files { 
            "clw/*.cpp",
            "clw/*.h" 
        }
        defines {
            -- needed in AMD's OpenCL headers to enable still used functions (such as clCreateImage2D)
            "CL_USE_DEPRECATED_OPENCL_1_1_APIS", 
            "HAVE_OPENCL_1_1"
        }
        if _OPTIONS["opencl12"] == "yes" then
            defines "HAVE_OPENCL_1_2"
        end
		
		-- linker settings for different architectures
		configuration "x32"
			libdirs { _OPTIONS["cllibdir"] }
		configuration "x64"
			libdirs { _OPTIONS["cllib64dir"] }		
			
		-- donfiguration for static builds
		configuration "DebugLib or ReleaseLib"
			kind "StaticLib"
			defines "CLW_STATIC_LIB"

		-- configuration for dll builds
		configuration "Debug or Release"
			kind "SharedLib"
            defines "CLW_BUILD_SHARED"
            links "OpenCL"
			
		-- output directory for different builds and architectures
		configuration { "DebugLib or ReleaseLib", "x32" }
			targetdir "lib"
		configuration { "DebugLib or ReleaseLib", "x64" }
			targetdir "lib64"
		configuration { "Debug or Release", "x32" }
			targetdir "bin"
		configuration { "Debug or Release", "x64" }
			targetdir "bin64"
			
		configureProject()
        
    --
    -- Simple tool to query basic platform and its devices attributes
    --
        
    project "clwinfo"
        language "C++"
        location "proj"
        kind "ConsoleApp"
		objdir "obj"
        files "clwinfo/main.cpp"
        includedirs { 
            ".",
            _OPTIONS["clincdir"]
        }
        defines "HAVE_OPENCL_1_1"
        if _OPTIONS["opencl12"] == "yes" then
            defines "HAVE_OPENCL_1_2"
        end
		links "clw"
        
		configuration "x32"
			targetdir "bin"
		configuration "x64"
			targetdir "bin64"
			
		configuration "DebugLib or ReleaseLib"
            defines "CLW_STATIC_LIB"
			-- need to explicitly link OpenCL
            links "OpenCL"
			configuration "x32"
				libdirs { _OPTIONS["cllibdir"] }
			configuration "x64"
				libdirs { _OPTIONS["cllib64dir"] }       
        configureProject()
		
		configuration "linux"
			linkoptions "-Wl,--rpath=."
