#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Level.h"
#include "Player.h"

int main()
{
	std::cout << "Level is loading" << std::endl;

	sf::Vector2f screenDimension(800, 600);

	sf::RenderWindow window(
		sf::VideoMode(static_cast<int>(screenDimension.x), static_cast<int>(screenDimension.y)),
		"MyFirstGame",
		sf::Style::Titlebar | sf::Style::Close
	);

	//window.setMouseCursorVisible(false);
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	Game game(screenDimension);
	Player player;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
			if (event.type == sf::Event::MouseMoved)
			{
				game.setMouseCoords(event.mouseMove.x, event.mouseMove.y);
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

		game.update();

		window.clear();
		window.draw(game);
		window.display();
	}

	return 0;
}