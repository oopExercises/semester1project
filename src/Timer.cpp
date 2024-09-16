#include "Timer.h"


Timer::Timer(const sf::Sprite& sprite, const int extraTime) : AllGifts(sprite,false)
{
}

ObjectTypes Timer::getObject() const
{
    return timerObject;
}

void Timer::handleCollision(GameObject& gameObject)
{
	gameObject.handleCollision(*this);
}

void Timer::handleCollision(Pacman&  gameObject)
{
	AllGifts::activateGift();
	FixedObject::deleteObject();
	gameObject.handleCollision(*this);
}

void Timer::doGiftTask(Controller& controller, std::vector<std::unique_ptr<MovingObject>>&)
{
	controller.addTime(EXTRA_TIME);
	Resources::instance().playSound(timerUp);
}

