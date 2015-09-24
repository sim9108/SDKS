#include "Game.h"


Game::Game()
:_window(sf::VideoMode(640, 480), "SFML Application"),
_player()
{
	this->_texture.loadFromFile("Eagle.png");
	this->_player.setTexture(this->_texture);
	this->_player.setPosition(100.0f, 100.0f);
	this->_window.setVerticalSyncEnabled(true);
}


Game::~Game()
{
}


void
Game::run(){
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time TimePerFrame = sf::seconds(1.f / 120.f);

	while (this->_window.isOpen()){
		this->processEvents();

		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}

		this->render();
	}
}

void
Game::processEvents(){
	sf::Event event;
	while (this->_window.pollEvent(event)){
		switch (event.type)
		{
			case sf::Event::Closed:
				this->_window.close();
				break;

			case sf::Event::KeyPressed:
				this->handleLKey(event.key.code, true);
				break;

			case sf::Event::KeyReleased:
				this->handleLKey(event.key.code, false);
				break;
		}
	}
}

void
Game::handleLKey(sf::Keyboard::Key key, bool keypressed){
	keymap_[key] = keypressed;
}

void
Game::update(sf::Time deltaTime){
	sf::Vector2f movement(0.0f, 0.0f);
	if (keymap_[sf::Keyboard::W]){
		movement.y -= 1.0f;
	}

	if (keymap_[sf::Keyboard::S]){
		movement.y += 1.0f;
	}

	if (keymap_[sf::Keyboard::A]){
		movement.x -= 1.0f;
	}

	if (keymap_[sf::Keyboard::D]){
		movement.x += 1.0f;
	}
		
	this->_player.move(movement*deltaTime.asSeconds()*10.0f);
}

void
Game::render(){
	this->_window.clear();
	this->_window.draw(this->_player);
	this->_window.display();
}