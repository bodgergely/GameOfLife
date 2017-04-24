

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
		_model = new Model(_col, _row, config);
		_view = new View(*_model, _col, _row, name);
	}
	void play()
	{
		_view->loop();
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
	int cols = 520;
	int rows = 460;
	Configuration config(cols, rows, cols*rows/4000);
	GameOfLife gof(cols, rows, config, "GameOfLife");
	gof.play();
}
