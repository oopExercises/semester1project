#include "RegularGhost.h"

RegularGhost::RegularGhost(const sf::Sprite& sprite) : Ghost(sprite)
{
    //the regular ghost can move up down or left right(randomly)
    int direction = rand();

    if(direction%2 == 0)
    m_speed = sf::Vector2f(SPEED, 0);

    else if(direction%2 == 1)
    m_speed = sf::Vector2f(0, SPEED);

}


ObjectTypes RegularGhost::getObject() const
{
	return regularGhostObject;
}

void RegularGhost::handleCollision(GameObject& gameObject)
{
    if (&gameObject == this) return;
    gameObject.handleCollision(*this);
}

void RegularGhost::handleCollision(Wall& gameObject)
{
    this->setPosition(this->getPreviousPosition());
    setSpeed(sf::Vector2f(-getSpeed().x, -getSpeed().y));
}


void RegularGhost::handleCollision(Door& gameObject)
{
    this->setPosition(this->getPreviousPosition());
    setSpeed(sf::Vector2f(-getSpeed().x, -getSpeed().y));
}

void RegularGhost::handleCollision(Pacman& gameObject)
{
    setTouchPacman(true);
}

void RegularGhost::setTouchPacman(const bool touched)
{
    m_touchPacman = touched;
}

bool RegularGhost::getTouchedPacman()const
{
    return m_touchPacman;
}



