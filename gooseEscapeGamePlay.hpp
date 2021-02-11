/*
---------------------------------------------------------------------
gooseEscapeGamePlay.hpp Source File
---------------------------------------------------------------------
*/

#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"

/*
	This file is all about the game world.  You will modify this to add
    constants and function prototypes.  Modify gooseGamePlay.cpp to
	actually add functionality.
*/

/*
    Declare constants to indicate various game world features in the board
    array.  Modify them to fit what you would like to do in the game.  You can
    change the type if you choose to store your game board as something other
    than integers.
*/	
// Going further:  Learn how to use an enum for these values
const int EMPTY = 0;
const int SHALL_NOT_PASS = 1;
const int WINNER = 2;

//Constants for creating random wall positions and sizes
const int WALL_ROW_MAX_SIZE = 20, WALL_ROW_MIN_SIZE = 10;
const int WALL_ROW_MAX_START = 30, WALL_ROW_MIN_START = 5;
const int WALL_COL_MAX_SIZE = 10, WALL_COL_MIN_SIZE = 5;
const int WALL_COL_MAX_START = 5, WALL_COL_MIN_START = 2;
const int FIVE = 5, THREE = 3;

/*
    A few examples of characters both for actors and for the game board
    itself are shown.
*/	
//display characters
const int PLAYER_CHAR = int('@');
const int MONSTER_CHAR = int('G');
const int WALL_CHAR = int('o');
const int WIN_CHAR = int('%'); 
const int TELEPORT_CHAR = int('T');
const int HEART_CHAR = int('H');

/*
    Game play function prototypes are give below.
*/

// print the game board function protype
void gameBoard(int gameMap[MAX_BOARD_Y][MAX_BOARD_X], Actor const & player, 
			   Actor const & monster, Actor const & teleport_pad1, 
			   Actor const & teleport_pad2);

bool captured(Actor const & player, Actor const & monster);

/*
    Move the player to a new location based on the user input.  
*/

void movePlayer(int key, Actor & player, int gameMap[MAX_BOARD_Y][MAX_BOARD_X]);

/*
    What other functions do you need to make the game work?  What can you
    add to the basic functionality to make it more fun to play?
*/

bool levelComplete(Actor const & player, int gameMap[MAX_BOARD_Y][MAX_BOARD_X]);

void calcDistanceGoose(Actor & player, Actor & monster, 
					   int gameMap[MAX_BOARD_Y][MAX_BOARD_X]);
					   
void moveGoose(Actor & player, Actor & monster, 
			   int gameMap[MAX_BOARD_Y][MAX_BOARD_X], int distanceX, 
			   int distanceY);
				
int randomize(int min, int max);

void setWall(bool isRow, int index, const int MAX, const int MIN_SIZE, 
			 const int MAX_SIZE, const int MIN_START, const int MAX_START, 
			 int gameMap[MAX_BOARD_Y][MAX_BOARD_X]);

bool checkTeleport(Actor & player);

bool checkGooseWall(Actor & monster, int gameMap[MAX_BOARD_Y][MAX_BOARD_X]);

void resetSafeZone(Actor & player, int gameMap[MAX_BOARD_Y][MAX_BOARD_X], 
				   int safeX, int safeY);

#endif
