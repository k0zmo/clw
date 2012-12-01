#pragma once

#include <CL/cl.h>
#include <CL/cl_ext.h>
#include <CL/cl_gl.h>

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

#ifdef _MSC_VER
// nonstandard extension used: enum used in qualified name
#pragma warning(disable:4482)
#endif

#define HAVE_OPENCL_1_1
//#define HAVE_OPENCL_1_2

namespace clw
{
	using std::vector;
	using std::string;
	using std::function;

	class Platform;
	class Device;
	class Context;
	class CommandQueue;
	class Program;
	class Kernel;
	class MemoryObject;
	class Buffer;
	struct ImageFormat;
	class Image2D;
	class Image3D;
	class NDRange;
	class Event;
	class UserEvent;
	class EventList;
	class Sampler;

	inline int compiledOpenCLVersion()
	{
#if defined(HAVE_OPENCL_1_2)
		return 120;
#elif defined(HAVE_OPENCL_1_1)
		return 110;
#else
		return 100;
#endif
	}

	namespace detail
	{
		void reportError(const char* name, cl_int eid);
		bool supportsExtension(const string& list, const char* ext);
		vector<string> tokenize(const string& str, char delim, char group = 0);
		void trim(string* str, bool left, bool right);
		bool readAsString(const string& filename, string* contents);
	}
}
