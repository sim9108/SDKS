// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <SFML/Graphics.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 600), "My First Sfmal Game");
	
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("player.png")){
		std::cout << "Error image file" << std::endl;
	}

	sf::Sprite playerImage;
	playerImage.setTexture(pTexture);

	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (sf::Event::Closed == Event.type){
				window.close();
			}
		}
		window.draw(playerImage);
		window.display();
	}

	return 0;
}

