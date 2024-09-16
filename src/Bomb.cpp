#include "Bomb.h"

Bomb::Bomb(const sf::Sprite& sprite) :AllGifts(sprite, false)
{
}

ObjectTypes Bomb::getObject() const
{
	return bombObject;
}

void Bomb::handleCollision(GameObject& gameObject)
{
	if (&gameObject == this) return;
	gameObject.handleCollision(*this);
}

void Bomb::handleCollision(Pacman& gameObject)
{
	AllGifts::activateGift();
	FixedObject::deleteObject();
	gameObject.handleCollision(*this);
}

void Bomb::doGiftTask(Controller& controller, std::vector<std::unique_ptr<MovingObject>>& movingObjects)
{
	Resources::instance().playSound(bomb);
}
