#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

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
	std::vector<sf::Sprite> tiles;
};

class Level
{
public:
	bool loadFromFile(std::string filename);
	std::vector<Object> getObjects(std::string name);
	std::vector<Object> getAllObjects();
	void draw(sf::RenderWindow &window);
	sf::Vector2i getTileSize();

private:
	int width, height, tileWidth, tileHeight;
	int firstTileID;
	sf::Rect<float> drawingBounds;
	sf::Texture tilesetImage;
	std::vector<Object> objects;
	std::vector<Layer> layers;
};

#endif