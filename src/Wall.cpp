#include "Wall.h"

Wall::Wall(const sf::Sprite& sprite) :FixedObject(sprite)
{
}

ObjectTypes Wall::getObject() const
{
    return wallObject;
}

void Wall::handleCollision(GameObject& gameObject)
{
    if (&gameObject == this) return;
	gameObject.handleCollision(*this);
}

void Wall::handleCollision(Pacman& gameObject)
{
	gameObject.handleCollision(*this);
}

void Wall::handleCollision(SmartGhost& gameObject)
{
	gameObject.handleCollision(*this);
}

void Wall::handleCollision(RegularGhost& gameObject)
{
	gameObject.handleCollision(*this);
}
