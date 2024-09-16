#include "Board.h"

Board::Board(const sf::RenderWindow* window, const int lvl)
{
	//background image
	m_background.setTexture(Resources::instance().getTexture(pacmanBackground));

	m_tookTheKey = false;
	m_eatenCakeCounter = 0;
	
	//build the level
	auto map = Resources::instance().buildMap(lvl);
	int numOfCols = (int)map[0].size();
	int numOfRows = (int)map.size();

	//to draw the map inside the big window(the main window),the size of the map 
	//depends on the size of the level in the text file
	m_levelSize = sf::Vector2f((float)numOfCols * TILE_SIZE, (float)numOfRows * TILE_SIZE);

	//set the position of the level in the center of the big window
	m_levelPosition = sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f);

	//to put a frame around the level
	m_board = sf::RectangleShape(m_levelSize);

	//make the color of the board half transparent
	m_board.setFillColor(sf::Color(255, 255, 255, 128));

	//set the center of the board to be the center of the big window
	m_board.setOrigin(m_levelSize / 2.f);
	m_board.setPosition(m_levelPosition);

	//health sprite
	auto& textures = Resources::instance().getTextures();
	sf::Sprite lives;
	lives.setTexture(textures[health]);

	for (size_t i = 0; i < 3; i++)
	{
		m_livesObjects.push_back(lives);
	}

	//read the chars from the levels file
	for (int row = 0; row < numOfRows; row++)
		for (int col = 0; col < numOfCols; col++)
		{
			char tile = map[row][col];

			makeObject((ObjectTypes)tile, sf::Vector2f(Xposition(col), Yposition(row)));
		}
	m_cakeCounter = getCakeNumber();
}

//returns the number of the cakes
int Board::getCakeNumber() const
{
	int x = 0;

	for (size_t i = 0; i < m_fixedObjects.size(); i++)
	{
		if (m_fixedObjects[i].get()->getObject() == cakeObject)
			x++;
	}
	return x;
}

//draws the objects
void Board::draw(Controller& controller)
{
	auto window = controller.getWindow();

	window->draw(m_background);	//background
	drawInfo(controller);		//info bar
	window->draw(m_board);		//game board

	for (auto& object : m_fixedObjects)
		object->draw(window);	//all the fixed objects
	for (auto& object : m_movingObjects)
		object->draw(window);	//all the moving objects
}

//moves the moving objects if the movement is legal, the pacman moves 
//according to the button that the user press
//the smart ghosts try to follow the pacman if its not super
//if the pacman is super then the ghosts run away
//the regular ghosts move up down or left right(randomly)
void Board::move(const float x, const float y, Controller& controller)
{
	//saves the previous position. all the collisions handeled after that
	for (size_t i = 0; i < m_movingObjects.size(); i++)
	{
		m_movingObjects[i]->setPreviousPosition(m_movingObjects[i]->getPosition());

		//all the collisions handled in the way that first we do the move and then we check if the move
		//was legal, if not, the object goes back to his previous position

		//if the object is pacman
		if (m_movingObjects[i].get()->getObject() == pacmanObject )
		{
			//save the current position of the pacman
			m_pacmanCurrPos = m_movingObjects[i].get()->getPosition();

			if (!controller.getSuper())
			{
				//check if the next move will be out of the board, if yes dont move
				if (checkBoardBounds(m_pacmanCurrPos.x + x, m_pacmanCurrPos.y + y, m_board))
				{
					m_movingObjects[i].get()->setSpeed(sf::Vector2f(x, y));
				}
				else
				{
					m_movingObjects[i].get()->setSpeed(sf::Vector2f(0, 0));
			    }

			}

			else if (controller.getSuper())
				{
				if (checkBoardBounds(m_pacmanCurrPos.x + 2 * x, m_pacmanCurrPos.y + 2 * y, m_board))
				{
					m_movingObjects[i].get()->setSpeed(sf::Vector2f(2 * x, 2 * y));
				}
				else
				{
					m_movingObjects[i].get()->setSpeed(sf::Vector2f(0, 0));
				}
			}
			//move the pacman
			m_movingObjects[i]->makeMove();
			
		}

		//if the object is regular ghost and not freezed then move
		else if (m_movingObjects[i].get()->getObject() == regularGhostObject && !controller.getFreezeGhosts())
		{
				m_movingObjects[i]->setSpeed(m_movingObjects[i]->getSpeed());
				m_movingObjects[i]->makeMove();
			
		}

		//if the object is smart ghost and not freezed then check which is the best movement to
		//reach the pacman and move,if its super then run away
		else if(m_movingObjects[i].get()->getObject() == smartGhostObject && !controller.getFreezeGhosts())
		{
			if (!controller.getSuper())
			{
				m_movingObjects[i]->setSpeed(followPacman(m_pacmanCurrPos, i, controller));
			}

			else
			{
				m_movingObjects[i]->setSpeed(escapeFromPacman(m_pacmanCurrPos, i, controller));
			}
			m_movingObjects[i]->makeMove();
		}
		//handles all the collisions
		handleCollisions(*m_movingObjects[i]);

		//handles the objects on the map
		handleBoard(controller, *m_movingObjects[i]);
	}
}

