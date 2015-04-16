#ifndef GAME_H
#define GAME_H

#include <fstream>
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

	void movePlayer(int moveDirection);
	sf::Vector2f getScreenDimension();
	sf::Vector2f getPlayerCamPosition();
	void update();
	void setView();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Vector2f camPosition;
	sf::Vector2f screenDimension;
	sf::View playerView;
	
	void updatePlayerCam();
	bool moveIsColliding(sf::FloatRect playerRect);
};

#endif