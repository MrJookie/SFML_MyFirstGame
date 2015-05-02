#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Game.h"

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

	sf::Clock frameClock;

	TileMap level;
	TileMap level2;
	Game game(screenDimension);

	try {
		level.loadFromFile("map.tmx");
		level2.loadFromFile("map2.tmx");
	}
	catch (std::string err) {
		std::cout << err << std::endl;
		return 1;
	}

	//level = level2;

	game.setSubRects(level.getSubRects());
	game.setObjects(level.getObjects());
	game.setLayers(level.getLayers());

	game.setMapSizeTiles(level.getMapSizeTiles());
	game.setMapSizePixels(level.getMapSizePixels());
	game.setTileSize(level.getTileSize());

	std::cout << "Map width: " << level.getMapSizeTiles().x << std::endl;
	std::cout << "Map height: " << level.getMapSizeTiles().y << std::endl;
	std::cout << "Objects loaded: " << level.getObjects().size() << std::endl;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
			else if (event.type == sf::Event::MouseWheelMoved)
				game.onMouseWheelMoved(event);
			else if (event.type == sf::Event::MouseMoved)
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

		frameClock.restart().asSeconds();

		game.update();

		window.clear();
		window.draw(game);
		window.display();

		window.setTitle("MyFirstGame " + std::to_string(1.f / frameClock.getElapsedTime().asSeconds()));
	}

	return 0;
}