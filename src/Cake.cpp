#include "Cake.h"
#include "Board.h"

Cake::Cake(const sf::Sprite& sprite) :FixedObject(sprite)
{
}

ObjectTypes Cake::getObject() const
{
    return cakeObject;
}

void Cake::handleCollision(GameObject& gameObject)
{
    if (&gameObject == this) return;
    gameObject.handleCollision(*this);
}

void Cake::handleCollision(Pacman& gameObject)
{
    FixedObject::deleteObject();
    gameObject.handleCollision(*this);
}

