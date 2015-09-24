// lec1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\MTMode.h"


int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");

	sf::CircleShape shape;
	shape.setPosition(100.0f, 100.f);
	shape.setRadius(40.0f);
	shape.setFillColor(sf::Color::Cyan);

	while (window.isOpen()){

		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}


	return 0;
}

