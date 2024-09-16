#include "MovingObject.h"

void MovingObject::setSpeed(sf::Vector2f speed)
{
	m_speed = speed;
}

sf::Vector2f MovingObject::getSpeed() const
{
	return m_speed;
}

MovingObject::MovingObject(const sf::Sprite& sprite) : GameObject(sprite), m_previousPosition(0.f, 0.f), m_speed(0,SPEED)
{
}

void MovingObject::setPreviousPosition(const sf::Vector2f& position)
{
	m_previousPosition = position;
}

sf::Vector2f MovingObject::getPreviousPosition() const
{
	return m_previousPosition;
}

void MovingObject::makeMove()
{
	this->m_sprite.move(getSpeed());
}

void MovingObject::changeColor(sf::Color color)
{
	m_sprite.setColor(color);
}
