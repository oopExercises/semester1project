#pragma once
#include "MovingObject.h"
#include "FixedObject.h"
#include "AllGifts.h"
#include "io.h"
#include <vector>
#include <string>
#include "Resources.h"
#include "Controller.h"
#include "Pacman.h"
#include "SmartGhost.h"
#include "RegularGhost.h"
#include "Cake.h"
#include "Door.h"
#include "Gift.h"
#include "Health.h"
#include "Key.h"
#include "Timer.h"
#include "Wall.h"
#include "IceBall.h"
#include "Bomb.h"
//-------------------------------------------------------------
// Board class 
//-------------------------------------------------------------

class Controller;

class Board
{
private:
	sf::RectangleShape m_board;
	sf::Vector2f m_levelSize;
	sf::Vector2f m_levelPosition;
	sf::Sprite m_background;

	std::vector<std::unique_ptr<MovingObject>> m_movingObjects;
	std::vector<std::unique_ptr<FixedObject>> m_fixedObjects;
	std::vector<sf::Sprite> m_livesObjects;
	std::vector<sf::Vector2f> m_movingInitPos;

	bool m_tookTheKey;
	int m_cakeCounter;
	int m_eatenCakeCounter;

	sf::Clock m_superClock;
	sf::Clock m_freezingClock;
	sf::Vector2f m_pacmanCurrPos;
	void makeObject(const ObjectTypes&, const sf::Vector2f&);
	void drawInfo(Controller&);

	std::vector<std::string> buildMap(const std::string&);

	float Xposition(const int) const; //returns the x position according to the big window
	float Yposition(const int) const; //returns the y position according to the big window

	void handleCollisions(GameObject&);
	void handleBoard(Controller&, MovingObject&);

public:

	Board(const sf::RenderWindow*, const int);

	int getCakeNumber() const;
	int getEatenCake() const;

	int getGhostsNum() const;

	void draw(Controller&);
	void move(const float, const float, Controller& );
	bool checkBoardBounds(float, float, sf::RectangleShape);

	sf::Vector2f followPacman(sf::Vector2f,int, Controller);
	sf::Vector2f escapeFromPacman(sf::Vector2f, int, Controller);
	void pacmanSprite(int&, int);
	
}; 