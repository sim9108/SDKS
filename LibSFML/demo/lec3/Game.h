#pragma once

#include"../MTMode.h"

#include <SFML/Graphics.hpp>
#include <unordered_map>


class Game
{
private:
	sf::Texture _texture;
	sf::Sprite _player;
	sf::RenderWindow _window;
	std::unordered_map<sf::Keyboard::Key, bool> keymap_;
	
public:
	Game();
	~Game();

	void run();

private:
	void handleLKey(sf::Keyboard::Key key, bool keypressed);
	void processEvents();
	void update(sf::Time deltaTime);
	void render();
};

