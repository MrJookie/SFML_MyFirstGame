#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "RapidXML/rapidxml.hpp"

class Object
{
public:
	std::string name;
	std::string type;
	sf::Rect<int> rect;
	std::map<std::string, std::string> properties;

	sf::Sprite sprite;
};

class Layer
{
public:
	int opacity;
	std::vector<std::vector<sf::Sprite>> tiles;
};

class Level
{
public:
	Level();
	Level(Level& value);
	Level operator=(Level& value);
	~Level();

	bool loadFromFile(std::string filename);
	std::vector<Object> getObjects(std::string name);
	std::vector<Object> getAllObjects();
	void draw(sf::RenderWindow &window, int playerPosToTileX, int playerPosToTileY);
	sf::Vector2i getTileSize();
	void Level::updateAnimationTile(int tileGID);

protected:
	std::vector<Object> objects;
	std::vector<Layer> layers;

private:
	int width, height, tileWidth, tileHeight;
	int firstTileID;
	sf::Rect<float> drawingBounds;
	sf::Texture tilesetImage;
};

#endif