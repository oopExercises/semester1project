#pragma once
#include "FixedObject.h"
#include "Controller.h"
#include "MovingObject.h"

class AllGifts : public FixedObject
{
private:
	bool m_activated;

public:
	AllGifts(const sf::Sprite&, const bool);

	void activateGift();
	bool getActivated();
	
	virtual ObjectTypes getObject() const = 0;
	virtual void doGiftTask(Controller&, std::vector<std::unique_ptr<MovingObject>>&) = 0;
};