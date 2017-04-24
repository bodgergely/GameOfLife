#pragma once

#include <utility>
#include <cstddef>

//typedef std::pair<int, int> Point;

struct Point
{
	Point(int r, int c, bool on_=false) : row(r), col(c), on(on_) {}
	int row;
	int col;
	bool on;
};

inline bool operator == (const Point& lhs, const Point& rhs)
{
	if(lhs.row == rhs.row && lhs.col == rhs.col)
		return true;
	else
		return false;
}

struct PointHash
{
	size_t operator() (const Point& p) const
	{
		return p.row* p.col;
	}
};
