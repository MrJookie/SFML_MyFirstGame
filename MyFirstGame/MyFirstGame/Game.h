#ifndef PLAYER_H
#define PLAYER_H

#include "TileMap.h"

class Game : public sf::Transformable, public sf::Drawable
{
public:
	Game();
	Game(sf::Vector2f screenDimension);
	Game(Game& value);
	~Game();

	sf::Vector2f getScreenDimension();
	sf::Vector2f getPlayerCamPosition();
	void setMouseCoords(int x, int y);
	void onMouseWheelMoved(sf::Event event);
	void movePlayer(int moveDirection);
	void update();
	void setSubRects(std::vector<sf::Rect<int>>& subRects);
	void setObjects(std::vector<Object>& objects);
	void setLayers(std::vector<Layer>& layers);
	void setMapSizeTiles(sf::Vector2i mapSizeTiles);
	void setMapSizePixels(sf::Vector2i mapSizePixels);
	void setTileSize(sf::Vector2i tileSize);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<sf::Rect<int>> subRects;
	std::vector<Object> objects;
	std::vector<Layer> layers;
	sf::Vector2i mapSizeTiles;
	sf::Vector2i mapSizePixels;
	sf::Vector2i tileSize;
	int leftTileX, rightTileX, topTileY, bottomTileY;
	int FPS;
	int currentFlowersTile, currentCoinsTile;
	int pHP;
	int pLevel;
	float pSpeed;
	float frameCounter, switchFrame, frameSpeed;
	sf::Time time;
	sf::Clock fpsClock;
	sf::Clock updateFpsClock;
	sf::Clock flowersAnimationClock;
	sf::Clock coinsAnimationClock;
	sf::Vector2f camPosition;
	sf::Vector2f screenDimension;
	sf::Vector2i mousePosition;
	sf::View playerView;
	sf::View HUD;
	sf::Text FPSText;
	sf::Vector2i pSize;
	sf::Texture pTexture;
	sf::Vector2f pPosition;
	sf::Sprite pSprite;
	sf::Vector2i source;
	sf::Clock playerAnimationClock;
	sf::RectangleShape hpBarOutline;
	sf::RectangleShape hpBarFill;
	sf::Font font;
	sf::Text pLevelHUD;
	void updatePlayerHUD();
	void initPlayerHUD();
	void setView();
	void calculateFPS();
	void updateHUD();
	void updatePlayerCam();
	void updateAnimatedFlowers();
	void updateAnimatedCoins();
	void setDrawingBounds();
	bool moveIsColliding(sf::FloatRect& playerRect);
	void interactObject(Object& object);
	void setHP(int hp);
	int getHP();
	void setSpeed(float speed);
	float getSpeed();
	void setLevel(int level);
	int getLevel();
};

#endif