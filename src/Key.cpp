#include "Key.h"
#include "Board.h"

Key::Key(const sf::Sprite& sprite):FixedObject(sprite)
{
}

ObjectTypes Key::getObject() const
{
    return keyObject;
}

void Key::handleCollision(GameObject& gameObject)
{
    if (&gameObject == this) return;
    gameObject.handleCollision(*this);
}

void Key::handleCollision(Pacman& gameObject)
{
    FixedObject::deleteObject();
    gameObject.handleCollision(*this);
}

