#include "Game.h"
#include "Level.h"

#ifndef PLAYER_H
#define PLAYER_H

enum Direction { moveDown, moveLeft, moveRight, moveUp };

class Player : public Game
{
public:
	Player();
	Player(Player& value);
	//Player operator=(Player& value);
	~Player();

	bool Player::moveIsColliding(sf::FloatRect playerRect, const std::vector<Object>& collisionObjects);
	void Player::animate();
	void Player::move();

protected:
	sf::Sprite pSprite;

private:
	sf::Texture pTexture;
	//sf::Sprite pSprite;
	sf::Vector2f pPosition;
	float frameCounter = 0, switchFrame = 80, frameSpeed = 500;
	sf::Vector2i source;
	sf::Clock clock;

	virtual void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif