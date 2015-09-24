// lec1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\MTMode.h"
#include <SFML/Graphics.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
	//원 생성한다.
	sf::CircleShape shape;
	shape.setPosition(100.0f, 100.f);
	shape.setRadius(40.0f);
	shape.setFillColor(sf::Color::Cyan);

	// 윈도우를 생성한다.
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
	while (window.isOpen()){
		// 사용자 이벤트를 처리한다.
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed) window.close();
		}

		//기본 이미지를 지우고, 원을 그리고, 표시한다.
		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}

