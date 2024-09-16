#pragma once
#include "GameObject.h"
#include "FixedObject.h"

class MovingObject : public GameObject
{
protected:
	sf::Vector2f m_previousPosition;	// previous position of the object
	sf::Vector2f m_speed;
public:

	 void setSpeed(sf::Vector2f);
	 sf::Vector2f getSpeed() const;

	MovingObject(const sf::Sprite&);

	virtual void setPreviousPosition(const sf::Vector2f&);
	sf::Vector2f getPreviousPosition() const;

    void makeMove();
	virtual ObjectTypes getObject() const = 0;
	
	 void changeColor(sf::Color);

}; 