//checks if the movement of the pacman doesnt go out of the window
bool Board::checkBoardBounds(float x, float y, sf::RectangleShape board)
{
	return (x > board.getGlobalBounds().left &&
		    x < board.getGlobalBounds().width + board.getGlobalBounds().left - PACMAN_SIZE&&
		    y > board.getGlobalBounds().top &&
		    y < board.getGlobalBounds().height + board.getGlobalBounds().top - PACMAN_SIZE);
}

//algorithm for the smart ghosts to follow the pacman
//it checks which directions are legal then checks which one is the best(shortest distance) to the pacman
sf::Vector2f Board::followPacman(sf::Vector2f pacmanPos,int i, Controller controller)
{
	auto ghostPosition = m_movingObjects[i].get()->getPosition();
	//to check if the ghosts is surrounded by walls
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	float shortestPath = 1000;
	sf::Vector2f shortestPathVector;
	for (size_t index = 0; index < m_fixedObjects.size(); index++)
	{
		if (m_fixedObjects[index].get()->getObject() == wallObject)
		{
			//left 
			if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x - SPEED, ghostPosition.y)))
				left = true;
			//right
			else if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x + SPEED, ghostPosition.y)))
				right = true;
			//up
			else if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x, ghostPosition.y - SPEED)))
				up = true;
			//down
			else if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x, ghostPosition.y + SPEED)))
				down = true;
		}
	}

	if (!left)
	{
		shortestPath = sqrt((pow((double)(ghostPosition.x- SPEED) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y) - pacmanPos.y, 2));
		shortestPathVector = sf::Vector2f(-SPEED, 0);
	}
	if (!up)
	{
		if (sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y - SPEED) - pacmanPos.y, 2)) < shortestPath)
		{
			shortestPath = sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y - SPEED) - pacmanPos.y, 2));
			shortestPathVector = sf::Vector2f(0, -SPEED);
		}
	}
	if (!right)
	{
		if (sqrt((pow((double)(ghostPosition.x) - pacmanPos.x + SPEED, 2)) + pow((double)(ghostPosition.y) - pacmanPos.y, 2)) < shortestPath)
		{
			shortestPath = sqrt((pow((double)(ghostPosition.x + SPEED) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y) - pacmanPos.y, 2)) ;
			shortestPathVector = sf::Vector2f(SPEED,0 );
		}
	}
	if (!down)
	{
		if (sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y + SPEED) - pacmanPos.y, 2)) < shortestPath)
		{
			shortestPath = sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y + SPEED) - pacmanPos.y, 2)) ;
			shortestPathVector = sf::Vector2f(0, SPEED);
		}
	}
	return shortestPathVector;
}

