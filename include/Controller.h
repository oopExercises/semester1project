#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Menu.h"

class Board;

class Controller
{
private:
	sf::RenderWindow* m_window;
	int m_currentLevel;
	int m_health;
	int m_points;
	bool m_super;
	bool m_freezeGhosts;

	sf::Vector2f m_currenrDirection;

	sf::Time m_time;

	bool m_levelFinished;
	bool m_gameOver;

	void startGame();
	void infoMenu(Menu&) const;
	void nextLevel(bool&);
	void endLevels();
	void handleMouseClick(const sf::Vector2f&, Menu&);

public:

	Controller();

	void run();

	int getHealth()const;
	void setHealth(const int);

	int getPoints() const;
	void setPoints(const int);
	 
	void addTime(const float);
	sf::Time getTime() const;

	void setSuper(bool);
	bool getSuper() const;

	void setFreezeGhosts(bool);
	bool getFreezeGhosts() const;

	
	sf::RenderWindow* getWindow() const;

	int getLevel() const;
	void levelWon();
	void nextLevel();
};