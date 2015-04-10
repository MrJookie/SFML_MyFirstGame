#include <iostream>
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Player.h"

bool moveIsColliding(sf::FloatRect playerRect, const std::vector<Object>& collisionObjects)
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

void movePlayer(sf::Sprite& player, const std::vector<Object>& collisionObjects, int direction)
{
	sf::FloatRect playerRect = player.getGlobalBounds();

	if (direction == 1) { //up
		playerRect.top -= 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			player.move(0, -1);
	}
	else if (direction == 2) { //down
		playerRect.height += 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			player.move(0, 1);
	}
	else if (direction == 3) { //right
		playerRect.width += 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			player.move(1, 0);
	}
	else if (direction == 4) { //left
		playerRect.left -= 1;
		if (!moveIsColliding(playerRect, collisionObjects))
			player.move(-1, 0);
	}
}

int main()
{
	std::cout << "Level is loading" << std::endl;

	enum Direction { Down, Left, Right, Up };

	sf::Vector2i source(1, 0);
	float frameCounter = 0, switchFrame = 80, frameSpeed = 500;

	Level level;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	sf::Clock clock;

	if (!level.loadFromFile("C:\\Users\\admin\\Desktop\\SFML_MyFirstGame\\MyFirstGame\\x64\\Debug\\map.tmx"))
	{
		std::cout << "Failed to load tile map!" << std::endl;
		return -1;
	}

	std::cout << "Level has been loaded" << std::endl;

	if (!playerTexture.loadFromFile("C:\\Users\\admin\\Desktop\\SFML_MyFirstGame\\MyFirstGame\\x64\\Debug\\player.png"))
	{
		std::cout << "Failed to load player spritesheet!" << std::endl;
		return -1;
	}

	std::vector<Object> collisionObjects = level.getAllObjects();

	std::cout << "Setting up window" << std::endl;

	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(100, 100);

	sf::Vector2f screenDimensions(800, 600);
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "First app", sf::Style::Titlebar | sf::Style::Close);
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	std::cout << "Preparing to launch" << std::endl;

	sf::View cam(sf::FloatRect(0, 0, screenDimensions.x, screenDimensions.y));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		bool playerMoved = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			source.y = Up;
			movePlayer(playerSprite, collisionObjects, 1);
			playerMoved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			source.y = Left;
			movePlayer(playerSprite, collisionObjects, 4);
			playerMoved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			source.y = Right;
			movePlayer(playerSprite, collisionObjects, 3);
			playerMoved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			source.y = Down;
			movePlayer(playerSprite, collisionObjects, 2);
			playerMoved = true;
		}

		if (playerMoved) {
			frameCounter += frameSpeed * clock.restart().asSeconds();
			if (frameCounter >= switchFrame)
			{
				frameCounter = 0;
				source.x++;
				if (source.x * 32 >= playerTexture.getSize().x)
					source.x = 0;
			}
		}

		//window.clear(sf::Color(101, 194, 151, 255));
		window.clear();

		window.setView(cam);
		level.draw(window);

		for (int i = 0; i < collisionObjects.size(); ++i)
		{
			window.draw(collisionObjects[i].sprite);
		}

		playerSprite.setTextureRect(sf::IntRect(source.x * 32, source.y * 32, 32, 32));
		window.draw(playerSprite);

		window.display();
	}

	return 0;
}