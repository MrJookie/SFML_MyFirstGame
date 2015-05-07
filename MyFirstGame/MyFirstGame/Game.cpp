#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

	selectionStartPosition.x = mousePosition.x;
	selectionStartPosition.y = mousePosition.y;

	musicIsPlaying = false;
	musicIsPaused = false;
	music.openFromFile("music.ogg");
	music.setVolume(70);
	soundBuffer.loadFromFile("pickitem.ogg");
	pickItem.setBuffer(soundBuffer);

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

	pLevelExp = 0;

	setLevel(1);
	setLevelExp(0);
	setHP(30);
	setSpeed(100);

	itemIsSelected = false;
	isInventoryActive = false;
	for (int i = 0; i < 5; ++i)
		inventorySlotUsed[i] = false;

	HUD = sf::View(sf::FloatRect(0, 0, screenDimension.x, screenDimension.y));

	initHUD();
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

void Game::initHUD()
{
	FPSText.setFont(font);
	FPSText.setCharacterSize(30);

	pLevelHUD.setFont(font);
	pLevelHUD.setCharacterSize(10);
	pLevelHUD.setColor(sf::Color(0, 0, 0));

	hpBarOutline.setSize(sf::Vector2f(static_cast<float>(pSize.x), 3));
	hpBarOutline.setFillColor(sf::Color(0, 255, 0, 0));
	hpBarOutline.setOutlineThickness(1);
	hpBarOutline.setOutlineColor(sf::Color(0, 0, 0, 255));

	hpBarFill.setSize(sf::Vector2f(static_cast<float>(pSize.x), 3));

	inventoryTex.loadFromFile("inventory.png");
	inventory.setTexture(inventoryTex);
	inventory.setPosition((screenDimension.x / 2) - 126, screenDimension.y - 50);

	textureSpeakerOn.loadFromFile("speakerOn.png");
	textureSpeakerOff.loadFromFile("speakerOff.png");
	speaker.setTexture(textureSpeakerOn);

	speaker.setPosition(screenDimension.x - 20, 10);

	highlightBox.setFillColor(sf::Color(255, 255, 255, 60));
	highlightBox.setOutlineThickness(1.0);
	highlightBox.setOutlineColor(sf::Color(255, 0, 0, 255));

	itemSelectedBox.setSize(sf::Vector2f(32, 32));
	itemSelectedBox.setFillColor(sf::Color(255, 255, 255, 0));
	itemSelectedBox.setOutlineThickness(1.0);
	itemSelectedBox.setOutlineColor(sf::Color(255, 0, 0, 255));
}

void Game::playMusic()
{
	if (!musicIsPlaying)
	{
		musicIsPlaying = true;
		music.play();
	}
}

void Game::toggleMusic()
{
	if (speaker.getGlobalBounds().contains(mousePosition))
	{
		if (musicIsPaused)
		{
			speaker.setTexture(textureSpeakerOn);
			musicIsPaused = false;
			music.play();
		}
		else {
			speaker.setTexture(textureSpeakerOff);
			musicIsPaused = true;
			music.pause();
		}
	}
}

bool Game::selectItem(sf::RenderTarget& window, sf::Vector2i mousePosition)
{
	sf::Vector2f mouseClickPosition = window.mapPixelToCoords(mousePosition, playerView);

	itemIsSelected = false;

	for (const auto& it : objects)
	{
		if (it.sprite.getGlobalBounds().contains(mouseClickPosition) && it.isVisible)
		{
			itemIsSelected = true;
			itemSelectedBox.setPosition(it.sprite.getPosition());

			std::cout << "Selected item: " << it.name << std::endl;
		}
	}

	return true;
}

