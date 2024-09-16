#pragma once
#include "Ghost.h"

class SmartGhost :public Ghost
{

public:
	SmartGhost(const sf::Sprite&);
	ObjectTypes getObject() const;

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Wall&) override;
	virtual void handleCollision(Door&) override;
	virtual void handleCollision(Pacman&)override;


	virtual void setTouchPacman(const bool)override;
	virtual bool getTouchedPacman() const override;

};