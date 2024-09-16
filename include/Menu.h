#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Resources.h"

class Menu
{
private:
	sf::Text m_title;
	std::vector<sf::Text> m_texts;
	sf::Sprite m_background;
	sf::RectangleShape m_backgroundShape;
	std::vector<sf::Sprite> m_menuSprites;
	std::vector<sf::Text> m_menuTexts;

	void initializeBackground(sf::RenderWindow*);
	void initializeTitle(sf::RenderWindow*, const std::string&);
	void initializeText(sf::RenderWindow*, const std::vector<std::string>&);

public:

	Menu(sf::RenderWindow*, const std::vector<std::string>&, const std::string&);

	void drawMenu(sf::RenderWindow*) const;
	std::vector<sf::Text>& getMenuText();
	sf::Sprite about(sf::RenderWindow*);
};