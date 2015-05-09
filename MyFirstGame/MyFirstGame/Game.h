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
	void toggleInventory();
	void playMusic();
	void toggleMusic();
	void recalculateHighlightBox(sf::RenderTarget& window, sf::Vector2i mousePos);
	void recalculateHighlightBox(sf::Vector2i mousePosition);
	bool removeInventoryItem();
	bool selectItem(sf::RenderTarget& window, sf::Vector2i mousePosition);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	bool isInventoryActive;
	bool inventorySlotUsed[5];
	Object inventoryItems[5];
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
	int pMana;
	int pLevel;
	int pLevelExp;
	int pSpeed;
	float frameCounter, switchFrame, frameSpeed;
	bool itemIsSelected;
	bool musicIsPlaying;
	bool musicIsPaused;
	sf::Music music;
	sf::SoundBuffer soundBuffer;
	sf::Sound pickItem;
	sf::Time time;
	sf::Time elapsedTimeStep;
	sf::Clock timeStepClock;
	sf::Clock fpsClock;
	sf::Clock updateFpsClock;
	sf::Clock flowersAnimationClock;
	sf::Clock coinsAnimationClock;
	sf::Vector2f camPosition;
	sf::Vector2f screenDimension;
	sf::Vector2f mousePosition;
	sf::View playerView;
	sf::View HUD;
	sf::View minimapView;
	sf::Font font;
	sf::Text pLevelHUD;
	sf::Text FPSText;
	sf::Vector2i pSize;
	sf::Texture pTexture;
	sf::Vector2f pPosition;
	sf::Sprite pSprite;
	sf::Vector2i source;
	sf::Sprite speaker;
	sf::Texture textureSpeakerOn;
	sf::Texture textureSpeakerOff;
	sf::Clock playerAnimationClock;
	sf::RectangleShape hpBarOutline;
	sf::RectangleShape hpBarFill;
	sf::RectangleShape manaBarOutline;
	sf::RectangleShape manaBarFill;
	sf::RectangleShape minimapOutline;
	sf::RectangleShape minimapFill;
	sf::Texture inventoryTex;
	sf::Sprite inventory;
	sf::Vector2i selectionStartPosition;
	sf::Vector2f selectionCurrentPosition;
	sf::RectangleShape highlightBox;
	sf::RectangleShape itemSelectedBox;
	template <typename T> size_t getSumVisibleObjectsByName(std::vector<T>& vec, std::string objectName);
	void printSumVisibleObjectsLeft(Object& object);
	void initHUD();
	void setView();
	void calculateFPS();
	void updateHUD();
	void updateCam();
	void updateAnimatedFlowers();
	void updateAnimatedCoins();
	void setDrawingBounds();
	bool moveIsColliding(sf::FloatRect& playerRect);
	bool addToInventory(Object& object);
	bool useInventoryItem(std::string objectName);
	void interactObject(Object& object);
	void setHP(int hp);
	void setMana(int mana);
	void setSpeed(float speed);
	void setLevel(int level);
	void setLevelExp(int exp);
	int getHP();
	int getMana();
	float getSpeed();
	int getLevel();
	int getLevelExp();
	void loadShaders();
	sf::Shader transparencyShader;
	sf::RenderStates minimapTransparentState;
	std::string fragmentShaderTransparency;
};

#endif