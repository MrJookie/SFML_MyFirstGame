#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

Player::Player()
{
	source.x = 1;
	source.y = 0;

	pTexture.loadFromFile("player.png");
	pSprite.setTexture(pTexture);
	pSprite.setPosition(100, 100);

	pSprite.setTextureRect(sf::IntRect(source.x * 32, source.y * 32, 32, 32));
}

Player::~Player()
{
}
