#pragma once
#include "FixedObject.h"

class Cake :public FixedObject
{
public:
	Cake(const sf::Sprite&);
	ObjectTypes getObject() const;

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Pacman&)override;


};