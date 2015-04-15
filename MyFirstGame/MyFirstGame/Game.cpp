#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

Game::Game()
{
	//add cam here
	/*
	if (pSprite.getPosition().x + 32 > screenDimensions.x / 2)
		camPosition.x = pSprite.getPosition().x + 32;
	else
		camPosition.x = screenDimensions.x / 2;

	if (playerSprite.getPosition().y + 32 > screenDimensions.y / 2) {
		if (playerSprite.getPosition().y + 32 < 8000)
			camPosition.y = playerSprite.getPosition().y + 32;
	}
	else
		camPosition.y = screenDimensions.y / 2;
		*/
}

Game::~Game()
{
}

void Game::setCollisionObject(std::vector<Object> collisionObjects)
{
	this->collisionObjects = collisionObjects;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(pSprite, states);

	for (int i = 0; i < collisionObjects.size(); ++i)
	{
		target.draw(collisionObjects[i].sprite);
	}
}