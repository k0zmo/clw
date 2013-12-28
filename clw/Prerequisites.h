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

    class Point
    {
    public:
        Point()
            : x(0), y(0), z(0) {}
        Point(size_t x, size_t y, size_t z = 0) 
            : x(x), y(y), z(z) {}

        const size_t* origin() const { return &x; }

        size_t x;
        size_t y;
        size_t z;
    };

    class Rect
    {
    public:
        Rect()
            : x(0), y(0), z(0), width(0), height(0), depth(1) {}
        Rect(size_t x, size_t y, size_t width, size_t height) 
            : x(x), y(y), z(0), width(width), height(height), depth(1) {}
        Rect(size_t x, size_t y, size_t z, size_t width, size_t height, size_t depth) 
            : x(x), y(y), z(z), width(width), height(height), depth(depth) {}

        const size_t* origin() const { return &x; };
        const size_t* region() const { return &width; };

        size_t x;
        size_t y;
        size_t z;
        size_t width;
        size_t height;
        size_t depth;
    };

    CLW_EXPORT int compiledOpenCLVersion();
}
