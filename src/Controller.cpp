#include "Controller.h"
#include "Board.h"

//constructor
Controller::Controller() :  m_currentLevel(0), m_window(NULL), m_levelFinished(0), m_gameOver(0), m_health(3),m_points(0),m_super(0),
m_freezeGhosts(0), m_currenrDirection(0,0)
{

}

sf::Time Controller::getTime() const
{
    return m_time;
}

void Controller::setSuper(bool super)
{
    m_super = super;
}

bool Controller::getSuper()const
{
    return m_super;
}

void Controller::setFreezeGhosts(bool freeze)
{
    m_freezeGhosts = freeze;
}

bool Controller::getFreezeGhosts() const
{
    return m_freezeGhosts;
}

sf::RenderWindow* Controller::getWindow() const
{
	return m_window;
}

void Controller::levelWon()
{
    m_levelFinished = true;
}

void Controller::nextLevel()
{
    if (m_levelFinished)
    {
        setPoints(WON_LEVEL_POINTS);//add 50 points when winning the level
        m_currentLevel++;
    }
}

void Controller::run()
{
    //creat the game window and set the frame rate limit to be  60FPS
    m_window = new sf::RenderWindow(sf::VideoMode(1600, 1000), "PACMAN", sf::Style::Close);
    Resources::instance().playMusic(); //play the background music all the time
    m_window->setFramerateLimit(60);
    std::string gameTitle = "WELCOME TO PACMAN GAME";
  
    int currentLevel = Resources::instance().getCurrentLevel();
    std::vector<std::string> settings;

    //if the player have never played before then give him the new menu
    if (currentLevel == 0)
    {
        settings = Resources::instance().newMenu();
    }

    //if the player played before and reached a level, give him the chance to continue
    else
    {
        settings = Resources::instance().continueMenu();
        m_currentLevel = currentLevel;
    }

    Menu main_menu(m_window, settings, gameTitle);
    sf::Vector2f position;

    //main loop of the game 
    while (m_window->isOpen())
    {
        auto& text = main_menu.getMenuText();

        if (m_currentLevel != 0 )
        {
            main_menu = Menu(m_window, Resources::instance().continueMenu(), gameTitle);
        }
        else if (m_currentLevel == 0 )
        {
            main_menu = Menu(m_window, Resources::instance().newMenu(), gameTitle);
        }

        m_window->clear();
        main_menu.drawMenu(m_window); //drawing the menu
        m_window->display();

        //handling mouse events 
        if (auto event = sf::Event{};  m_window->waitEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_window->close();
                break;
            case sf::Event::MouseMoved:
                position = (sf::Vector2f)sf::Mouse::getPosition(*m_window);
                break;
            case sf::Event::MouseButtonReleased:
                position = m_window->mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Button::Left:
                    handleMouseClick(position, main_menu);
                    break;
                }
                break;
            }
        }
    }
}

int Controller::getLevel() const
{
    return m_currentLevel;
}

int Controller::getPoints() const
{
    return m_points;
}

//add points
void Controller::setPoints(const int points)
{
    m_points += points;
}

//add time
void Controller::addTime(const float extraTime)
{
    m_time += sf::seconds(extraTime);
}

int Controller::getHealth() const
{
    return m_health;
}

//increase or decrease health
void Controller::setHealth(const int lives)
{
    m_health += lives;
    if (m_health >= 3)
    {
        m_health = 3;
    }

}

void Controller::startGame()
{
   
    bool backToMenu = false;
    int numOfLevels = Resources::instance().getNumberOfLevels();
    int frame = 0;
    int frameSpeed = 0;
    int row = 0;
    float deltaTime;
    sf::Clock clock;

    //as long the game isnt over (the levels isnt over) and the player  
    //didnt choose to go back to the main menu
    while (!m_gameOver && !backToMenu && m_window->isOpen() )
    {
        Board gameBoard(m_window, m_currentLevel); //initialization of the board
 
       m_levelFinished = false;
       clock.restart();
       m_time = sf::seconds(Resources::instance().getlevelDuration());

       sf::Event event;

        //as long the level wasn't won and the the time wasn't over
        while (!m_levelFinished && m_window->isOpen() && m_time.asSeconds() > 0 && m_health > 0)
        {
            m_window->clear(sf::Color::White);
            gameBoard.draw(*this);
            m_window->display();

            //updating the current time left for the level
            m_time -= clock.getElapsedTime();

            //sf::Event event;
            while (m_window->pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    m_window->close();
                    break;
                }
            }
            //restarting the clock each round to know how much time passed in each loop interaction
            deltaTime = clock.restart().asSeconds();
            frameSpeed++;
            
            frame++;
            if (frameSpeed >= 10)
            {
                frameSpeed = 0;
                frame = frame % 2;
            }
            else
                frame--;
           
            switch (event.key.code)
            {
             
            case sf::Keyboard::Up:
                row = 3;
                gameBoard.pacmanSprite(frame, row);
                gameBoard.move(0.f, -SPEED, *this);
                m_currenrDirection = sf::Vector2f(0, -SPEED);
                break;
            case sf::Keyboard::Down:
                row = 1;
                gameBoard.pacmanSprite(frame, row);
                gameBoard.move(0.f, SPEED, *this);
                m_currenrDirection = sf::Vector2f(0, SPEED);
                break;
            case sf::Keyboard::Right:
                row = 0;
                gameBoard.pacmanSprite(frame, row);
                gameBoard.move(SPEED, 0.f, *this);
                m_currenrDirection = sf::Vector2f(SPEED,0);
                break;
            case sf::Keyboard::Left:
                row = 2;
                gameBoard.pacmanSprite(frame, row);
                gameBoard.move(-SPEED, 0.f, *this);
                m_currenrDirection = sf::Vector2f(-SPEED, 0);
                break;
            default:
                gameBoard.pacmanSprite(frame ,row);
                gameBoard.move(m_currenrDirection.x, m_currenrDirection.y, *this);
                break;
            }
        }

        //checks if the level won, if so increase the level number
        nextLevel();

        //if the levels are not over we will go to the main menu
        if (m_currentLevel == numOfLevels && m_window->isOpen() && m_health > 0)
        {
            endLevels();
        }
        //if the levels are over we will go to the end game menu
        else if (m_window->isOpen())
        {
            nextLevel(backToMenu);
        }
    }
}

