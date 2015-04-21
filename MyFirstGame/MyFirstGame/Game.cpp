#include "Game.h"

Game::Game(sf::Vector2f screenDimension)
{
	this->screenDimension = screenDimension;

	playerView.setCenter(screenDimension.x / 2, screenDimension.y / 2);
	playerView.setSize(screenDimension.x, screenDimension.y);
	playerView.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	camPosition.x = screenDimension.x / 2;
	camPosition.y = screenDimension.y / 2;

	initHUD();
}

Game::~Game()
{
}

void Game::update()
{
	setDrawingBounds();
	calculateFPS();
	updateAnimatedFlowers();
	updateHUD();
}

void Game::initHUD()
{
	font.loadFromFile("arial.ttf");
	FPSText.setFont(font);
	FPSText.setCharacterSize(30);
}

void Game::updateHUD()
{
	if (FPS > 0)
		FPSText.setString(std::to_string(FPS));
}

void Game::calculateFPS()
{
	time = fpsClock.getElapsedTime();
	fpsClock.restart().asSeconds();
	if (updateFpsClock.getElapsedTime().asSeconds() >= 1)
	{
		updateFpsClock.restart().asSeconds();
		FPS = static_cast<int> (1.0f / time.asSeconds());
	}
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
		if (!moveIsColliding(playerRect) && pSprite.getPosition().y > 0)
			pSprite.move(0, -1);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		playerRect.height += 1;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().y + 32 < getMapSizePixels().y)
			pSprite.move(0, 1);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerRect.width += 1;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().x + 32 < getMapSizePixels().x)
			pSprite.move(1, 0);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerRect.left -= 1;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().x > 0)
			pSprite.move(-1, 0);
		source.y = moveDirection;
		playerMoved = true;
	}

	if (playerMoved) {
		frameCounter += frameSpeed * playerAnimationClock.restart().asSeconds();
		if (frameCounter >= switchFrame)
		{
			frameCounter = 0;
			source.x++;
			if (source.x * 32 >= static_cast<int> (pTexture.getSize().x))
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
	{
		if (pSprite.getPosition().x + 32 + screenDimension.x / 2 < getMapSizePixels().x)
			camPosition.x = pSprite.getPosition().x + 32;
	}
	else
	{
		camPosition.x = screenDimension.x / 2;
	}

	if (pSprite.getPosition().y + 32 > screenDimension.y / 2)
	{
		if (pSprite.getPosition().y + 32 + screenDimension.y / 2 < getMapSizePixels().y)
			camPosition.y = pSprite.getPosition().y + 32;
	}
	else
	{
		camPosition.y = screenDimension.y / 2;
	}
}

void Game::updateAnimatedFlowers()
{
	if (flowerAnimationClock.getElapsedTime().asMilliseconds() >= 300)
	{
		flowerAnimationClock.restart().asMilliseconds();
		currentFlowerTile++;
	}

	if (currentFlowerTile < 60 || currentFlowerTile > 64)
		currentFlowerTile = 60;

	for (int x = leftTileX; x <= rightTileX; ++x)
	{
		for (int y = topTileY; y <= bottomTileY; ++y)
		{
			if (x < getMapSizeTiles().x && y < getMapSizeTiles().y)
			{
				layers[1].tiles[x][y].setTextureRect(subRects[currentFlowerTile]);
			}
		}
	}
}

sf::Vector2f Game::getScreenDimension()
{
	return screenDimension;
}

sf::Vector2f Game::getPlayerCamPosition()
{
	return camPosition;
}

void Game::setMouseCoords(int x, int y)
{
	mousePosition.x = x;
	mousePosition.y = y;
}

void Game::setDrawingBounds()
{
	int playerPosToTileX = static_cast<int> (pSprite.getPosition().x / 32);
	int playerPosToTileY = static_cast<int> (pSprite.getPosition().y / 32);

	leftTileX = playerPosToTileX - static_cast<int> (screenDimension.x / 32);
	leftTileX = leftTileX < 0 ? 0 : leftTileX;
	leftTileX = leftTileX > getMapSizeTiles().x ? getMapSizeTiles().x : leftTileX;

	rightTileX = playerPosToTileX + static_cast<int> (screenDimension.x / 32);
	rightTileX = rightTileX < 0 ? 0 : rightTileX;
	rightTileX = rightTileX > getMapSizeTiles().x ? getMapSizeTiles().x : rightTileX;

	topTileY = playerPosToTileY - static_cast<int> (screenDimension.y / 32);
	topTileY = topTileY < 0 ? 0 : topTileY;
	topTileY = topTileY > getMapSizeTiles().y ? getMapSizeTiles().y : topTileY;

	bottomTileY = playerPosToTileY + static_cast<int> (screenDimension.y / 32);
	bottomTileY = bottomTileY < 0 ? 0 : bottomTileY;
	bottomTileY = bottomTileY > getMapSizeTiles().y ? getMapSizeTiles().y : bottomTileY;
}

//vector<Object*> Level::getObjectsAt(float x, float y) {

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(playerView);

	for (int layer = 0; layer < layers.size(); ++layer)
	{
		for (int x = leftTileX; x <= rightTileX; ++x)
		{
			for (int y = topTileY; y <= bottomTileY; ++y)
			{
				if (x < getMapSizeTiles().x && y < getMapSizeTiles().y)
					target.draw(layers[layer].tiles[x][y], states);
			}
		}
	}

	for (int i = 0; i < objects.size(); ++i)
	{
		target.draw(objects[i].sprite, states);
	}

	target.draw(pSprite, states);

	target.setView(HUD);
	target.draw(FPSText);
}