#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "io.h"

class Pacman;
class RegularGhost;
class SmartGhost;
class Gift;
class Wall;
class Cake;
class Timer;
class IceBall;
class Key;
class Door;
class Health;
class Bomb;

class GameObject
{
protected: // to let the other class (the sons) to get access to the sprite 
	sf::Sprite m_sprite;

public:

	GameObject(const sf::Sprite&);
	virtual ~GameObject() = default;

	sf::Sprite getSprite() const;
	void setSprite(sf::Sprite);
	sf::Vector2f getPosition() const;

	void draw(sf::RenderWindow*);
	void setPosition(const sf::Vector2f&);
	virtual ObjectTypes getObject() const = 0;
	
	
	virtual void handleCollision(GameObject&) = 0;
	virtual void handleCollision(Pacman&) {};
	virtual void handleCollision(SmartGhost&) {};
	virtual void handleCollision(RegularGhost&) {};
	virtual void handleCollision(Wall&) {};
	virtual void handleCollision(Cake&) {};
	virtual void handleCollision(Timer&) {};
	virtual void handleCollision(IceBall&) {};
	virtual void handleCollision(Bomb&) {};
	virtual void handleCollision(Key&) {};
	virtual void handleCollision(Health&) {};
	virtual void handleCollision(Gift&) {};
	virtual void handleCollision(Door&) {};

	bool checkCollision(const GameObject&) const;
};