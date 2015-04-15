#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Level.h"

class Game : public sf::Transformable, public sf::Drawable
{
public:
	Game();
	Game(Game& value);
	//Player operator=(Player& value);
	~Game();
	void setCollisionObject(std::vector<Object> collisionObjects);

protected:
	std::vector<Object> collisionObjects;

private:
	sf::View cam;

	virtual void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif