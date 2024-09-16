#include "Menu.h"

Menu::Menu(sf::RenderWindow* window, const std::vector<std::string>& options, const std::string& string)
{
    initializeBackground(window);
    initializeTitle(window, string);
    initializeText(window, options);
}

void Menu::initializeBackground(sf::RenderWindow* window)
{
   
    m_background = sf::Sprite(Resources::instance().getTexture(11));

    m_backgroundShape = sf::RectangleShape(sf::Vector2f(1000.f, 800.f));
    m_backgroundShape.setOrigin(m_backgroundShape.getSize() / 2.f);
    m_backgroundShape.setPosition(sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f));
    m_backgroundShape.setFillColor(sf::Color(225, 225, 225, 180));
    m_backgroundShape.setOutlineColor(sf::Color::Black);
    m_backgroundShape.setOutlineThickness(1);
}

void Menu::initializeTitle(sf::RenderWindow* window, const std::string& string)
{
    m_title.setString(string);
    m_title.setFont(Resources::instance().getFont());
    m_title.setFillColor(sf::Color::Yellow);
    m_title.setOutlineColor(sf::Color::Magenta);
    m_title.setOutlineThickness(4);
    m_title.setCharacterSize(40);
    m_title.setOrigin(m_title.getGlobalBounds().width / 2.f, m_title.getGlobalBounds().height / 2.f);
    m_title.setPosition(window->getSize().x / 2.f, m_backgroundShape.getGlobalBounds().top + 100);
}

void Menu::initializeText(sf::RenderWindow* window, const std::vector<std::string>& options)
{
    for (int i = 0; i < options.size(); i++)
    {
        sf::Text text;

        text.setString(options[i]);
        text.setFont(Resources::instance().getFont());
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setOutlineColor(sf::Color(0, 171, 255, 255));
        text.setCharacterSize(40);
        text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
        text.setPosition(window->getSize().x / 2.0f, m_backgroundShape.getGlobalBounds().top + 200 + i * 100);

        m_texts.push_back(text);
    }
}

void Menu::drawMenu(sf::RenderWindow* window) const
{
    window->draw(m_background);
    window->draw(m_backgroundShape);
    window->draw(m_title);
    for (int i = 0; i < m_texts.size(); i++)
        window->draw(m_texts[i]);
}

std::vector<sf::Text>& Menu::getMenuText()
{
    return m_texts;
}

