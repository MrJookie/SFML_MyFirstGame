#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

Game::Game(sf::Vector2f screenDimension)
{
	this->screenDimension = screenDimension;

	playerView.setCenter(screenDimension.x / 2, screenDimension.y / 2);
	playerView.setSize(screenDimension.x, screenDimension.y);
	playerView.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	camPosition.x = screenDimension.x / 2;
	camPosition.y = screenDimension.y / 2;
}

Game::~Game()
{
}

bool Game::moveIsColliding(sf::FloatRect playerRect)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		if (playerRect.intersects(objects[i].sprite.getGlobalBounds()))
		{
			std::cout << objects[i].name << std::endl;
			return true;
		}
	}

	return false;
}

void Game::movePlayer(int moveDirection)
{
	bool playerMoved = false;
	sf::FloatRect playerRect = pSprite.getGlobalBounds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		playerRect.top -= 1;
		if (!moveIsColliding(playerRect))
			pSprite.move(0, -1);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		playerRect.height += 1;
		if (!moveIsColliding(playerRect))
			pSprite.move(0, 1);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerRect.width += 1;
		if (!moveIsColliding(playerRect))
			pSprite.move(1, 0);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerRect.left -= 1;
		if (!moveIsColliding(playerRect))
			pSprite.move(-1, 0);
		source.y = moveDirection;
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
		updatePlayerCam();
		playerView.setCenter(camPosition);
	}
}

void Game::updatePlayerCam()
{
	if (pSprite.getPosition().x + 32 > screenDimension.x / 2)
		camPosition.x = pSprite.getPosition().x + 32;
	else
		camPosition.x = screenDimension.x / 2;

	if (pSprite.getPosition().y + 32 > screenDimension.y / 2) {
		if (pSprite.getPosition().y + 32 < 8000)
			camPosition.y = pSprite.getPosition().y + 32;
	}
	else
		camPosition.y = screenDimension.y / 2;
}

sf::Vector2f Game::getScreenDimension()
{
	return screenDimension;
}

sf::Vector2f Game::getPlayerCamPosition()
{
	return camPosition;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(playerView);

	int playerPosToTileX = pSprite.getPosition().x / 32;
	int playerPosToTileY = pSprite.getPosition().y / 32;

	int leftTileX = playerPosToTileX - screenDimension.x / 32;
	int rightTileX = playerPosToTileX + screenDimension.x / 32;
	int topTileY = playerPosToTileY - screenDimension.y / 32;
	int bottomTileY = playerPosToTileY + screenDimension.y / 32;

	if (leftTileX < 0)
		leftTileX = 0;
	else if (leftTileX > 255)
		leftTileX = 255;

	if (rightTileX < 0)
		rightTileX = 0;
	else if (rightTileX > 255)
		rightTileX = 255;

	if (topTileY < 0)
		topTileY = 0;
	else if (topTileY > 255)
		topTileY = 255;

	if (bottomTileY < 0)
		bottomTileY = 0;
	else if (bottomTileY > 255)
		bottomTileY = 255;

	/*
	std::cout << "leftTileX: " << leftTileX << " topTileY: " << topTileY << std::endl;
	std::cout << "rightTileX: " << rightTileX << " bottomTileY: " << bottomTileY << std::endl;
	*/

	for (int layer = 0; layer < layers.size(); ++layer)
	{
		for (int x = leftTileX; x < rightTileX; ++x)
		{
			for (int y = topTileY; y < bottomTileY; ++y)
			{
				target.draw(layers[layer].tiles[x][y], states);
			}
		}
	}

	for (int i = 0; i < objects.size(); ++i)
	{
		target.draw(objects[i].sprite, states);
	}

	target.draw(pSprite, states);
}