#pragma once
#include "MovingObject.h"

class Pacman :public MovingObject
{
private:
	bool m_destroyTheDoor;
public:
	Pacman() = default;
	Pacman(const sf::Sprite&);

	ObjectTypes getObject() const;

	void setDestroyTheDoor(bool);
	bool getDestroyTheDoor();

	virtual void handleCollision(GameObject&) override;
	virtual void handleCollision(Wall&) override;
	virtual void handleCollision(Cake&) override;
	virtual void handleCollision(Timer&)override;
	virtual void handleCollision(IceBall&) override;
	virtual void handleCollision(Bomb&) override;
	virtual void handleCollision(Key&) override;
	virtual void handleCollision(Health&)override;
	virtual void handleCollision(Gift&) override;
	virtual void handleCollision(Door&) override;

};