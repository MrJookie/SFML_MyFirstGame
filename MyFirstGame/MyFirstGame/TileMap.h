#ifndef TILEMAP_H
#define TILEMAP_H

class Object
{
public:
	std::string name;
	std::string type;
	sf::Rect<int> rect;
	std::map<std::string, std::string> properties;
	sf::Sprite sprite;
	bool isVisible;
};

class Layer
{
public:
	int opacity;
	std::vector<std::vector<sf::Sprite>> tiles;
};

class TileMap
{
public:
	TileMap();
	TileMap(TileMap& value);
	TileMap& operator=(TileMap& value);
	~TileMap();

	bool loadFromFile(std::string fileName);
	std::vector<sf::Rect<int>>& getSubRects();
	std::vector<Object> getObjects(std::string name);
	std::vector<Object>& getObjects();
	std::vector<Layer>& getLayers();
	sf::Vector2i getMapSizeTiles() const;
	sf::Vector2i getMapSizePixels();
	sf::Vector2i getTileSize() const;

private:
	int width, height, tileWidth, tileHeight;
	int firstTileID;
	sf::Texture tilesetImage;
	std::vector<sf::Rect<int>> subRects;
	std::vector<Object> objects;
	std::vector<Layer> layers;
};

#endif