void Controller::infoMenu(Menu& menu) const
{
    bool back = false;

    while (m_window->isOpen() && !back)
    {
        m_window->clear();
        auto go_back = menu.about(m_window);
        m_window->display();

        if (auto event = sf::Event{};  m_window->waitEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_window->close();
                break;

            case sf::Event::MouseButtonReleased:
                auto position = m_window->mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Button::Left:
                    //go back to main menu
                    if (go_back.getGlobalBounds().contains(position))
                    {
                        Resources::instance().playSound(menuClick);
                        back = true;
                    }
                    break;
                }
                break;
            }
        }
    }
}

void Controller::nextLevel(bool& bakeToMenu)
{
    std::string title;
    std::vector<std::string> options;

    //if the level won, updates the current level number and saves it to the file
    if (m_levelFinished)
    {
        title = "THE PACMAN ATE ALL CAKES ";
        options.push_back("NEXT LEVEL");
        Resources::instance().saveCurentLevel(m_currentLevel);
    }
    //if the time is over, can try again or exit to main menu
    else
    { 
        m_health = 3;
        m_points = 0;
        Resources::instance().playSound(pacmanDeath);
        title = "GAME OVER, TRY AGAIN";
        options.push_back("TRY AGAIN");
    }
    setFreezeGhosts(false);
    setSuper(false);
    options.push_back("BACK TO MENU");

    //creates the menu
    Menu mainMenu(m_window, options, title);

    sf::Vector2f position;

    bool continueGame = false;

    while (!continueGame && m_window->isOpen())
    {
        m_window->clear();
        mainMenu.drawMenu(m_window);
        m_window->display();

        sf::Event event;
        while (m_window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_window->close();
                break;
            case sf::Event::MouseMoved:
                position = (sf::Vector2f)sf::Mouse::getPosition(*m_window);
                break;
            case sf::Event::MouseButtonReleased:
                position = (sf::Vector2f)sf::Mouse::getPosition(*m_window);

                switch (event.mouseButton.button)
                {
                case sf::Mouse::Button::Left:
                    auto& text = mainMenu.getMenuText();
                    //try again/next level
                    if (text[0].getGlobalBounds().contains(position))
                    {
                        Resources::instance().playSound(menuClick);
                        continueGame = true;
                    }
                    //back to menu
                    if (text[1].getGlobalBounds().contains(position))
                    {
                        Resources::instance().playSound(menuClick);
                        bakeToMenu = true;
                        continueGame = true;
                    }
                    break;
                }
                break;
            }
        }
    }
}

void Controller::endLevels()
{
    //game over is true so will not load another level
    m_gameOver = true;
    //updating the current level to 0 for if the player would choose to start another game
    m_currentLevel = 0;
    //saving the current level
    Resources::instance().saveCurentLevel(m_currentLevel);

    std::string title = "YOU WIN";
    std::vector<std::string> options = { "BACK TO MENU", "EXIT" };

    //creating the menu
    Menu endMenu(m_window, options, title);

    sf::Vector2f position;

    bool back = false;
    
    while (!back && m_window->isOpen())
    {
        m_window->clear();
        endMenu.drawMenu(m_window);
        m_window->display();

        sf::Event event;
        while (m_window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_window->close();
                break;
            case sf::Event::MouseMoved:
                position = (sf::Vector2f)sf::Mouse::getPosition(*m_window);
                break;
            case sf::Event::MouseButtonReleased:
                position = (sf::Vector2f)sf::Mouse::getPosition(*m_window);

                switch (event.mouseButton.button)
                {
                case sf::Mouse::Button::Left:
                    auto& text = endMenu.getMenuText();
                    //back to main menu
                    if (text[0].getGlobalBounds().contains(position))
                    {
                       Resources::instance().playSound(menuClick);
                        back = true;
                    }
                    //exit game
                    if (text[1].getGlobalBounds().contains(position))
                    {
                       Resources::instance().playSound(menuClick);
                        m_window->close();
                    }
                    break;
                }
                break;
            }
        }
    }
}

void Controller::handleMouseClick(const sf::Vector2f& location, Menu& menu)
{
    auto& text = menu.getMenuText();

    for (int i = 0; i < text.size(); i++)
    {
        if (text[i].getGlobalBounds().contains(location))
        {
            if (text.size() == 3)
                i++;

            switch (i)
            {
                //continue previous game
            case 0:
                Resources::instance().playSound(menuClick);
                startGame();
                break;
                //start new game
            case 1:
                Resources::instance().playSound(menuClick);
                m_currentLevel = 0;
                m_gameOver = false;
                startGame();
                break;
                //about
            case 2:
                Resources::instance().playSound(menuClick);
                infoMenu(menu);
                break;
                //exit the game
            case 3:
                Resources::instance().playSound(menuClick);
                m_window->close();
                break;
            }
        }
    }
}