sf::Sprite Menu::about(sf::RenderWindow* window)
{
    initializeBackground(window);

    auto font = Resources::instance().getFont();

    sf::Text introduction;
    introduction.setFont(font);
    introduction.setString("The main character is the pacman which it is the character that the player can control.\n"
    "The pacman must eat all the cakes to go to the next level, and it must run away from the ghosts.\n"
    "There are two types of ghosts: \n"
    "Regular ghost: it moves in random directions.\n"
    "Smart ghostd: it follows the pacman.\n"
    "There are some gifts in the map that can help the pacman to win, these gifts are: \n"
    "The gift box: it turns the pacman into superPacman which can't be killed by the ghosts.\n"
    "For 7 seconds and it can open the doors without a key.\n"
    "The iceBall: it freezes the ghosts.\n"
    "The heart: it inceases the pacman's health.\n"
    "The clock: it gives the player extra 5 seconds for the level.\n"
    "There are keys that open the doors.\n"
    "The bomb : it decreases two lives from the pacman and it also can cause the same damage\n "
     "to the super pacman\n");

    introduction.setFillColor(sf::Color::Black);
    introduction.setCharacterSize(22);
    introduction.setOutlineThickness(2);
    introduction.setOutlineColor(sf::Color::Yellow);
    introduction.setPosition(m_backgroundShape.getGlobalBounds().left + 10.f, m_backgroundShape.getGlobalBounds().top + 10.f);
    m_menuTexts.push_back(introduction);

    sf::Sprite pacmanSprite(Resources::instance().getTexture(pacman));
    pacmanSprite.setTextureRect(sf::IntRect(PACMAN_SIZE, 0, PACMAN_SIZE, PACMAN_SIZE));
    pacmanSprite.setPosition(m_backgroundShape.getGlobalBounds().left + 10.f, m_backgroundShape.getGlobalBounds().top + 370.f);
    m_menuSprites.push_back(pacmanSprite);

    sf::Sprite smartGhostSprite(Resources::instance().getTexture(smartGhost));
    smartGhostSprite.setPosition(m_backgroundShape.getGlobalBounds().left + 10.f, m_backgroundShape.getGlobalBounds().top + 470.f);
    m_menuSprites.push_back(smartGhostSprite);

    sf::Sprite regularGhostSprite(Resources::instance().getTexture(regularGhost));
    regularGhostSprite.setPosition(m_backgroundShape.getGlobalBounds().left + 10.f, m_backgroundShape.getGlobalBounds().top + 570.f);
    m_menuSprites.push_back(regularGhostSprite);

    sf::Text pacmanText;
    pacmanText.setFont(font);
    pacmanText.setString("PACMAN : The character that the player can move.\n it must eat the cakes and run away from the ghosts");
    pacmanText.setCharacterSize(20);
    pacmanText.setFillColor(sf::Color::Yellow);
    pacmanText.setOutlineThickness(3);
    pacmanText.setPosition(m_backgroundShape.getGlobalBounds().left + 80.f, m_backgroundShape.getGlobalBounds().top + 380.f);
    m_menuTexts.push_back(pacmanText);

    sf::Text smartGhostText;
    smartGhostText.setFont(font);
    smartGhostText.setString("Smart Ghost: it follows the pacman");
    smartGhostText.setCharacterSize(20);
    smartGhostText.setFillColor(sf::Color::Magenta);
    smartGhostText.setOutlineThickness(3);
    smartGhostText.setPosition(m_backgroundShape.getGlobalBounds().left + 80.f, m_backgroundShape.getGlobalBounds().top + 480.f);
    m_menuTexts.push_back(smartGhostText);

    sf::Text regularGhostText;
    regularGhostText.setFont(font);
    regularGhostText.setString("Regular Ghost: it moves in random directions");
    regularGhostText.setCharacterSize(20);
    regularGhostText.setFillColor(sf::Color::Green);
    regularGhostText.setOutlineThickness(3);
    regularGhostText.setPosition(m_backgroundShape.getGlobalBounds().left + 80.f, m_backgroundShape.getGlobalBounds().top + 580.f);
    m_menuTexts.push_back(regularGhostText);

    sf::Text madeByText;
    madeByText.setFont(font);
    madeByText.setString("Made by: Ahmad Hirbawy & Masa Abu Dhaim \n");
    madeByText.setCharacterSize(20);
    madeByText.setFillColor(sf::Color::White);
    madeByText.setOutlineThickness(3);
    madeByText.setPosition(m_backgroundShape.getGlobalBounds().left + m_backgroundShape.getGlobalBounds().width - madeByText.getGlobalBounds().width - 10,
        m_backgroundShape.getGlobalBounds().top + m_backgroundShape.getGlobalBounds().height - madeByText.getGlobalBounds().height - 10);
    m_menuTexts.push_back(madeByText);

    sf::Sprite backSprite(Resources::instance().getTexture(backButton));
    backSprite.setPosition(m_backgroundShape.getGlobalBounds().left + 10.f, m_backgroundShape.getGlobalBounds().top + 740.f);
    m_menuSprites.push_back(backSprite);

    //draw the about 
    window->draw(m_background);
    window->draw(m_backgroundShape);
    window->draw(introduction);
    window->draw(pacmanSprite);
    window->draw(regularGhostSprite);
    window->draw(smartGhostSprite);
    window->draw(pacmanText);
    window->draw(regularGhostText);
    window->draw(smartGhostText);
    window->draw(madeByText);
    window->draw(backSprite);
    
    
    return backSprite;
}
