#pragma once

#include "Prerequisites.h"

namespace clw
{
    class Grid
    {
    public:
        Grid();
        Grid(size_t width);
        Grid(size_t width, size_t height);
        Grid(size_t width, size_t height, size_t depth);

        cl_uint dimensions() const { return _dims; }
        size_t width() const { return _sizes[0]; }
        size_t height() const { return _sizes[1]; }
        size_t depth() const { return _sizes[2]; }

        operator const size_t*() const { return _sizes; }

        Grid roundTo(const Grid& range) const;

    private:
        cl_uint _dims;
        size_t _sizes[3];
    };

    inline Grid::Grid()
        : _dims(1)
    {
        _sizes[0] = _sizes[1] = _sizes[2] = 1;
    }

    inline Grid::Grid(size_t width)
        : _dims(1)
    {
        _sizes[0] = width;
        _sizes[1] = _sizes[2] = 0;
    }

    inline Grid::Grid(size_t width, size_t height)
        : _dims(2)
    {
        _sizes[0] = width;
        _sizes[1] = height;
        _sizes[2] = 0;
    }

    inline Grid::Grid(size_t width, size_t height, size_t depth)
        : _dims(3)
    {
        _sizes[0] = width;
        _sizes[1] = height;
        _sizes[2] = depth;
    }
}