#pragma once
#include "Board.h"

class Gift :public AllGifts
{
public:
	Gift(const sf::Sprite&, const bool);
	ObjectTypes getObject() const;

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Pacman&)override;

	void doGiftTask(Controller&, std::vector<std::unique_ptr<MovingObject>>&) override;

};