//when the pacman is super, the smart ghosts try to run away from the pacman
sf::Vector2f Board::escapeFromPacman(sf::Vector2f pacmanPos, int i, Controller controller)
{
	auto ghostPosition = m_movingObjects[i].get()->getPosition();
	//to check if the ghosts is surrounded by walls
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	float longestPath = 0;
	sf::Vector2f longestPathVector;

	for (size_t index = 0; index < m_fixedObjects.size(); index++)
	{
		if (m_fixedObjects[index].get()->getObject() == wallObject)
		{
			//left 
			if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x - SPEED, ghostPosition.y)))
				left = true;
			//right
			else if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x + SPEED, ghostPosition.y)))
				right = true;
			//up
			else if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x, ghostPosition.y - SPEED)))
				up = true;
			else if (m_fixedObjects[index].get()->getSprite().getGlobalBounds().contains(sf::Vector2f(ghostPosition.x, ghostPosition.y + SPEED)))
				down = true;
		}
	}

	if (!left)
	{
		longestPath = sqrt((pow((double)(ghostPosition.x - SPEED) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y) - pacmanPos.y, 2));
		longestPathVector = sf::Vector2f(-SPEED, 0);
	}
	if (!up)
	{
		if (sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y - SPEED) - pacmanPos.y, 2)) > longestPath)
		{
			longestPath = sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y - SPEED) - pacmanPos.y, 2)) ;
			longestPathVector = sf::Vector2f(0, -SPEED);
		}
	}
	if (!right)
	{
		if (sqrt((pow((double)(ghostPosition.x) - pacmanPos.x + SPEED, 2)) + pow((double)(ghostPosition.y) - pacmanPos.y, 2)) > longestPath)
		{
			longestPath = sqrt((pow((double)(ghostPosition.x + SPEED) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y) - pacmanPos.y, 2)) ;
			longestPathVector = sf::Vector2f(SPEED, 0);
		}
	}
	if (!down)
	{
		if (sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y + SPEED) - pacmanPos.y, 2)) > longestPath)
		{
			longestPath = sqrt((pow((double)(ghostPosition.x) - pacmanPos.x, 2)) + pow((double)(ghostPosition.y + SPEED) - pacmanPos.y, 2)) ;
			longestPathVector = sf::Vector2f(0, SPEED);
		}
	}
	return longestPathVector;
}

//the number of the eaten cakes
int Board::getEatenCake() const
{
	return m_eatenCakeCounter;
}

//return the number of the ghosts in the game
int Board::getGhostsNum() const
{
	return m_movingObjects.size() - 1;
}

//to make animation for the pacman
void Board::pacmanSprite(int &frame, int row)
{
	for (size_t i = 0; i < m_movingObjects.size(); i++)
	{
		if (m_movingObjects[i].get()->getObject() == pacmanObject)
		{
			auto& textures = Resources::instance().getTextures();
			sf::Sprite sprite;
			sprite.setPosition(m_movingObjects[i].get()->getPosition());
			sprite.setTexture(textures[pacman]);
			sprite.setColor(m_movingObjects[i].get()->getSprite().getColor());

			sprite.setTextureRect(sf::IntRect(PACMAN_SIZE * frame, PACMAN_SIZE * row, PACMAN_SIZE, PACMAN_SIZE));
			m_movingObjects[i].get()->setSprite(sprite);
			break;
		}
	}
	
}

