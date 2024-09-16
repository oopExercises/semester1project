#pragma once

const float SPEED = 2.0;
const int TILE_SIZE = 48;
const int PACMAN_SIZE = 36;
const int EXTRA_TIME = 5;
const int HEALTHTOP = 3;
//points
const int CAKE_POINTS = 2;
const int KEY_POINTS = 7;
const int GIFT_POINTS = 5;
const int WON_LEVEL_POINTS = 50;
const int GHOSTS_POINTS = 2;

enum ObjectTypes
{
	pacmanObject = 'a',
	smartGhostObject = '&',
	regularGhostObject = 'G',
	wallObject = '#',
	doorObject = 'D',
	cakeObject = '*',
	giftObject = '$',
	keyObject = '%',
	iceBallObject = 's', //snow
	healthObject = 'h', //heart
	timerObject = 't', //time
	bombObject = 'B' //bomb
};

enum SoundTypes
{
	eatCake,
	superGift,
	background,
	gameWon,
	pacmanDeath,
	freeze,
	timerUp,
	healing,
	touchGhost,
	menuClick,
	tookTheKey,
	bomb
};

enum TextureTypes
{
	pacman,
	smartGhost,
	regularGhost,
	door,
	wall,
	cake,
	gift,
	key,
	iceBall,
	timer,
	health,
	pacmanBackground,
	backButton,
	bombTexture
};