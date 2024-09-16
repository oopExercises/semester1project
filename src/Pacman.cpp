#include "MovingObject.h"
#include "Pacman.h"
#include "Controller.h"

Pacman::Pacman(const sf::Sprite& sprite) :MovingObject(sprite), m_destroyTheDoor(0){}

ObjectTypes Pacman::getObject() const
{
	return pacmanObject;
}

void Pacman::setDestroyTheDoor(bool destroy)
{
	m_destroyTheDoor = destroy;
}

bool Pacman::getDestroyTheDoor()
{
	return m_destroyTheDoor;
}

void Pacman::handleCollision(GameObject& gameObject)
{
	if (&gameObject == this) return;
	gameObject.handleCollision(*this);
}

void Pacman::handleCollision(Wall&)
{
	this->setPosition(this->getPreviousPosition());
}

void Pacman::handleCollision(Cake&)
{
	
	Resources::instance().playSound(eatCake);
}

void Pacman::handleCollision(Timer&)
{
	
}

void Pacman::handleCollision(IceBall&)
{
	
}

void Pacman::handleCollision(Bomb&)
{
	
}

void Pacman::handleCollision(Key&)
{
	Resources::instance().playSound(tookTheKey);
}

void Pacman::handleCollision(Health&)
{
	
	Resources::instance().playSound(healing);
}

void Pacman::handleCollision(Gift&)
{
	m_destroyTheDoor = true;
	setSpeed(sf::Vector2f(SPEED * 2, SPEED * 2));
}

void Pacman::handleCollision(Door&)
{
	this->setPosition(this->getPreviousPosition());
}