//draw the info bar above the level, it contains
//level, time, points, super(yes or no), health
void Board::drawInfo(Controller& controller)
{
	auto window = controller.getWindow();
	auto font = Resources::instance().getFont();
	auto level = controller.getLevel() + 1;
	auto time = controller.getTime();
	auto health = controller.getHealth();
	auto points = controller.getPoints();
	auto super = controller.getSuper();

	std::string levelString = std::to_string(level);
	std::string timeString = std::to_string((int)time.asSeconds());
	std::string healthString = std::to_string(health);
	std::string pointString = std::to_string(points);
	std::string superString = std::to_string(super);

	sf::RectangleShape bar;
	bar.setSize(sf::Vector2f(800.f, 50.f));
	bar.setFillColor(sf::Color(255, 255, 255, 170));
	bar.setOutlineColor(sf::Color::Black);
	bar.setOutlineThickness(2);
	bar.setOrigin(bar.getGlobalBounds().width / 2.0f, bar.getGlobalBounds().height / 2.0f);
	bar.setPosition(window->getSize().x / 2.0f, m_board.getGlobalBounds().top - 25);

	sf::Text levelText;
	levelText.setFont(font);
	levelText.setStyle(sf::Text::Bold);
	levelText.setString("LEVEL:" + levelString);
	levelText.setCharacterSize(30);
	levelText.setFillColor(sf::Color(100, 60, 100, 255));
	levelText.setOutlineColor(sf::Color::Black);
	levelText.setOutlineThickness(1);
	levelText.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width - levelText.getGlobalBounds().width - 10, bar.getGlobalBounds().top + 5);

	sf::Text timeText;
	timeText.setFont(font);
	timeText.setStyle(sf::Text::Bold);
	timeText.setString("TIME:" + timeString);
	timeText.setCharacterSize(30);
	timeText.setFillColor(sf::Color(100, 60, 100, 255));
	timeText.setOutlineColor(sf::Color::Black);
	timeText.setOutlineThickness(1);
	timeText.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width - 285,
		                 bar.getGlobalBounds().top + 5);

	sf::Text pointText;
	pointText.setFont(font);
	pointText.setStyle(sf::Text::Bold);
	pointText.setString("POINTS:" + pointString);
	pointText.setCharacterSize(30);
	pointText.setFillColor(sf::Color(100,60,100,255));
	pointText.setOutlineColor(sf::Color::Black);
	pointText.setOutlineThickness(1);
	pointText.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width - 465, bar.getGlobalBounds().top + 5);

	sf::Text superText;

	if (super)
	{
		//if super then the color will be full
		superText.setFillColor(sf::Color(100, 60, 100, 255));
		superText.setOutlineColor(sf::Color(0, 0, 0, 255));
	}
	else
	{
		//if not super then the color will be half transparent
		superText.setFillColor(sf::Color(100, 60, 100, 100));
		superText.setOutlineColor(sf::Color(0, 0, 0, 0));
	}

	superText.setString("SUPER");
	superText.setFont(font);
	superText.setStyle(sf::Text::Bold);
	superText.setCharacterSize(30);


	superText.setOutlineThickness(1);
	superText.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width - 600, bar.getGlobalBounds().top + 5);

	for (size_t i = 0; i < m_livesObjects.size(); i++)
	{
		m_livesObjects[i].setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width - 790 + (m_livesObjects[i].getGlobalBounds().width - 10)*i, bar.getGlobalBounds().top );
	}

	window->draw(bar);
	window->draw(timeText);
	for (size_t i = 0; i < m_livesObjects.size(); i++)
	{
		window->draw(m_livesObjects[i]);
	}
	window->draw(superText);
	window->draw(levelText);
	window->draw(pointText);
}

//creates the moving objects
static std::unique_ptr<MovingObject> createMovingObject(ObjectTypes type,
	const sf::Vector2f& position,
	const std::vector<sf::Texture>& textures)
{
	auto sprite = sf::Sprite();
	sprite.setPosition(position);
	switch (type)
	{
	case ObjectTypes::pacmanObject:
		sprite.setTextureRect(sf::IntRect(PACMAN_SIZE * 0, PACMAN_SIZE * 0, PACMAN_SIZE, PACMAN_SIZE));
		sprite.setTexture(textures[pacman]);
		return std::make_unique<Pacman>(sprite);
	case ObjectTypes::smartGhostObject:
		sprite.setTexture(textures[smartGhost]);
		return std::make_unique<SmartGhost>(sprite);
	case ObjectTypes::regularGhostObject:
		sprite.setTexture(textures[regularGhost]);
		return std::make_unique<RegularGhost>(sprite);
	}
	return nullptr;
}

//creats the fixed objects
static std::unique_ptr<FixedObject> createFixedObject(ObjectTypes type,
	const sf::Vector2f& position,
	const std::vector<sf::Texture>& textures)
{
	auto sprite = sf::Sprite();
	sprite.setPosition(position);

	switch (type)
	{
	case ObjectTypes::wallObject:
		sprite.setTexture(textures[wall]);
		return std::make_unique<Wall>(sprite);
	case ObjectTypes::doorObject:
		sprite.setTexture(textures[door]);
		return std::make_unique<Door>(sprite);
	case ObjectTypes::cakeObject:
		sprite.setTexture(textures[cake]);
		return std::make_unique<Cake>(sprite);
	case ObjectTypes::giftObject:
		sprite.setTexture(textures[gift]);
		return std::make_unique<Gift>(sprite,0);
	case ObjectTypes::timerObject:
		sprite.setTexture(textures[timer]);
		return std::make_unique<Timer>(sprite,0);
	case ObjectTypes::keyObject:
		sprite.setTexture(textures[key]);
		return std::make_unique<Key>(sprite);
	case ObjectTypes::healthObject:
		sprite.setTexture(textures[health]);
		return std::make_unique<Health>(sprite,0);
	case ObjectTypes::iceBallObject:
		sprite.setTexture(textures[iceBall]);
		return std::make_unique<IceBall>(sprite);
	case ObjectTypes::bombObject:
		sprite.setTexture(textures[bombTexture]);
		return std::make_unique<Bomb>(sprite);
	}
	return nullptr;
}

