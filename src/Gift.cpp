#include "Gift.h"

Gift::Gift(const sf::Sprite& sprite,const bool superStatus) :AllGifts(sprite, false)
{
}

ObjectTypes Gift::getObject() const
{
    return giftObject;
}

void Gift::handleCollision(GameObject& gameObject)
{
    if (&gameObject == this) return;
    gameObject.handleCollision(*this);
}

void Gift::handleCollision(Pacman& gameObject)
{
    AllGifts::activateGift();
    FixedObject::deleteObject();
    gameObject.handleCollision(*this);
}

void Gift::doGiftTask(Controller& controller, std::vector<std::unique_ptr<MovingObject>>& movingObjects)
{
    controller.setPoints(GIFT_POINTS);
    controller.setSuper(true);
    Resources::instance().playSound(superGift);
}


