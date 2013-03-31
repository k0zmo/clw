#pragma once

#include "config.h"

#include <CL/cl.h>
#include <CL/cl_ext.h>
/// TODO: Not yet used
#include <CL/cl_gl.h>

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

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
    class Grid;
    class Event;
    class UserEvent;
    class EventList;
    class Sampler;

    CLW_EXPORT int compiledOpenCLVersion();
}
