#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "io.h"
#include <SFML/Audio.hpp>

//-------------------------------------------------------------
// Resources class :
//-------------------------------------------------------------

class Resources//singleton
{
private:

	std::vector<std::string> m_levels;		//vector of all the levels
	std::vector<sf::Texture> m_textures;	//vector of textures
	std::vector<sf::Sound> m_sounds;
	std::vector<sf::SoundBuffer> m_buffers;
	sf::Font m_font;		


	std::vector<std::string> m_menuOptions;

	float m_levelTime;

	Resources();
	Resources(const Resources&) = delete;
	Resources& operator=(const Resources&) = delete;

	std::vector<std::string> readNamesVector(const char[]) const; //to read strings(names) from a file
	std::vector<sf::Texture> getTexturesVector(const std::vector<std::string>&) const; //to create the vector that contains the textures
	std::vector<sf::Sound> fillSoundsVector(const std::vector<sf::SoundBuffer>& buffer) const; //to create the vector that contains the buffers
	std::vector<sf::SoundBuffer> fillBufferVector(const std::vector<std::string>&) const;//to create the vector that contains the sounds
public:
	void playMusic(); //play the background music all time
	void playSound(const int index);//play a specific sound when needed
	static Resources& instance();
	~Resources() = default;

	const std::vector<sf::Texture>& getTextures() const;
	const sf::Texture& getTexture(const int) const;
	const sf::Font& getFont() const;
	const std::string& getLevel(const int) const;
	const std::vector<std::string>& getMenuOptions() const;

	void saveCurentLevel(const int);

	int getCurrentLevel() const;
	int getNumberOfLevels() const;
	float getlevelDuration() const;

	

	std::vector<std::string> buildMap(const int);
	std::vector<std::string> newMenu() const;
	std::vector<std::string> continueMenu() const;
};