void Game::recalculateHighlightBox(sf::RenderTarget& window, sf::Vector2i mousePos)
{
	/*
	sf::Vector2f mousePosition = window.mapPixelToCoords(mousePos, playerView);

	std::cout << "mousePos.x: " << mousePos.x << std::endl;
	std::cout << "mousePosition.x: " << mousePosition.x << std::endl;
	*/

	selectionCurrentPosition.x = mousePosition.x - selectionStartPosition.x;
	selectionCurrentPosition.y = mousePosition.y - selectionStartPosition.y;

	highlightBox.setSize(selectionCurrentPosition);
	highlightBox.setPosition(selectionStartPosition.x, selectionStartPosition.y);

	/*
	//needs fix (convert to world coords), not important
	std::vector<Object> objectsHighlighted;

	for (auto& it : objects)
	{
		if (it.sprite.getGlobalBounds().intersects(highlightBox.getGlobalBounds()))
		{
			objectsHighlighted.push_back(it);
			it.isVisible = false;
		}
	}

	std::cout << "objects selected: " << objectsHighlighted.size() << std::endl;
	*/
}

void Game::recalculateHighlightBox(sf::Vector2i mousePosition)
{
	selectionStartPosition.x = mousePosition.x;
	selectionStartPosition.y = mousePosition.y;
}

void Game::update()
{
	setDrawingBounds();
	calculateFPS();
	updateAnimatedFlowers();
	updateAnimatedCoins();
	updateHUD();
	updateCam();

	elapsedTimeStep = timeStepClock.restart();
}

void Game::updateHUD()
{
	if (FPS > 0)
		FPSText.setString(std::to_string(FPS));

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

	pLevelHUD.setString("L" + std::to_string(getLevel()) + " " + std::to_string(getLevelExp()) + "%");
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

void Game::setLevelExp(int exp)
{
	pLevelExp += exp;
	if (pLevelExp >= 100)
	{
		setLevel(getLevel() + 1);
		pLevelExp = pLevelExp - 100;
	}
}

int Game::getLevel()
{
	return pLevel;
}

int Game::getLevelExp()
{
	return pLevelExp;
}

void Game::toggleInventory()
{
	if (isInventoryActive) {
		isInventoryActive = false;
	}
	else {
		isInventoryActive = true;
	}
}

void Game::interactObject(Object& object)
{
	//std::cout << object.name << std::endl;

	if (object.name == "elixir_hp")
	{
		object.isVisible = false;
		setHP(getHP() + 30);
		printSumVisibleObjectsLeft(object);
		pickItem.play();
	}
	else if (object.name == "elixir_speed") {
		object.isVisible = false;
		setSpeed(getSpeed() + 20);
		printSumVisibleObjectsLeft(object);
		pickItem.play();
	}
	else if (object.name == "pokeball") {
		if (addToInventory(object))
		{
			object.isVisible = false;
			printSumVisibleObjectsLeft(object);
			setLevelExp(30);
			pickItem.play();
		}
		else {
			std::cout << "Inventory is full!" << std::endl;
		}
	}
	else if (object.name == "coin") {
		if (addToInventory(object))
		{
			object.isVisible = false;
			printSumVisibleObjectsLeft(object);
			setLevelExp(60);
			pickItem.play();
		}
		else {
			std::cout << "Inventory is full!" << std::endl;
		}
	}
	else if (object.name == "key") {
		if (addToInventory(object))
		{
			object.isVisible = false;
			printSumVisibleObjectsLeft(object);
			setLevelExp(40);
			pickItem.play();
		}
		else {
			std::cout << "Inventory is full!" << std::endl;
		}
	}
	else if (object.name == "table1") {
		if (addToInventory(object))
		{
			object.isVisible = false;
			printSumVisibleObjectsLeft(object);
			setLevelExp(60);
			pickItem.play();
		}
		else {
			std::cout << "Inventory is full!" << std::endl;
		}
	}
	else if (object.name == "chest") {
		if (useInventoryItem("key"))
		{
			//addToInventory random item...
			object.sprite.setTextureRect(subRects[51]);
			object.name = "chest_opened";
			setLevelExp(80);
			pickItem.play();
			std::cout << "Chest has been opened and is empty!" << std::endl;
		}
		else {
			std::cout << "Missing key to open chest!" << std::endl;
		}
	}
}

bool Game::addToInventory(Object& object)
{
	sf::Vector2i inventoryItemPosition;
	inventoryItemPosition.x = inventory.getPosition().x + 7;
	inventoryItemPosition.y = inventory.getPosition().y + 6;

	for (int i = 0; i < 5; ++i)
	{
		if (!inventorySlotUsed[i])
		{
			inventorySlotUsed[i] = true;

			if (i == 0)
				inventoryItemPosition.x += 0;
			else if (i == 1)
				inventoryItemPosition.x += 52;
			else if (i == 2)
				inventoryItemPosition.x += 104;
			else if (i == 3)
				inventoryItemPosition.x += 156;
			else if (i == 4)
				inventoryItemPosition.x += 208;

			if (object.name == "coin")
				object.sprite.setTextureRect(subRects[105]);

			object.sprite.setPosition(inventoryItemPosition.x, inventoryItemPosition.y);
			inventoryItems[i] = object;

			return true;
		}
	}

	return false;
}

bool Game::useInventoryItem(std::string objectName)
{
	for (int i = 0; i < 5; ++i)
	{
		if (inventorySlotUsed[i] && inventoryItems[i].name == objectName)
		{
			inventorySlotUsed[i] = false;
			inventoryItems[i].isVisible = false;

			return true;
		}
	}

	return false;
}

template <typename T>
size_t Game::getSumVisibleObjectsByName(std::vector<T>& vec, std::string objectName)
{
	int sum = 0;
	for (std::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it) {
		if ((*it).name == objectName && (*it).isVisible)
			sum++;
	}

	return sum;
}

void Game::printSumVisibleObjectsLeft(Object& object)
{
	std::cout << "Visible objects " 
		<< object.name << ": " 
		<< getSumVisibleObjectsByName(objects, object.name) 
		<< std::endl;
}

bool Game::removeInventoryItem()
{
	for (int i = 0; i < 5; ++i)
	{
		if (inventorySlotUsed[i] && inventoryItems[i].isVisible 
			&& inventoryItems[i].sprite.getGlobalBounds().contains(mousePosition))
		{
			inventorySlotUsed[i] = false;
			inventoryItems[i].isVisible = false;

			std::cout << "removing item: " <<  inventoryItems[i].name << std::endl;

			return true;
		}
	}

	return false;
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
	float moveDelta = elapsedTimeStep.asSeconds() * pSpeed;

	bool playerMoved = false;
	sf::FloatRect playerRect = pSprite.getGlobalBounds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		playerRect.top -= moveDelta;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().y > 0)
			pSprite.move(0, -moveDelta);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		playerRect.height += moveDelta;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().y + pSize.y < mapSizePixels.y)
			pSprite.move(0, moveDelta);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerRect.width += moveDelta;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().x + pSize.x < mapSizePixels.x)
			pSprite.move(moveDelta, 0);
		source.y = moveDirection;
		playerMoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerRect.left -= moveDelta;
		if (!moveIsColliding(playerRect) && pSprite.getPosition().x > 0)
			pSprite.move(-moveDelta, 0);
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

