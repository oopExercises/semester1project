#include "Resources.h"

Resources::Resources()
{
    m_levels = readNamesVector("levels.txt"); //read the levels names from the levels file
    std::vector<std::string> texturesNames = readNamesVector("allTextures.txt"); //read the textures names from allTextures file
    std::vector<std::string> soundNames = readNamesVector("allSounds.txt"); //read the sounds names from sounds file
    m_textures = getTexturesVector(texturesNames); //get the images textures from the images file
    m_buffers = fillBufferVector(soundNames);
    m_sounds = fillSoundsVector(m_buffers);
    m_font.loadFromFile("arial.ttf"); 
}

//the constructor that we use to call any function in this class
Resources& Resources::instance()
{
    static Resources instance;
    return instance;
}

//return vector of textures
const std::vector<sf::Texture>& Resources::getTextures() const
{
    return m_textures;
}

//return a specific texture
const sf::Texture& Resources::getTexture(const int index) const
{
    return m_textures[index];
}

const sf::Font& Resources::getFont() const
{
    return m_font;
}

//return the name of the level
const std::string& Resources::getLevel(const int index) const
{
    return m_levels[index];
}

//return a vector of strings that must appear in the menu (start, about, exit)
const std::vector<std::string>& Resources::getMenuOptions() const
{
    return m_menuOptions;
}

int Resources::getNumberOfLevels() const
{
    return (int)m_levels.size();
}

//if the game time wasn't >0 ,then return 60(seconds)
float Resources::getlevelDuration() const
{
    return m_levelTime > 0 ? m_levelTime : 60;
}

//return the current level
int Resources::getCurrentLevel() const
{
    std::ifstream file;
    char level;

    //opens the file
    file.open("currentLevel.txt");
    if (!file.is_open()) {
        std::cerr << "input file not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    file.get(level);
    file.close();

    return level - '0'; //to return the number of the level , for example if the level is 1 (char) then we 
                        // decrease the char 0 and we get 1(int)
}

//to print the current level number on the currentLevel file
void Resources::saveCurentLevel(const int num)
{
    std::ofstream file;

    //opens the file
    file.open("..\\..\\..\\resources\\textFiles\\currentLevel.txt");
    if (!file.is_open()) {
        std::cerr << "input file not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    file << num;
    file.close();
}

std::vector<std::string> Resources::newMenu() const
{
    return { "START", "ABOUT", "EXIT" };
}

std::vector<std::string> Resources::continueMenu() const
{
    return { "CONTINUE", "NEW GAME", "ABOUT", "EXIT" };
}

//return a vector of strings that read from text
std::vector<std::string> Resources::readNamesVector(const char file_name[]) const
{
    std::ifstream file;
    std::vector<std::string> names;

    //opens the file
    file.open(file_name);
    if (!file.is_open()) {
        std::cerr << "input file not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    //reads the image's name
    for (auto line = std::string(); std::getline(file, line);)
    {
        names.push_back(line);
    }

    file.close();

    return names;
}

//retur a vector of textures 
std::vector<sf::Texture> Resources::getTexturesVector(const std::vector<std::string>& names) const
{
    std::vector<sf::Texture> textures(names.size());

    for (int i = 0; i < names.size(); i++)
        textures[i].loadFromFile(names[i]);

    return textures;
}

//return a vector of sounds
std::vector<sf::Sound> Resources::fillSoundsVector(const std::vector<sf::SoundBuffer>& buffer) const
{
    std::vector<sf::Sound> sounds(buffer.size());

    for (int i = 0; i < buffer.size(); i++)
        sounds[i].setBuffer(buffer[i]);

    return sounds;
}

//return a vector of buffers
std::vector<sf::SoundBuffer> Resources::fillBufferVector(const std::vector<std::string>& names) const
{
    std::vector<sf::SoundBuffer> buffers(names.size());
    sf::SoundBuffer buffer;
    
    for (int i = 0; i < names.size(); i++)
    {
        buffers[i].loadFromFile(names[i]);
    }

    return buffers;
}


//to build the map from a text file
std::vector<std::string> Resources::buildMap(const int level)
{
    std::ifstream file;
    std::vector<std::string> map;
    auto time = std::string();

    //opens the file
    file.open(getLevel(level));
    if (!file.is_open()) {
        std::cerr << level << " file not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::getline(file, time);
    m_levelTime = (float)std::stoi(time); //stoi : to convert a string to integer

    //reads the map
    for (auto line = std::string(); std::getline(file, line);)
    {
        map.push_back(line);
    }

    file.close();

    return map;
}

//the music of the background
void Resources::playMusic()
{
    m_sounds[background].setLoop(true);// play all the time
    m_sounds[background].setVolume(8); 
    m_sounds[background].play();
}

//play a specific sound
void Resources::playSound(const int index)
{
    m_sounds[index].setVolume(15);
    m_sounds[index].play();
}