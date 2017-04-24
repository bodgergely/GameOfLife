#pragma once

#include <SFML/Graphics.hpp>
#include "Model.h"
#include "Observer.h"
#include <vector>
#include <iostream>
#include <atomic>
#include "Thread.h"

using std::vector;
using std::cout;
using std::endl;

class View
{
public:
	View(Model& model, int col, int row, const char* name) : _model(model), _col(col), _row(row), _name(name), _window(NULL)
	{
	}

	/*
	void notify(const vector<Point>& updates)
	{
		for(const Point& p : updates)
		{
			cout << p.first << " " << p.second << endl;
		}
	}
	*/
	void loop()
	{
		 // Start the game loop
		_window = new sf::RenderWindow(sf::VideoMode(_col, _row), _name);
		unsigned long long iters = 0;
		while (_window->isOpen())
		{
			// Process events
			_window->clear();
			sf::Event event;
			while (_window->pollEvent(event))
			{
				// Close window: exit
				if (event.type == sf::Event::Closed)
					_window->close();
			}
			sf::VertexArray va;
			const Tile& tile = _model.get();

			for(int r=0;r<_row;r++)
			{
				for(int c=0;c<_col;c++)
				{
					sf::Vector2f coord(c, r);
					if(tile.isSet(r,c))
						va.append(sf::Vertex(coord, sf::Color::Yellow));
					else
						va.append(sf::Vertex(coord, sf::Color::Black));

				}

			}


			// Draw the point (sf::Vertex)
			_window->draw(va);


			// Update the window
			_window->display();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		}
	}
private:
	Model&		 _model;
	int				 _col;
	int				 _row;
	const char*		 _name;
	sf::RenderWindow* _window;
};
