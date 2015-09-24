#include "Game.h"

Game::Game()
:
_window(sf::VideoMode(640, 480), "SFML Application"),
_player()
{
	this->_player.setRadius(40.0f);
	this->_player.setPosition(100.0f, 100.0f);
	this->_player.setFillColor(sf::Color::Cyan);
}

Game::~Game()
{
}

void
Game::run(){
	while (this->_window.isOpen()){
		this->processEvents();
		this->update();
		this->render();
	}
}

void 
Game::processEvents(){
	sf::Event event;
	while (this->_window.pollEvent(event)){
		if (event.type == sf::Event::Closed)
			this->_window.close();
	}
}

void
Game::update(){
}

void
Game::render(){
	this->_window.clear();
	this->_window.draw(this->_player);
	this->_window.display();
}