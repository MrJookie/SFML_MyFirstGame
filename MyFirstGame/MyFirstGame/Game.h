#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Player.h"

class Game : public Level, public Player, public sf::Transformable, public sf::Drawable
{
public:
	Game();
	Game(Game& value);
	Game(sf::Vector2f screenDimension);
	//Game operator=(Game& value);
	~Game();

	sf::Vector2f getScreenDimension();
	sf::Vector2f getPlayerCamPosition();
	void setMouseCoords(int x, int y);
	void movePlayer(int moveDirection);
	void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	int leftTileX, rightTileX, topTileY, bottomTileY;
	int FPS;
	int currentFlowerTile;
	sf::Time time;
	sf::Clock fpsClock;
	sf::Clock updateFpsClock;
	sf::Clock flowerAnimationClock;
	sf::Vector2f camPosition;
	sf::Vector2f screenDimension;
	sf::Vector2i mousePosition;
	sf::View playerView;
	sf::View HUD;
	sf::Text FPSText;
	sf::Font font;
	void initHUD();
	void setView();
	void calculateFPS();
	void updateHUD();
	void updatePlayerCam();
	void updateAnimatedFlowers();
	void setDrawingBounds();
	bool moveIsColliding(sf::FloatRect playerRect);
};

#endif