#pragma once

#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <unordered_set>
#include <iostream>
#include "Controller.h"
#include "Point.h"
#include "Thread.h"

using std::vector;
using std::pair;
using std::make_pair;
using std::unordered_set;


class Configuration
{
public:
	Configuration(int cols, int rows, int population) : _cols(cols), _rows(rows), _population(population)
	{
		if(population > cols*rows)
			throw std::runtime_error("Pre-population can not be bigger than table size!");
		srand(time(NULL));

		int patchSize = 10;

		int c = 0;
		vector<int> likelihoods{40, 56, 87,37, 49, 67, 46, 87, 78, 34};
		while(_ps.size() < _population)
		{
			pair<Point, Point> patch = getPatch(patchSize);
			fillPatch(patch.first, patch.second, likelihoods[rand() % likelihoods.size()]);
		}
	}

	vector<Point>& points()
	{
		return _points;
	}
private:
	void fillPatch(Point x, Point y, int likelihood)
	{
		for(int r=x.row;r <= y.row; r++)
		{
			for(int c=x.col;c<y.col; c++)
			{
				Point p = Point(r, c);
				if(( _ps.find(p)==_ps.end() ) && (rand() % 100 > likelihood) && _ps.size() < _population)
				{
					_points.push_back(p);
					_ps.insert(p);
				}
			}
		}
	}

	pair<Point, Point> getPatch(int patchSize)
	{
		int sr = rand() % _rows;
		if(sr > _rows - patchSize)
			sr-=1;
		int er = sr + patchSize;

		int sc = rand() % _cols;
		if(sc > _cols - patchSize)
			sc-=1;
		int ec = sc + patchSize;

		return make_pair(Point(sr, sc), Point(er,ec));
	}


private:
	int _cols;
	int _rows;
	int _population;
	vector<Point> _points;
	unordered_set<Point, PointHash> 	  _ps;
};

class Tile
{
public:
	Tile(int col, int row) : _rows(row), _cols(col)
	{
		_elems = new char[col*row];
		memset(_elems, 0, col*row);
		_updated = new bool[col * row];
		memset(_updated, 0, col*row);
	}
	~Tile(){ delete[] _elems; }

	inline void  set(int r, int c)
	{
		_elems[r*_cols + c] = 1;
		_updated[r*_cols + c] = true;
	}
	inline void  clear(int r, int c)
	{
		_elems[r*_cols + c] = 0;
		_updated[r*_cols + c] = true;
	}
	inline bool  isSet(int r, int c) { return _elems[r*_cols + c];}

	vector<Point> updates() const
	{
		vector<Point> points;
		for(int i=0;i<_rows;i++)
		{
			for(int j=0;j<_cols;j++)
			{
				if(_updated[pos(i,j)])
				{
					bool live = _elems[pos(i, j)];
					points.push_back(Point(i, j, live));
				}
			}
		}
		memset(_updated, 0, _cols*_rows);
		return points;
	}

private:
	int pos(int r, int c) const { return r*_cols + c; }

	int	  _rows;
	int	  _cols;
	char* _elems {NULL};
	bool* _updated {NULL};
};


/*
 * Rules:
 * 	Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
	Any live cell with two or three live neighbours lives on to the next generation.
	Any live cell with more than three live neighbours dies, as if by overpopulation.
	Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 */

class Model : public Thread
{
public:
	Model(Controller& controller, int col, int row, Configuration config) : _controller(controller), _col(col), _row(row), _tile(col, row)
	{
		vector<Point>& preconfig = config.points();
		for(Point& p : preconfig)
		{
			_tile.set(p.row, p.col);
		}
	}


	virtual void startThread()
	{
		_thread = std::thread(&Model::loop, this);
	}

private:
	void loop()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		while(true)
		{
			updateTile();
			updateController();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	void updateTile()
	{
		for(int r=0;r<_row;r++)
		{
			for(int c=0;c<_col;c++)
			{
				int l = liveNeighborCount(r,c);
				if(_tile.isSet(r,c) && (l < 2 || l >3) )
				{
					_tile.clear(r,c);
				}
				else if(l == 3)
				{
					_tile.set(r, c);
				}
			}
		}
	}

	// there are 8 neigbours in total to inspect - except on the edges
	int liveNeighborCount(int row, int col)
	{
		int l = 0;

		for(int r=row-1;r <= row+1;r++)
		{
			if((row == 0 && r == 0) || (row == _row-1 && r == _row-1) )
				continue;
			for(int c=col-1;c <= col+1;c++)
			{
				if((col == 0 && c == 0) || (col == _col-1 && c == _col-1) )
					continue;
				if(_tile.isSet(r,c))
					++l;
			}
		}

		return l;

	}

	void updateController()
	{
		_controller.update(_tile.updates());
	}

private:
	Controller& _controller;
	int			_col;
	int			_row;
	Tile		_tile;
};
