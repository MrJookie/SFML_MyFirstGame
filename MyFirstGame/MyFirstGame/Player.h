#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	Player();
	Player(Player& value);
	Player operator=(Player& value);
	~Player();
	void setHP(int hp);
	int getHP();
	void setSpeed(float speed);
	void setLevel(int level);
	int getLevel();

protected:
	void updatePlayerHUD();

	int pHP;
	int pLevel;
	float pSpeed;
	float frameCounter, switchFrame, frameSpeed;
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

private:
	void initPlayerHUD();
};

#endif