//the function receives the object and inserts to the correct vector
void Board::makeObject(const ObjectTypes& type, const sf::Vector2f& position)
{
	auto& textures = Resources::instance().getTextures();

	std::unique_ptr<FixedObject> fixedObjects = createFixedObject(type, position, textures);
	if (fixedObjects)
	{
		m_fixedObjects.push_back(std::move(fixedObjects));

		return;
	}

	std::unique_ptr<MovingObject> movingObjects = createMovingObject(type, position, textures);
	if (movingObjects)
	{
		m_movingObjects.push_back(std::move(movingObjects));
		m_movingInitPos.push_back(position);
		return;
	}
	// to initialize the current position of the pacman
	for (size_t i = 0; i < m_movingObjects.size(); i++)
	{
		if (m_movingObjects[i].get()->getObject() == pacmanObject)
			m_pacmanCurrPos = m_movingObjects[i].get()->getPosition();
	}
}

//according to the board
float Board::Xposition(const int number) const
{
	return float((m_levelPosition.x - m_levelSize.x/2) + number * TILE_SIZE);
}

//according to the board
float Board::Yposition(const int number) const
{
	return float((m_levelPosition.y - m_levelSize.y / 2) + number * TILE_SIZE);
}

//checkig the collisions
void Board::handleCollisions(GameObject& gameObject)
{
	for (auto& fixed : m_fixedObjects)
	{
		if (gameObject.checkCollision(*fixed))
		{
			gameObject.handleCollision(*fixed);
		}
	}
	for (auto& moving : m_movingObjects)
	{
		if (gameObject.checkCollision(*moving))
		{
			gameObject.handleCollision(*moving);
		}
	}
}

