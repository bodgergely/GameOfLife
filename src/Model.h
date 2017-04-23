#pragma once

#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <set>
#include <iostream>
#include "Controller.h"

using std::vector;
using std::pair;
using std::make_pair;
using std::set;

typedef pair<int, int> Point;

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
		for(int r=x.first;r <= y.first; r++)
		{
			for(int c=x.second;c<y.second; c++)
			{
				Point p = make_pair(r, c);
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
	set<Point> 	  _ps;
};

class Tile
{
public:
	Tile(int col, int row) : _rows(row), _cols(col)
	{
		_elems = new char[col*row];
		memset(_elems, 0, col*row);
	}
	~Tile(){ delete[] _elems; }

	inline void  set(int r, int c){ _elems[r*_cols + c] = 1;}
	inline void  clear(int r, int c) { _elems[r*_cols + c] = 0; }
	inline bool  isSet(int r, int c) { return _elems[r*_cols + c];}

private:
	int	  _rows;
	int	  _cols;
	char* _elems {NULL};
};

class Model
{
public:
	Model(Controller& controller, int col, int row, Configuration config) : _controller(controller), _col(col), _row(row), _tile(col, row)
	{
		vector<Point>& preconfig = config.points();
		for(Point& p : preconfig)
		{
			_tile.set(p.first, p.second);
		}
	}
private:
	Controller& _controller;
	int			_col;
	int			_row;
	Tile		_tile;
};
