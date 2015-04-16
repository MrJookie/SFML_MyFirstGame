#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Level.h"
#include "Player.h"

int main()
{
	std::cout << "Level is loading" << std::endl;

	sf::Vector2f screenDimension(1024, 768);

	sf::RenderWindow window(
		sf::VideoMode(screenDimension.x, screenDimension.y),
		"First app",
		sf::Style::Titlebar | sf::Style::Close
	);

	Game game(screenDimension);
	Player player;

	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(200);
	window.setKeyRepeatEnabled(false);

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
			if (event.type == sf::Event::MouseMoved)
			{
				//std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
				//std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			game.movePlayer(3);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			game.movePlayer(0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			game.movePlayer(2);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			game.movePlayer(1);
		}

		window.clear();

		window.draw(game);

		window.setView(HUD);
		window.draw(FPSText);

		window.display();
	}

	return 0;
}