#pragma once

class Controller
{
public:
	Controller(int col, int row) : _col(col), _row(row)
	{
	}
private:
	int			_col;
	int			_row;
};
