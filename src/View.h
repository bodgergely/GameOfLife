#pragma once

#include <SFML/Graphics.hpp>
#include "Controller.h"

class View
{
public:
	View(Controller& controller, int col, int row, const char* name) : _controller(controller), _window(sf::VideoMode(col, row), name)
	{
	}
	void loop()
	{
		 // Start the game loop
		while (_window.isOpen())
		{
			// Process events
			sf::Event event;
			while (_window.pollEvent(event))
			{
				// Close window: exit
				if (event.type == sf::Event::Closed)
					_window.close();
			}
			// Clear screen
			_window.clear();
			// Draw the sprite
			//window.draw(sprite);
			// Draw the string
			//window.draw(text);
			// Update the window
			_window.display();
		    }
	}
private:
	Controller&		 _controller;
	sf::RenderWindow _window;
};
