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

//move colliding to Game class
bool Player::moveIsColliding(sf::FloatRect playerRect, const std::vector<Object>& collisionObjects)
{
	for (int i = 0; i < collisionObjects.size(); ++i)
	{
		if (playerRect.intersects(collisionObjects[i].sprite.getGlobalBounds()))
		{
			std::cout << collisionObjects[i].name << std::endl;
			return true;
		}
	}

	return false;
}

void Player::move()
{
	bool playerMoved = false;
	sf::FloatRect playerRect = pSprite.getGlobalBounds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		playerRect.top -= 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			pSprite.move(0, -1);
		source.y = moveUp;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		playerRect.height += 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			pSprite.move(0, 1);
		source.y = moveDown;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerRect.width += 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			pSprite.move(1, 0);
		source.y = moveRight;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerRect.left -= 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			pSprite.move(-1, 0);
		source.y = moveLeft;
		playerMoved = true;
	}

	if (playerMoved) {
		frameCounter += frameSpeed * clock.restart().asSeconds();
		if (frameCounter >= switchFrame)
		{
			frameCounter = 0;
			source.x++;
			if (source.x * 32 >= pTexture.getSize().x)
				source.x = 0;
		}

		pSprite.setTextureRect(sf::IntRect(source.x * 32, source.y * 32, 32, 32));
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(pSprite, states);
}