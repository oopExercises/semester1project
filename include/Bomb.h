#pragma once
#include "Board.h"

class Bomb :public AllGifts
{
public:
	Bomb(const sf::Sprite&);
	ObjectTypes getObject() const;

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Pacman&)override;

	void doGiftTask(Controller&, std::vector<std::unique_ptr<MovingObject>>&) override;
};