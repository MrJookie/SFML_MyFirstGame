#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	Player();
	Player(Player& value);
	Player operator=(Player& value);
	~Player();

protected:
	sf::Sprite pSprite;
	sf::Texture pTexture;
	sf::Vector2f pPosition;
	float frameCounter = 0, switchFrame = 80, frameSpeed = 500;
	sf::Vector2i source;
	sf::Clock playerAnimationClock;
};

#endif