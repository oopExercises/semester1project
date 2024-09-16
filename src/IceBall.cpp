#include "IceBall.h"
#include "Board.h"

IceBall::IceBall(const sf::Sprite& sprite ) :AllGifts(sprite,false)
{
}

ObjectTypes IceBall::getObject() const
{
    return iceBallObject;
}

void IceBall::handleCollision(GameObject& gameObject)
{
    if (&gameObject == this) return;
    gameObject.handleCollision(*this);
}

void IceBall::handleCollision(Pacman& gameObject)
{
    AllGifts::activateGift();
    FixedObject::deleteObject();
    gameObject.handleCollision(*this);
}

void IceBall::doGiftTask(Controller& controller, std::vector<std::unique_ptr<MovingObject>>& movingObjects)
{
    for (size_t i = 0; i < movingObjects.size(); i++)
    {
        if (movingObjects[i].get()->getObject() != pacmanObject)
        {
            movingObjects[i].get()->changeColor(sf::Color::Blue);
            
        }
    }
    Resources::instance().playSound(freeze);
}


