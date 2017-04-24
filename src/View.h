#pragma once

#include <SFML/Graphics.hpp>
#include "Controller.h"
#include "Observer.h"
#include <vector>
#include <iostream>
#include <atomic>
#include "Thread.h"

using std::vector;
using std::cout;
using std::endl;

class View : public Thread
{
public:
	View(Controller& controller, int col, int row, const char* name) : _controller(controller), _col(col), _row(row), _name(name), _window(NULL)
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
	virtual void startThread()
	{
		_thread = std::thread(&View::loop, this);
	}

private:
	void loop()
	{
		 // Start the game loop
		_window = new sf::RenderWindow(sf::VideoMode(_col, _row), _name);
		// Clear screen
		_window->clear();
		while (_window->isOpen())
		{
			// Process events
			sf::Event event;
			while (_window->pollEvent(event))
			{
				// Close window: exit
				if (event.type == sf::Event::Closed)
					_window->close();
			}
			sf::VertexArray va;
			vector<Point> points = _controller.get();
			for(const Point& p : points)
			{
				sf::Vector2f coord( p.col,  p.row);
				if(p.on)
					va.append(sf::Vertex(coord, sf::Color::Yellow));
				else
					va.append(sf::Vertex(coord, sf::Color::Black));
			}

			// Draw the point (sf::Vertex)
			_window->draw(va);


			// Update the window
			_window->display();

		}
	}
private:
	Controller&		 _controller;
	int				 _col;
	int				 _row;
	const char*		 _name;
	sf::RenderWindow* _window;
};
