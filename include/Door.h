#pragma once
#include "FixedObject.h"
#include "Board.h"

class Door :public FixedObject
{
public:
	Door(const sf::Sprite&);
	ObjectTypes getObject() const;

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Pacman&) override;
	virtual void handleCollision(RegularGhost&) override;
	virtual void handleCollision(SmartGhost&) override;

};