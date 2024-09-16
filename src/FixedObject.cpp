#include "FixedObject.h"

FixedObject::FixedObject(const sf::Sprite& sprite) : GameObject(sprite)
{
    m_delete = false;
}

bool FixedObject::getDelete()
{
    return m_delete;
}

void FixedObject::deleteObject()
{
    m_delete = true;
}
