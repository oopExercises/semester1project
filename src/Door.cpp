#include "Door.h"

Door::Door(const sf::Sprite& sprite) :FixedObject(sprite)
{
}

ObjectTypes Door::getObject() const
{
    return doorObject;
}

void Door::handleCollision(GameObject& gameObject)
{

	if (&gameObject == this) return;
	gameObject.handleCollision(*this);
}

void Door::handleCollision(Pacman& gameObject)
{
	if (gameObject.getDestroyTheDoor())
	{
		FixedObject::deleteObject();
		gameObject.handleCollision(*this);
	}
	else
	gameObject.handleCollision(*this);
}

void Door::handleCollision(RegularGhost& gameObject)
{
	gameObject.handleCollision(*this);
}

void Door::handleCollision(SmartGhost& gameObject)
{
	gameObject.handleCollision(*this);
}
