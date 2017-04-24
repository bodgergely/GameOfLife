

/*
 * What we need to have:
 *
 * - Graphical window to contain the dots
 * - Engine to light up and down the dots
 *
 *
 */

#include "Controller.h"
#include "View.h"
#include "Model.h"


class GameOfLife
{
public:
	GameOfLife(int col, int row, Configuration& config, const char* name) : _col(col), _row(row), _name(name)
	{
		_controller = new Controller(_col, _row);
		_model = new Model(*_controller, _col, _row, config);
		_view = new View(*_controller, _col, _row, name);
	}
	void play()
	{
		_model->startThread();
		_view->startThread();
		_model->join();
		_view->join();
	}
private:
	Controller* 	_controller{NULL};
	Model* 			_model{NULL};
	View* 			_view{NULL};
	int				_col;
	int				_row;
	const char*		_name;
};


int main()
{
	int cols = 1920;
	int rows = 1080;
	Configuration config(cols, rows, cols*rows/40);
	GameOfLife gof(cols, rows, config, "GameOfLife");
	gof.play();
}