//changes the objects on the boards
void Board::handleBoard(Controller& controller, MovingObject& movingObjects)
{
	for (int i = 0; i < m_fixedObjects.size(); i++)
	{
		FixedObject* object = m_fixedObjects[i].get();
		AllGifts* gift = dynamic_cast<AllGifts*>(object);

		//checks if activated a gift
		if (gift)
		{
			if (gift->getActivated())
			{
				gift->doGiftTask(controller, m_movingObjects);
				
				//check if the pacman touched a heart(health object)
				if (gift->getObject() == healthObject)
				{
					//add a heart if the hearts are not full
					for (int j = m_livesObjects.size() - 1; j > -1; j--)
					{
						if (m_livesObjects[j].getColor() == sf::Color(100, 100, 100, 100))
						{
							m_livesObjects[j].setColor(sf::Color(255, 255, 255, 255));
							break;
						}

					}
				}

				else if (gift->getObject() == iceBallObject)
				{
					//start the freezing clock to freeze the ghosts for 5 seconds
					 m_freezingClock.restart();
					controller.setFreezeGhosts(true);
				}
				else if (gift->getObject() == giftObject)
				{
					//start the super clock to make the pacman super for 5 seconds
					m_superClock.restart();
					controller.setSuper(true);
				}
				else if (gift->getObject() == bombObject)
				{
					//if the pacman touched the bomb it will lose 2 lives
					controller.setHealth(-2);

					//to make 2 hearts half transparent
					int x = 0;
					while (x < 2)
					{
						for (size_t j = 0; j < m_livesObjects.size(); j++)
						{
							if (m_livesObjects[j].getColor() != sf::Color(100, 100, 100, 100))
							{
								m_livesObjects[j].setColor(sf::Color(100, 100, 100, 100));
								x++;
								break;
							}
						}
					}
				}
			}
		}
	}

	//when freezing time is 5 seconds
	if (m_freezingClock.getElapsedTime().asSeconds() >= 5)
	{
		//restart the freezing time
		m_freezingClock.restart();
		controller.setFreezeGhosts(false);//stop the freezing(make it false)
	}
	//when the pacman is super for 5 seconds
	if (m_superClock.getElapsedTime().asSeconds() >= 5 && controller.getSuper())
	{
		//restart the super time
		m_superClock.restart();
		controller.setSuper(false);//stop the super(make it false)
	}

	//handle the collision with the cake
	for (int i = 0; i < m_fixedObjects.size(); i++)
	{
		FixedObject* object = m_fixedObjects[i].get();
		Cake* cake = dynamic_cast<Cake*>(object);

		//checks if took a cake
		if (cake)
		{
			if (cake->getDelete())
			{
				m_eatenCakeCounter++;//increase the number of the eaten cakes
				controller.setPoints(CAKE_POINTS);//add the cake points

				//check if all the cakes are eaten
				if (m_eatenCakeCounter == m_cakeCounter)
				{
					//if the player wins a level, add 2 points for every ghost in the level
					controller.setPoints(GHOSTS_POINTS * getGhostsNum());
				}
			}
		}
	}

	//check the key and door
	for (int i = 0; i < m_fixedObjects.size(); i++)
	{
		FixedObject* object = m_fixedObjects[i].get();
		Key* key = dynamic_cast<Key*>(object);

		//checks if took a key
		if (key)
		{
			if (key->getDelete())
			{
				controller.setPoints(KEY_POINTS);//add the points of the key
				for (int i = 0; i < m_fixedObjects.size(); i++)
				{
					FixedObject* object = m_fixedObjects[i].get();
					Door* door = dynamic_cast<Door*>(object);

					//deletes a door
					if (door)
					{
						door->deleteObject();
						break;
					}
				}
			}
		}
	}

	//to handle the moving objects
	for (size_t i = 0; i < m_movingObjects.size(); i++)
	{
		if (m_movingObjects[i].get()->getObject() == pacmanObject)
		{
			MovingObject* object = m_movingObjects[i].get();
			Pacman* pacmanObject = dynamic_cast<Pacman*>(object);

			//if the pacman is super, make it red
			if (controller.getSuper())
				pacmanObject->changeColor(sf::Color::Red);
			else
			{
				pacmanObject->setDestroyTheDoor(false);//now the pacman cant destroy the doors
				pacmanObject->changeColor(sf::Color(255, 255, 255, 255)); //color it to the original color
				pacmanObject->setSpeed(sf::Vector2f(SPEED, SPEED)); //set the speed to the regular speed
			}
		}

		//if not pacman(if ghost)
		else 
		{
			//if the ghosts are not freezed, color them to their original color
			if (!controller.getFreezeGhosts())
				m_movingObjects[i]->changeColor(sf::Color(255, 255, 255, 255));
		}
	}

	//handle the collision with the ghosts
	for (int i = 0; i < m_movingObjects.size(); i++)
	{
		MovingObject* object = m_movingObjects[i].get();
		Ghost* ghost = dynamic_cast<Ghost*>(object);

		//checks if the pacman touched a ghost
		if (ghost)
		{
			//if the regular pacman touched a ghost
			if (ghost->getTouchedPacman() && !controller.getSuper())
			{
				Resources::instance().playSound(touchGhost);
				controller.setHealth(-1); //decrease the health by 1

				for (size_t j = 0; j < m_movingObjects.size(); j++)
				{
					//return all the moving objects to their initial position
					m_movingObjects[j].get()->setPosition(m_movingInitPos[j]);
					ghost->setTouchPacman(false);
				}

				//make one heart half transparent
				for (size_t j = 0; j < m_livesObjects.size(); j++)
				{
					if (m_livesObjects[j].getColor() != sf::Color(100, 100, 100, 100))
					{
						m_livesObjects[j].setColor(sf::Color(100, 100, 100, 100));
						break;
					}
				}
			}
			else
				ghost->setTouchPacman(false);
		}
		MovingObject* p = m_movingObjects[i].get();

		Pacman* pacman = dynamic_cast<Pacman*>(p);
		if (pacman)
		{
			if (m_cakeCounter == m_eatenCakeCounter)
			{
				controller.levelWon();
				controller.setHealth(3);
				Resources::instance().playSound(gameWon);
			}
		}
	}

	//erasing the objects that deleted
	std::erase_if(m_fixedObjects, [](const auto& object){return object->getDelete();});
}

