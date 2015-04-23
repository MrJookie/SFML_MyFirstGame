#include "Game.h"

Game::Game(sf::Vector2f screenDimension)
{
	this->screenDimension = screenDimension;

	playerView.setCenter(screenDimension.x / 2, screenDimension.y / 2);
	playerView.setSize(screenDimension.x, screenDimension.y);
	playerView.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	camPosition.x = screenDimension.x / 2;
	camPosition.y = screenDimension.y / 2;

	updatePlayerCam();

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
	updateAnimatedCoins();
	updateHUD();
	updatePlayerHUD();
}

void Game::initHUD()
{
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

void Game::interactObject(Object& object)
{
	std::cout << object.name << std::endl;

	if (object.name == "elixir_hp")
	{
		object.isVisible = false;

		setHP(getHP() + 30);
	}
}

bool Game::moveIsColliding(sf::FloatRect playerRect)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i].isVisible)
		{
			if (playerRect.intersects(objects[i].sprite.getGlobalBounds()))
			{
				interactObject(objects[i]);
				return true;
			}
		}
	}

	return false;
}

void Game::movePlayer(int moveDirection)
{
	bool playerMoved = false;
	sf::FloatRect playerRect = pSprite.getGlobalBounds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		playerRect.top -= pSpeed;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().y > 0)
			pSprite.move(0, -pSpeed);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		playerRect.height += pSpeed;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().y + pSize.y < getMapSizePixels().y)
			pSprite.move(0, pSpeed);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerRect.width += pSpeed;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().x + pSize.x < getMapSizePixels().x)
			pSprite.move(pSpeed, 0);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerRect.left -= pSpeed;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().x > 0)
			pSprite.move(-pSpeed, 0);
		source.y = moveDirection;
		playerMoved = true;
	}

	if (playerMoved) {
		frameCounter += frameSpeed * playerAnimationClock.restart().asSeconds();
		if (frameCounter >= switchFrame)
		{
			frameCounter = 0;
			source.x++;
			if (source.x * pSize.x >= static_cast<int> (pTexture.getSize().x))
				source.x = 0;
		}

		pSprite.setTextureRect(sf::IntRect(source.x * pSize.x, source.y * pSize.y, pSize.x, pSize.y));

		updatePlayerCam();
	}
}

void Game::updatePlayerCam()
{
	if (pSprite.getPosition().x + pSize.x > screenDimension.x / 2)
	{
		if (pSprite.getPosition().x + pSize.x + screenDimension.x / 2 < getMapSizePixels().x)
			camPosition.x = pSprite.getPosition().x + pSize.x;
	}
	else
	{
		camPosition.x = screenDimension.x / 2;
	}

	if (pSprite.getPosition().y + pSize.y > screenDimension.y / 2)
	{
		if (pSprite.getPosition().y + pSize.y + screenDimension.y / 2 < getMapSizePixels().y)
			camPosition.y = pSprite.getPosition().y + pSize.y;
	}
	else
	{
		camPosition.y = screenDimension.y / 2;
	}

	playerView.setCenter(camPosition);
}

void Game::updateAnimatedFlowers()
{
	if (flowersAnimationClock.getElapsedTime().asMilliseconds() >= 300)
	{
		flowersAnimationClock.restart().asMilliseconds();
		currentFlowersTile++;
	}

	if (currentFlowersTile < 60 || currentFlowersTile > 64)
		currentFlowersTile = 60;

	for (int x = leftTileX; x <= rightTileX; ++x)
	{
		for (int y = topTileY; y <= bottomTileY; ++y)
		{
			if (x < getMapSizeTiles().x && y < getMapSizeTiles().y)
			{
				layers[1].tiles[x][y].setTextureRect(subRects[currentFlowersTile]);
			}
		}
	}
}

void Game::updateAnimatedCoins()
{
	if (coinsAnimationClock.getElapsedTime().asMilliseconds() >= 150)
	{
		coinsAnimationClock.restart().asMilliseconds();
		currentCoinsTile++;
	}

	if (currentCoinsTile < 105 || currentCoinsTile > 108)
		currentCoinsTile = 105;

	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i].isVisible && objects[i].name == "coin")
			objects[i].sprite.setTextureRect(subRects[currentCoinsTile]);
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
	int playerPosToTileX = static_cast<int> (pSprite.getPosition().x / getTileSize().x);
	int playerPosToTileY = static_cast<int> (pSprite.getPosition().y / getTileSize().y);

	leftTileX = playerPosToTileX - static_cast<int> (screenDimension.x / getTileSize().x);
	leftTileX = leftTileX < 0 ? 0 : leftTileX;
	leftTileX = leftTileX > getMapSizeTiles().x ? getMapSizeTiles().x : leftTileX;

	rightTileX = playerPosToTileX + static_cast<int> (screenDimension.x / getTileSize().x);
	rightTileX = rightTileX < 0 ? 0 : rightTileX;
	rightTileX = rightTileX > getMapSizeTiles().x ? getMapSizeTiles().x : rightTileX;

	topTileY = playerPosToTileY - static_cast<int> (screenDimension.y / getTileSize().y);
	topTileY = topTileY < 0 ? 0 : topTileY;
	topTileY = topTileY > getMapSizeTiles().y ? getMapSizeTiles().y : topTileY;

	bottomTileY = playerPosToTileY + static_cast<int> (screenDimension.y / getTileSize().y);
	bottomTileY = bottomTileY < 0 ? 0 : bottomTileY;
	bottomTileY = bottomTileY > getMapSizeTiles().y ? getMapSizeTiles().y : bottomTileY;
}

//vector<Object*> Level::getObjectsAt(float x, float y) {

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(playerView);

	//draw map, flowers
	for (int layer = 0; layer < 2; ++layer)
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

	//draw objects
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i].isVisible)
			target.draw(objects[i].sprite, states);
	}

	//draw player
	target.draw(pSprite, states);

	//draw trees, so player can go behind the trees
	for (int layer = 2; layer < 3; ++layer)
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

	//draw playerHUD
	target.draw(hpBarOutline, states);
	target.draw(hpBarFill, states);
	target.draw(pLevelHUD);

	target.setView(HUD);
	target.draw(FPSText);
}