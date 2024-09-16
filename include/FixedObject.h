#pragma once
#include "GameObject.h"

//class GameObject;

class FixedObject : public GameObject
{
private:
	bool m_delete;// will be deleted if true

public:

	FixedObject(const sf::Sprite&);

	bool getDelete();
	void deleteObject();

	virtual ObjectTypes getObject() const = 0;
};