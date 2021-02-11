/*
---------------------------------------------------------------------
gooseEscapeGamePlay.cpp Source File
---------------------------------------------------------------------
*/

#include <iostream>
#include <cmath>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//Included libraries for use of the rand() and srand() functions
//Cited from //www.cplusplus.com/reference/cstdlib/rand/
#include <cstdlib> 
#include <cstdio>
#include <ctime>

extern Console out;
/*
With graphics, screens are given an x,y coordinate system with the origin
in the upper left corner.  So it means the coordinate axes are:
---------------->   x direction
|
|
|
|
|
V

y direction 
*/
	
/*
    Print the game world
    
    The functions should draw characters to present features of the game
    board, e.g. win location, obstacles, power ups
*/	
	
// print the game board function
void gameBoard(int gameMap[MAX_BOARD_Y][MAX_BOARD_X], Actor const & player, 
			   Actor const & monster, Actor const & teleport_pad1, 
			   Actor const & teleport_pad2)
{
	for (int row = 0; row < MAX_BOARD_Y; row++)
	{
		for (int col = 0; col < MAX_BOARD_X; col++)
		{
			char character = ' ';
			
			if (gameMap[row][col] == SHALL_NOT_PASS)
				character = WALL_CHAR;
			else if (gameMap[row][col] == WINNER)
				character = WIN_CHAR;
			else if (col == player.get_x() && row == player.get_y())
				character = PLAYER_CHAR;
			else if (col == monster.get_x() && row == monster.get_y())
				character = MONSTER_CHAR;
			else if (col == teleport_pad1.get_x() && row 
					 == teleport_pad1.get_y() || col == teleport_pad2.get_x() 
					 && row == teleport_pad2.get_y())
				character = TELEPORT_CHAR;
			
			terminal_put(col, row, character);
		}
	}
}

/*
	Captured function checks for if the goose has captured the player.
*/
bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
}


//Move the player to a new location based on the user input. 
void movePlayer(int key, Actor & player, int gameMap[MAX_BOARD_Y][MAX_BOARD_X])
{
    int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
        
    if (player.can_move(xMove, yMove) 
        && gameMap[player.get_y() + yMove][player.get_x() + xMove] 
	    != SHALL_NOT_PASS)
	    {
	    	player.update_location(xMove, yMove);
		}
}

/*
    Other functions required to make the game work
*/

//Function checks for overlap of safe zone and player
bool levelComplete(Actor const & player, int gameMap[MAX_BOARD_Y][MAX_BOARD_X])
{
	bool safeZone = false;
	
	if (gameMap[player.get_y()][player.get_x()] == WINNER)
		safeZone = true;
	
    return safeZone;
}

//Function for finding distance between player and goose
void calcDistanceGoose(Actor & player, Actor & monster, 
					   int gameMap[MAX_BOARD_Y][MAX_BOARD_X])
{
	int distanceX = 0, distanceY = 0;
	
	distanceX = monster.get_x() - player.get_x();
	distanceY = monster.get_y() - player.get_y();
	
	moveGoose(player, monster, gameMap, distanceX, distanceY);
}

//Function for goose chasing player
void moveGoose(Actor & player, Actor & monster, 
			   int gameMap[MAX_BOARD_Y][MAX_BOARD_X], int distanceX, 
			   int distanceY)
{
	int changeX = 1, changeY = 1;
	int tempCharacter = 0;
	
	//Check for greater distance in X or Y
	if (abs(distanceX) >= abs(distanceY))
	{
		if (distanceX > 0)
			changeX *= -1;
		
		changeY -= 1;
	}
	else 
	{
		if (distanceY > 0)
			changeY *= -1;
		
		changeX -= 1;
	}
	
	//Update goose location when the goose has not captured the player
	if (monster.get_x() != player.get_x() || monster.get_y() != player.get_y())
		monster.update_location(changeX, changeY);
}

//Function to randomize values
int randomize(int min, int max)
{
	//Set variables to random numbers between given range
	//Random cited from https://www.cplusplus.com/reference/cstdlib/rand/
	return (rand() % max) + min;
}

//Function to set walls for game map
void setWall(bool isRow, int index, const int MAX, const int MIN_SIZE, 
			 const int MAX_SIZE, const int MIN_START, const int MAX_START, 
			 int gameMap[MAX_BOARD_Y][MAX_BOARD_X])
{
	int wallSize = randomize(MIN_SIZE, MAX_SIZE);
	int wallStart = randomize(MIN_START, MAX_START);
	
	for (int count = 0; count < MAX; count++)
	{
		if (count >= wallStart && count <= wallStart + wallSize)
		{
			if (isRow)
				gameMap[index][count] = SHALL_NOT_PASS;		
			else
				gameMap[count][index] = SHALL_NOT_PASS;
		}
	}
}

//Check teleportation pads for player
bool checkTeleport(Actor & player)
{
	bool teleport = true;
	bool pad1 = true;
	
	//Teleport player to the other pad when standing on one of the pads
	if (player.get_x() == pad1_x && player.get_y() == pad1_y)
		player.teleport_pad(pad1);
	else if (player.get_x() == pad2_x && player.get_y() == pad2_y)
		player.teleport_pad(!pad1);
	else 
		teleport = false;
	
	return teleport;
}

//Check for goose hitting a wall
bool checkGooseWall(Actor & monster, int gameMap[MAX_BOARD_Y][MAX_BOARD_X])
{
	return gameMap[monster.get_y()][monster.get_x()] == SHALL_NOT_PASS;
}

//Reset safe zone after each level is completed
void resetSafeZone(Actor & player, int gameMap[MAX_BOARD_Y][MAX_BOARD_X], 
				   int safeX, int safeY)
{
	//Set previous safe zone to wall
    gameMap[safeY][safeX] = SHALL_NOT_PASS;
        	
	//Create new safe zone random position
    do
    {
    	safeX = randomize(WALL_ROW_MIN_SIZE, 
					  	  WALL_ROW_MAX_START * WALL_COL_MIN_START);
		safeY = randomize(WALL_ROW_MIN_START, 
					  	  WALL_ROW_MAX_START / WALL_COL_MIN_START);
	}
	while (gameMap[safeY][safeX] == SHALL_NOT_PASS);
	
        	
    //Set new safe zone in game board
	gameMap[safeY][safeX] = WINNER;
	
	//Update safeX and safeY positions for new safe zone
	player.set_safeX(safeX);
 	player.set_safeY(safeY);
}
