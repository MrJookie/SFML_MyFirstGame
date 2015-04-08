#include <iostream>
#include <SFML/Graphics.hpp>
//#include <Box2D/Box2D.h>
#include "AnimatedSprite.h"
#include "Level.h"

int main()
{
	sf::Vector2i screenDimensions(1024, 768);
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "First app", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.clear(sf::Color(101, 194, 151, 255));

	Level level1;
	if (!level1.LoadFromFile("I:\\Files\\C++\\MyFirstGame\\x64\\Debug\\map.tmx"))
	{
		std::cout << "Failed to load tile map!" << std::endl;
		return -1;
	}

	sf::Texture texture;
	if (!texture.loadFromFile("I:\\Files\\C++\\MyFirstGame\\x64\\Debug\\actor1.png"))
	{
		std::cout << "Failed to load player spritesheet!" << std::endl;
		return -1;
	}

	sf::Texture player;
	if (!player.loadFromFile("I:\\Files\\C++\\MyFirstGame\\x64\\Debug\\player2.png"))
	{
		std::cout << "Failed to load player spritesheet!" << std::endl;
		return -1;
	}
	sf::Sprite playerSprite;
	playerSprite.setTexture(player);

	std::cout << "height: " << playerSprite.getGlobalBounds().height << std::endl;
	std::cout << "top: " << playerSprite.getGlobalBounds().top << std::endl;
	std::cout << "right: " << playerSprite.getGlobalBounds().width << std::endl;
	std::cout << "left: " << playerSprite.getGlobalBounds().left << std::endl;

	sf::View cam(sf::FloatRect(0, 0, screenDimensions.x, screenDimensions.y));

	Animation walkingAnimationDown;
	walkingAnimationDown.setSpriteSheet(texture);
	walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(64, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(0, 0, 32, 32));

	Animation walkingAnimationLeft;
	walkingAnimationLeft.setSpriteSheet(texture);
	walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
	walkingAnimationLeft.addFrame(sf::IntRect(64, 32, 32, 32));
	walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
	walkingAnimationLeft.addFrame(sf::IntRect(0, 32, 32, 32));

	Animation walkingAnimationRight;
	walkingAnimationRight.setSpriteSheet(texture);
	walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
	walkingAnimationRight.addFrame(sf::IntRect(64, 64, 32, 32));
	walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
	walkingAnimationRight.addFrame(sf::IntRect(0, 64, 32, 32));

	Animation walkingAnimationUp;
	walkingAnimationUp.setSpriteSheet(texture);
	walkingAnimationUp.addFrame(sf::IntRect(32, 96, 32, 32));
	walkingAnimationUp.addFrame(sf::IntRect(64, 96, 32, 32));
	walkingAnimationUp.addFrame(sf::IntRect(32, 96, 32, 32));
	walkingAnimationUp.addFrame(sf::IntRect(0, 96, 32, 32));

	Animation* currentAnimation = &walkingAnimationDown;

	AnimatedSprite animatedSprite(sf::seconds(0.2), true, false);
	animatedSprite.setPosition(sf::Vector2f(screenDimensions / 2));

	sf::Clock frameClock;

	float speed = 75.f;
	bool noKeyWasPressed = true;

	/*
	sf::Text FPSText;
	sf::Font font;
	if (!font.loadFromFile("I:\\Files\\C++\\MyFirstGame\\x64\\Debug\\arial.ttf"))
		return -1;
	FPSText.setFont(font);
	FPSText.setString("Text");
	*/

	std::vector<Object> block;
	block = level1.GetObjects("Pokeball");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		sf::Time frameTime = frameClock.restart();
		sf::Vector2f movement(0.f, 0.f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			currentAnimation = &walkingAnimationUp;
			movement.y -= speed;
			noKeyWasPressed = false;

			sf::Vector2f characterPosition = animatedSprite.getPosition();
			std::cout << "x: " << characterPosition.x << " y: " << characterPosition.y << std::endl;

			//cam.setCenter(static_cast<int> (characterPosition.x), static_cast<int> (characterPosition.y));
			cam.setCenter(characterPosition.x, characterPosition.y);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			currentAnimation = &walkingAnimationLeft;
			movement.x -= speed;
			noKeyWasPressed = false;

			sf::Vector2f characterPosition = animatedSprite.getPosition();
			std::cout << "x: " << characterPosition.x << " y: " << characterPosition.y << std::endl;

			//cam.setCenter(static_cast<int> (characterPosition.x), static_cast<int> (characterPosition.y));
			cam.setCenter(characterPosition.x, characterPosition.y);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			currentAnimation = &walkingAnimationDown;
			movement.y += speed;
			noKeyWasPressed = false;

			sf::Vector2f characterPosition = animatedSprite.getPosition();
			std::cout << "x: " << characterPosition.x << " y: " << characterPosition.y << std::endl;

			//cam.setCenter(static_cast<int> (characterPosition.x), static_cast<int> (characterPosition.y));
			cam.setCenter(characterPosition.x, characterPosition.y);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			currentAnimation = &walkingAnimationRight;
			movement.x += speed;
			noKeyWasPressed = false;

			sf::Vector2f characterPosition = animatedSprite.getPosition();
			std::cout << "x: " << characterPosition.x << " y: " << characterPosition.y << std::endl;

			//cam.setCenter(static_cast<int> (characterPosition.x), static_cast<int> (characterPosition.y));
			cam.setCenter(characterPosition.x, characterPosition.y);
		}

		animatedSprite.play(*currentAnimation);
		animatedSprite.move(movement * frameTime.asSeconds());

		if (noKeyWasPressed)
		{
			animatedSprite.stop();
		}
		noKeyWasPressed = true;
		animatedSprite.update(frameTime);

		//window.clear(sf::Color(101, 194, 151, 255));
		window.clear();

		window.setView(cam);

		level1.Draw(window);

		for (int i = 0; i < block.size(); ++i)
		{
			window.draw(block[i].sprite);
		}

		window.draw(animatedSprite);

		window.display();
	}

	return 0;
}