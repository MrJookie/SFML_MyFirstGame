#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

Game::Game()
{
}

Game::Game(sf::Vector2f screenDimension)
{
	this->screenDimension = screenDimension;

	playerView.setCenter(screenDimension.x / 2, screenDimension.y / 2);
	playerView.setSize(screenDimension.x, screenDimension.y);
	playerView.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	font.loadFromFile("arial.ttf");
	pTexture.loadFromFile("player.png");
	pSprite.setTexture(pTexture);
	pSprite.setPosition(400, 250);

	pSize.x = 32;
	pSize.y = 32;

	source.x = 1;
	source.y = 0;

	frameCounter = 0;
	switchFrame = 120;
	frameSpeed = 700;

	pSprite.setTextureRect(sf::IntRect(source.x * pSize.x, source.y * pSize.y, pSize.x, pSize.y));

	setLevel(17);
	setHP(30);
	setSpeed(2.0);

	HUD = sf::View(sf::FloatRect(0, 0, screenDimension.x, screenDimension.y));

	initPlayerHUD();
}

Game::~Game()
{
}

void Game::setSubRects(std::vector<sf::Rect<int>>& subRects)
{
	this->subRects = subRects;
}

void Game::setObjects(std::vector<Object>& objects)
{
	this->objects = objects;
}

void Game::setLayers(std::vector<Layer>& layers)
{
	this->layers = layers;
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
		FPS = static_cast<int>(1.0f / time.asSeconds());
	}
}

void Game::initPlayerHUD()
{
	FPSText.setFont(font);
	FPSText.setCharacterSize(30);

	pLevelHUD.setFont(font);
	pLevelHUD.setString("lvl." + std::to_string(getLevel()) + " 97%");
	pLevelHUD.setCharacterSize(8);
	pLevelHUD.setColor(sf::Color(0, 0, 0));

	hpBarOutline.setSize(sf::Vector2f(static_cast<float>(pSize.x), 3));
	hpBarOutline.setFillColor(sf::Color(0, 255, 0, 0));
	hpBarOutline.setOutlineThickness(1);
	hpBarOutline.setOutlineColor(sf::Color(0, 0, 0, 255));

	hpBarFill.setSize(sf::Vector2f(static_cast<float>(pSize.x), 3));
}

void Game::update()
{
	setDrawingBounds();
	calculateFPS();
	updateAnimatedFlowers();
	updateAnimatedCoins();
	updateHUD();
	updatePlayerHUD();
	updatePlayerCam();
}

void Game::updatePlayerHUD()
{
	hpBarOutline.setPosition(sf::Vector2f(pSprite.getPosition().x, pSprite.getPosition().y - 8));
	hpBarFill.setPosition(sf::Vector2f(pSprite.getPosition().x, pSprite.getPosition().y - 8));
	pLevelHUD.setPosition(sf::Vector2f(pSprite.getPosition().x, pSprite.getPosition().y - 20));
	
	if (pHP < 26) {
		hpBarFill.setFillColor(sf::Color(180, 0, 0, 255));
	}
	else if (pHP < 51) {
		hpBarFill.setFillColor(sf::Color(255, 51, 0, 255));
	}
	else if (pHP < 76) {
		hpBarFill.setFillColor(sf::Color(255, 204, 0, 255));
	}
	else if (pHP <= 100) {
		hpBarFill.setFillColor(sf::Color(0, 255, 0, 255));
	}

	if (pHP >= 0 && pHP <= 100)
		hpBarFill.setSize(sf::Vector2f(static_cast<float>(pSize.x * (pHP / 100.0f)), 3));
	else
		hpBarFill.setSize(sf::Vector2f(0, 3));
}

void Game::setHP(int hp)
{
	pHP = hp;

	if (pHP < 0)
		pHP = 0;
	else if (pHP > 100)
		pHP = 100;
}

int Game::getHP()
{
	return pHP;
}

void Game::setSpeed(float speed)
{
	pSpeed = speed;
}

float Game::getSpeed()
{
	return pSpeed;
}

void Game::setLevel(int level)
{
	pLevel = level;
}

int Game::getLevel()
{
	return pLevel;
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

bool Game::moveIsColliding(sf::FloatRect& playerRect)
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
		if (!moveIsColliding(playerRect) && pSprite.getPosition().y + pSize.y < mapSizePixels.y)
			pSprite.move(0, pSpeed);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerRect.width += pSpeed;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().x + pSize.x < mapSizePixels.x)
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
			if (source.x * pSize.x >= static_cast<int>(pTexture.getSize().x))
				source.x = 0;
		}

		pSprite.setTextureRect(sf::IntRect(source.x * pSize.x, source.y * pSize.y, pSize.x, pSize.y));
	}
}

void Game::updatePlayerCam()
{
	if (pSprite.getPosition().x + pSize.x > screenDimension.x / 2)
	{
		if (pSprite.getPosition().x + pSize.x + screenDimension.x / 2 < mapSizePixels.x)
			camPosition.x = pSprite.getPosition().x + pSize.x;
	}
	else
	{
		camPosition.x = screenDimension.x / 2;
	}

	if (pSprite.getPosition().y + pSize.y > screenDimension.y / 2)
	{
		if (pSprite.getPosition().y + pSize.y + screenDimension.y / 2 < mapSizePixels.y)
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
			if (x < mapSizeTiles.x && y < mapSizeTiles.y)
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

void Game::onMouseWheelMoved(sf::Event event)
{
	/*
	int delta = event.mouseWheel.delta;
	sf::Vector2i pos(event.mouseWheel.x, event.mouseWheel.y);

	if (delta > 0)
		playerView.zoom(1.2f);
	else if (delta < 0)
		playerView.zoom(1.0f);
	*/
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
	int playerPosToTileX = static_cast<int>(pSprite.getPosition().x / tileSize.x);
	int playerPosToTileY = static_cast<int>(pSprite.getPosition().y / tileSize.y);

	leftTileX = playerPosToTileX - static_cast<int>(screenDimension.x / tileSize.x);
	leftTileX = leftTileX < 0 ? 0 : leftTileX;
	leftTileX = leftTileX > mapSizeTiles.x ? mapSizeTiles.x : leftTileX;

	rightTileX = playerPosToTileX + static_cast<int>(screenDimension.x / tileSize.x);
	rightTileX = rightTileX < 0 ? 0 : rightTileX;
	rightTileX = rightTileX > mapSizeTiles.x ? mapSizeTiles.x : rightTileX;

	topTileY = playerPosToTileY - static_cast<int>(screenDimension.y / tileSize.y);
	topTileY = topTileY < 0 ? 0 : topTileY;
	topTileY = topTileY > mapSizeTiles.y ? mapSizeTiles.y : topTileY;

	bottomTileY = playerPosToTileY + static_cast<int>(screenDimension.y / tileSize.y);
	bottomTileY = bottomTileY < 0 ? 0 : bottomTileY;
	bottomTileY = bottomTileY > mapSizeTiles.y ? mapSizeTiles.y : bottomTileY;
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
				if (x < mapSizeTiles.x && y < mapSizeTiles.y)
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
				if (x < mapSizeTiles.x && y < mapSizeTiles.y)
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

void Game::setMapSizeTiles(sf::Vector2i mapSizeTiles)
{
	this->mapSizeTiles = mapSizeTiles;
}

void Game::setMapSizePixels(sf::Vector2i mapSizePixels)
{
	this->mapSizePixels = mapSizePixels;
}

void Game::setTileSize(sf::Vector2i tileSize)
{
	this->tileSize = tileSize;
}