#pragma once
#include "FixedObject.h"
#include "Board.h"

class Key :public FixedObject
{
public:
	Key(const sf::Sprite&);
	ObjectTypes getObject() const;

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Pacman&)override;
	
	
};