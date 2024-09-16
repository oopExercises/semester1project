#pragma once
#include "FixedObject.h"
#include "Board.h"

class Wall : public FixedObject
{
public:
	Wall(const sf::Sprite&);
	ObjectTypes getObject() const;

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Pacman&) override;
	virtual void handleCollision(SmartGhost&) override;
	virtual void handleCollision(RegularGhost&) override;

};