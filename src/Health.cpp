#include "Health.h"

Health::Health(const sf::Sprite& sprite,const int extraHealth ):AllGifts(sprite,false)
{
}

ObjectTypes Health::getObject() const
{
    return healthObject;
}

void Health::handleCollision(GameObject&)
{
}

void Health::handleCollision(Pacman& gameObject)
{
    AllGifts::activateGift();
    FixedObject::deleteObject();
    gameObject.handleCollision(*this);
}

void Health::doGiftTask(Controller& controller, std::vector<std::unique_ptr<MovingObject>>& movingObjects)
{
	if (controller.getHealth() < HEALTHTOP)
	{
		controller.setHealth(1);

	}
}


