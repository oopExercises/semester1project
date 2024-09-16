#pragma once
#include "MovingObject.h"

class Ghost :public MovingObject
{
protected:
	bool m_touchPacman;
public:
	Ghost(const sf::Sprite&);
	
	virtual void setTouchPacman(bool)  = 0;
	virtual bool getTouchedPacman()const = 0;

};