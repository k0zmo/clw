/*
    Copyright (c) 2012, 2013 Kajetan Swierk <k0zmo@outlook.com>
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#pragma once

#include "clw/Prerequisites.h"

namespace clw
{
    class CLW_EXPORT Grid
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

    CLW_EXPORT std::ostream& operator<<(std::ostream& strm, const Grid& grid);
}