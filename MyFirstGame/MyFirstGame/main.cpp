#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Level.h"
#include "Player.h"

int main()
{
	std::cout << "Level is loading" << std::endl;

	Game game;
	Level level;
	Player player;

	sf::Sprite playerSprite;
	sf::Texture playerTexture;

	playerTexture.loadFromFile("player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(100, 100);


	//manage loadFromFile
	if (!level.loadFromFile("map.tmx"))
	{
		std::cout << "Failed to load tile map!" << std::endl;
		return -1;
	}

	//std::vector<Object> collisionObjects = level.getAllObjects();

	//game.collisionObjects = level.getAllObjects();
	game.setCollisionObject(level.getAllObjects());

	sf::Vector2f screenDimensions(1024, 768);
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "First app", sf::Style::Titlebar | sf::Style::Close);
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(70);
	window.setKeyRepeatEnabled(false);

	sf::View cam(sf::FloatRect(0, 0, screenDimensions.x, screenDimensions.y));
	cam.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	sf::Vector2f camPosition(screenDimensions.x / 2, screenDimensions.y / 2);

	sf::View HUD;

	sf::Text FPSText;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return -1;
	FPSText.setFont(font);
	FPSText.setString("Text");

	while (window.isOpen())
	{
		//sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		//std::cout << "pixelPos x: " << pixelPos.x << " pixelPos y: " << pixelPos.y << std::endl;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}


		//window.clear(sf::Color(101, 194, 151, 255));
		window.clear();
		window.setView(cam);

		player.move();

		//remove playerSprite from here
		if (playerSprite.getPosition().x + 32 > screenDimensions.x / 2)
			camPosition.x = playerSprite.getPosition().x + 32;
		else
			camPosition.x = screenDimensions.x / 2;

		if (playerSprite.getPosition().y + 32 > screenDimensions.y / 2) {
			if (playerSprite.getPosition().y + 32 < 8000)
				camPosition.y = playerSprite.getPosition().y + 32;
		}
		else
			camPosition.y = screenDimensions.y / 2;

		cam.setCenter(camPosition);

		//move playerPosToTileX, playerPosToTileY to Level class
		int playerPosToTileX = playerSprite.getPosition().x / 32;
		int playerPosToTileY = playerSprite.getPosition().y / 32;
		level.draw(window, playerPosToTileX, playerPosToTileY);


		window.draw(game);
		window.draw(player);

		window.setView(HUD);
		window.draw(FPSText);

		window.display();
	}

	return 0;
}