#pragma once
#include "..\MTMode.h"
#include <SFML/Graphics.hpp>

class Game
{
private:
	sf::CircleShape _player;
	sf::RenderWindow _window;

public:
	Game();
	~Game();

	void run();
private:
	void processEvents();
	void update();
	void render();
};

