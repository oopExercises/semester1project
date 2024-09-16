#include "GameObject.h"

//constructor taking Sprite
GameObject::GameObject(const sf::Sprite& sprite) : m_sprite(sprite)
{
}

void GameObject::draw(sf::RenderWindow* window)
{
	window->draw(m_sprite);
}

sf::Sprite GameObject::getSprite() const
{
	return m_sprite;
}

void GameObject::setSprite(sf::Sprite sprite)
{
	m_sprite = sprite;
}

sf::Vector2f GameObject::getPosition() const
{
	return m_sprite.getPosition();
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_sprite.setPosition(position);
}

bool GameObject::checkCollision(const GameObject& object) const
{
	return m_sprite.getGlobalBounds().intersects(object.getSprite().getGlobalBounds());
}

