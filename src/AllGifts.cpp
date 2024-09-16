#include "AllGifts.h"

AllGifts::AllGifts(const sf::Sprite& sprite, const bool status) : FixedObject(sprite), m_activated(status)
{
}

void AllGifts::activateGift()
{
	m_activated = true;
}

bool AllGifts::getActivated()
{
	return m_activated;
}