void Game::updateCam()
{
	if (pSprite.getPosition().x + pSize.x > screenDimension.x / 2)
	{
		if (pSprite.getPosition().x + pSize.x + screenDimension.x / 2 < mapSizePixels.x)
			camPosition.x = pSprite.getPosition().x + pSize.x;
	} 
	else {
		camPosition.x = screenDimension.x / 2;
	}

	if (pSprite.getPosition().y + pSize.y > screenDimension.y / 2)
	{
		if (pSprite.getPosition().y + pSize.y + screenDimension.y / 2 < mapSizePixels.y)
			camPosition.y = pSprite.getPosition().y + pSize.y;
	}
	else {
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
	//needs fix drawScreenBounds
	int delta = event.mouseWheel.delta;
	sf::Vector2i pos(event.mouseWheel.x, event.mouseWheel.y);

	if (delta > 0)
		playerView.zoom(1.2f);
	else if (delta < 0)
		playerView.zoom(0.8f);
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

//vector<Object*> Game::getObjectsAt(int x, int y) {

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

	if (itemIsSelected)
		target.draw(itemSelectedBox);

	target.setView(HUD);

	//draw highlightBox
	target.draw(highlightBox);

	target.draw(FPSText);

	target.draw(inventory);
	for each (auto item in inventoryItems)
	{
		if (item.isVisible)
			target.draw(item.sprite);
	}

	target.draw(speaker);
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