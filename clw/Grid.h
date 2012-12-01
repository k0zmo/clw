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

		cl_uint dimensions() const { return dims; }
		size_t width() const { return sizes[0]; }
		size_t height() const { return sizes[1]; }
		size_t depth() const { return sizes[2]; }

		operator const size_t*() const { return sizes; }

		Grid roundTo(const Grid& range) const;

	private:
		cl_uint dims;
		size_t sizes[3];
	};

	inline Grid::Grid()
		: dims(1)
	{
		sizes[0] = sizes[1] = sizes[2] = 1;
	}

	inline Grid::Grid(size_t width)
		: dims(1)
	{
		sizes[0] = width;
		sizes[1] = sizes[2] = 0;
	}

	inline Grid::Grid(size_t width, size_t height)
		: dims(2)
	{
		sizes[0] = width;
		sizes[1] = height;
		sizes[2] = 0;
	}

	inline Grid::Grid(size_t width, size_t height, size_t depth)
		: dims(3)
	{
		sizes[0] = width;
		sizes[1] = height;
		sizes[2] = depth;
	}
}