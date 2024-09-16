#include "SmartGhost.h"

SmartGhost::SmartGhost(const sf::Sprite& sprite) : Ghost(sprite)
{
}

ObjectTypes SmartGhost::getObject() const
{
    return smartGhostObject;
}

void SmartGhost::handleCollision(GameObject& gameObject)
{
    if (&gameObject == this) return;
    gameObject.handleCollision(*this);
}

void SmartGhost::handleCollision(Wall& gameObject)
{
    this->setPosition(this->getPreviousPosition());
}

void SmartGhost::handleCollision(Door& gameObject)
{
    this->m_sprite.move(getSpeed());
}

void SmartGhost::handleCollision(Pacman&)
{
    setTouchPacman(true);
}


void SmartGhost::setTouchPacman(const bool touched)
{
    m_touchPacman = touched;
}

bool SmartGhost::getTouchedPacman() const
{
    return m_touchPacman;
}


