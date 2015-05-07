#include <fstream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "RapidXML/rapidxml.hpp"
#include "TileMap.h"

using namespace rapidxml;

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

TileMap::TileMap(TileMap& value)
{
	tilesetImage = value.tilesetImage;

	objects = value.objects;
	layers = value.layers;
	subRects = value.subRects;

	tileWidth = value.tileWidth;
	tileHeight = value.tileHeight;

	firstTileID = value.firstTileID;
}

TileMap& TileMap::operator=(TileMap& value)
{
	tilesetImage = value.tilesetImage;

	objects = value.objects;
	layers = value.layers;
	subRects = value.subRects;

	tileWidth = value.tileWidth;
	tileHeight = value.tileHeight;

	firstTileID = value.firstTileID;

	return *this;
}

bool TileMap::loadFromFile(std::string fileName)
{
	std::ifstream mapFile(fileName);
	if (!mapFile)
	{
		throw "Error loading file " + fileName;
		return false;
	}

	rapidxml::xml_document<> doc;
	std::vector<char> buffer((std::istreambuf_iterator<char>(mapFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	xml_node<>* map;
	map = doc.first_node("map");

	width = atoi(map->first_attribute("width")->value());
	height = atoi(map->first_attribute("height")->value());
	tileWidth = atoi(map->first_attribute("tilewidth")->value());
	tileHeight = atoi(map->first_attribute("tileheight")->value());

	xml_node<>* tilesetElement;
	tilesetElement = map->first_node("tileset");
	firstTileID = atoi(tilesetElement->first_attribute("firstgid")->value());

	xml_node<>* image;
	image = tilesetElement->first_node("image");
	std::string tileSheetPath = image->first_attribute("source")->value();

	sf::Image tileSheet;
	if (!tileSheet.loadFromFile(tileSheetPath))
	{
		throw "Error loading file " + fileName;
		return false;
	}

	tileSheet.createMaskFromColor(sf::Color(109, 159, 185));
	tilesetImage.loadFromImage(tileSheet);
	tilesetImage.setSmooth(false);

	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			sf::Rect<int> rect;

			rect.top = y * tileHeight;
			rect.height = tileHeight;
			rect.left = x * tileWidth;
			rect.width = tileWidth;

			subRects.push_back(rect);
		}
	}

	xml_node<>* layerElement;
	layerElement = map->first_node("layer");
	while (layerElement)
	{
		Layer layer;
		if (layerElement->first_attribute("opacity"))
			layer.opacity = 255 * strtod(layerElement->first_attribute("opacity")->value(), 0);
		else
			layer.opacity = 255;

		xml_node<>* layerDataElement;
		layerDataElement = layerElement->first_node("data");
		if (!layerDataElement)
		{
			throw "Corrupted map file. No layer information found.";
			return false;
		}

		xml_node<>* tileElement;
		tileElement = layerDataElement->first_node("tile");
		if (!tileElement)
		{
			throw "Corrupted map file. No tile information found.";
			return false;
		}

		int x = 0;
		int y = 0;

		//resize 2D tile array
		layer.tiles.resize(width);
		for (int i = 0; i < height; ++i)
			layer.tiles[i].resize(height);

		while (tileElement)
		{
			int tileGID = atoi(tileElement->first_attribute("gid")->value());
			int GIDsubRect = tileGID - firstTileID;

			if (GIDsubRect >= 0)
			{
				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[GIDsubRect]);
				sprite.setPosition(x * tileWidth, y * tileHeight);
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

				layer.tiles[x][y] = sprite;
			}
			/*
			//very bad prevention - before was used in 1D tile array, not needed in 2D vector anymore
			else {
				sf::Sprite sprite;
				sprite.setPosition(x * tileWidth, y * tileHeight);
				sprite.setColor(sf::Color(255, 255, 255, 0));

				//layer.tiles.push_back(sprite);
				layer.tiles[x][y] = sprite;
			}
			*/

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}

			tileElement = tileElement->next_sibling("tile");
		}

		layers.push_back(layer);
		layerElement = layerElement->next_sibling("layer");
	}

	xml_node<>* objectGroupElement;
	if (map->first_node("objectgroup"))
	{
		objectGroupElement = map->first_node("objectgroup");
		while (objectGroupElement)
		{
			xml_node<>* objectElement;
			objectElement = objectGroupElement->first_node("object");
			while (objectElement)
			{
				std::string objectType;
				if (objectElement->first_attribute("type"))
					objectType = objectElement->first_attribute("type")->value();

				std::string objectName;
				if (objectElement->first_attribute("name"))
					objectName = objectElement->first_attribute("name")->value();

				float x = atof(objectElement->first_attribute("x")->value());
				float y = atof(objectElement->first_attribute("y")->value());

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(x, y);

				if (objectElement->first_attribute("width"))
				{
					width = atoi(objectElement->first_attribute("width")->value());
					height = atoi(objectElement->first_attribute("height")->value());
					sprite.setTextureRect(sf::Rect<int>(-1000, -1000, width, height));
					sprite.setColor(sf::Color(255, 255, 255, 0));
				}
				else
				{
					width = subRects[atoi(objectElement->first_attribute("gid")->value()) - firstTileID].width;
					height = subRects[atoi(objectElement->first_attribute("gid")->value()) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->first_attribute("gid")->value()) - firstTileID]);
				}

				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;
				object.isVisible = true;

				sf::Rect <int> objectRect;
				objectRect.top = y;
				objectRect.left = x;
				objectRect.height = height;
				objectRect.width = width;
				object.rect = objectRect;

				xml_node<>* properties;
				properties = objectElement->first_node("properties");
				if (properties)
				{
					xml_node<>* property;
					property = properties->first_node("property");
					if (property)
					{
						while (property)
						{
							std::string propertyName = property->first_attribute("name")->value();
							std::string propertyValue = property->first_attribute("value")->value();

							object.properties[propertyName] = propertyValue;

							property = property->next_sibling("property");
						}
					}
				}

				objects.push_back(object);

				objectElement = objectElement->next_sibling("object");
			}

			objectGroupElement = objectGroupElement->next_sibling("objectgroup");
		}
	}

	return true;
}

std::vector<Object> TileMap::getObjects(std::string name)
{
	std::vector<Object> vec;
	for (int i = 0; i < objects.size(); ++i) {
		if (objects[i].name == name)
			vec.push_back(objects[i]);
	}

	return vec;
}

std::vector<sf::Rect<int>>& TileMap::getSubRects()
{
	return subRects;
}

std::vector<Object>& TileMap::getObjects()
{
	return objects;
}

std::vector<Layer>& TileMap::getLayers()
{
	return layers;
}

sf::Vector2i TileMap::getTileSize() const
{
	return sf::Vector2i(tileWidth, tileHeight);
}

sf::Vector2i TileMap::getMapSizeTiles() const
{
	return sf::Vector2i(width, height);
}

sf::Vector2i TileMap::getMapSizePixels()
{
	return sf::Vector2i(width * tileWidth, height * tileHeight);
}
