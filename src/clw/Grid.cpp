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

#include "Grid.h"
#include <ostream>

namespace clw
{
    namespace detail
    {
        size_t roundUp(size_t value, size_t multiple)
        {
            if(multiple <= 1)
                return value;
            size_t v = value % multiple;
            if (v)
                return value + (multiple - v);
            return value;
        }
    }

    Grid Grid::roundTo(const Grid& range) const
    {
        switch(_dims)
        {
        case 1:
            return Grid(detail::roundUp(width(), range.width()));
        case 2:
            return Grid(detail::roundUp(width(), range.width()),
                        detail::roundUp(height(), range.height()));
        case 3:
            return Grid(detail::roundUp(width(), range.width()),
                        detail::roundUp(height(), range.height()),
                        detail::roundUp(depth(), range.depth()));
        default:
            return Grid();
        }
    }

    std::ostream& operator<<(std::ostream& strm, const Grid& grid)
    {
        strm << "(" << grid.width() << ", " << grid.height() << ", " << grid.depth() << ")";
        return strm;
    }
}