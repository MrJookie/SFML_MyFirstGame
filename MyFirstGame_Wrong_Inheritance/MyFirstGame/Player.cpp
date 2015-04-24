#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

Player::Player()
{
	font.loadFromFile("arial.ttf");
	pTexture.loadFromFile("player.png");
	pSprite.setTexture(pTexture);
	pSprite.setPosition(400, 250);

	pSize.x = 32;
	pSize.y = 32;

	source.x = 1;
	source.y = 0;

	frameCounter = 0;
	switchFrame = 120;
	frameSpeed = 700;

	pSprite.setTextureRect(sf::IntRect(source.x * pSize.x, source.y * pSize.y, pSize.x, pSize.y));

	setLevel(17);
	setHP(30);
	setSpeed(2.0);

	initPlayerHUD();
}

Player::~Player()
{
}

void Player::initPlayerHUD()
{
	pLevelHUD.setFont(font);
	pLevelHUD.setString("lvl." + std::to_string(getLevel()) + " 97%");
	pLevelHUD.setCharacterSize(8);
	pLevelHUD.setColor(sf::Color(0, 0, 0));

	hpBarOutline.setSize(sf::Vector2f(static_cast<float> (pSize.x), 3));
	hpBarOutline.setFillColor(sf::Color(0, 255, 0, 0));
	hpBarOutline.setOutlineThickness(1);
	hpBarOutline.setOutlineColor(sf::Color(0, 0, 0, 255));

	hpBarFill.setSize(sf::Vector2f(static_cast<float> (pSize.x), 3));
}

void Player::updatePlayerHUD()
{
	hpBarOutline.setPosition(sf::Vector2f(pSprite.getPosition().x, pSprite.getPosition().y - 8));
	hpBarFill.setPosition(sf::Vector2f(pSprite.getPosition().x, pSprite.getPosition().y - 8));
	pLevelHUD.setPosition(sf::Vector2f(pSprite.getPosition().x, pSprite.getPosition().y - 20));
	
	if (pHP < 26) {
		hpBarFill.setFillColor(sf::Color(180, 0, 0, 255));
	}
	else if (pHP < 51) {
		hpBarFill.setFillColor(sf::Color(255, 51, 0, 255));
	}
	else if (pHP < 76) {
		hpBarFill.setFillColor(sf::Color(255, 204, 0, 255));
	}
	else if (pHP <= 100) {
		hpBarFill.setFillColor(sf::Color(0, 255, 0, 255));
	}

	if (pHP >= 0 && pHP <= 100)
		hpBarFill.setSize(sf::Vector2f(static_cast<float> (pSize.x * (pHP / 100.0f)), 3));
	else
		hpBarFill.setSize(sf::Vector2f(0, 3));
}

void Player::setHP(int hp)
{
	pHP = hp;

	if (pHP < 0)
		pHP = 0;
	else if (pHP > 100)
		pHP = 100;
}

int Player::getHP()
{
	return pHP;
}

void Player::setSpeed(float speed)
{
	pSpeed = speed;
}

void Player::setLevel(int level)
{
	pLevel = level;
}

int Player::getLevel()
{
	return pLevel;
}
