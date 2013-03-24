#include "Grid.h"

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