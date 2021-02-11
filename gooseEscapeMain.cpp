/*
By: Sean Bakker & Aaditya Rastogi
*/

/*
---------------------------------------------------------------------
gooseEscapeMain.cpp Source File
---------------------------------------------------------------------
*/

#include <BearLibTerminal.h>
#include <cmath>
#include <iostream>
using namespace std;
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//Included libraries for use of the rand() and srand() functions
//Cited from //www.cplusplus.com/reference/cstdlib/rand/
#include <cstdlib> 
#include <cstdio>
#include <ctime>

//set up the console.
Console out;

int main()
{
	//Set up the window.  Don't edit these two lines
    terminal_open();
  	terminal_set(SETUP_MESSAGE);
	
	/*
	Use Random function to randomize the starting player position, wall 
	positions, safe zone positions and the goose position.
	*/
	
	//srand() function cited from:
	//https://www.cplusplus.com/reference/cstdlib/rand/
	srand(time(NULL));
	
	//Variables
	int playerX = 0, playerY = 0, safeX = 0, safeY = 0, gooseX = 0, gooseY = 0;
	int count = 1, freezeGoose = 0, hearts = WINNER, level = THREE;
    bool teleporting = false, gooseOnWall = false;
    
	// Declare the array that will hold the game board "map"
  	int gameMap[MAX_BOARD_Y][MAX_BOARD_X] = {0};
	
	/*
    Initiallize locations in the game board to have game features. 
	*/
	
	//Initialize the game map to empty characters, and create walls
	int wallCol1 = 0, wallCol2 = 0;
	int wallRow1 = 0, wallRow2 = 0, wallRow3 = 0;
	bool isRow = true;
	
	//Randomize the wall row positions
	do 
	{
		wallRow1 = randomize(WALL_ROW_MIN_START, WALL_ROW_MAX_SIZE);
		wallRow2 = randomize(WALL_ROW_MIN_START, WALL_ROW_MAX_SIZE);
		wallRow3 = randomize(WALL_ROW_MIN_START, WALL_ROW_MAX_SIZE);
	}
	while (wallRow1 == wallRow2 || wallRow1 == wallRow3 || 
		   wallRow2 == wallRow3);
		   
	//Set the wall row positions 
	//WALL_ROW_MAX_SIZE ensures a wall size of at least 10 tiles long
	setWall(isRow, wallRow1, MAX_BOARD_X, WALL_ROW_MIN_SIZE, WALL_ROW_MAX_SIZE, 
			WALL_ROW_MIN_START, WALL_ROW_MAX_START, gameMap);
	setWall(isRow, wallRow2, MAX_BOARD_X, WALL_ROW_MIN_SIZE, WALL_ROW_MAX_SIZE, 
			WALL_ROW_MIN_START, WALL_ROW_MAX_START, gameMap);
	setWall(isRow, wallRow3, MAX_BOARD_X, WALL_ROW_MIN_SIZE, WALL_ROW_MAX_SIZE, 
			WALL_ROW_MIN_START, WALL_ROW_MAX_START, gameMap);
	
	//Randomize the wall column positions
	do 
	{
		wallCol1 = randomize(WALL_COL_MIN_START, 
							 WALL_ROW_MAX_START * WALL_COL_MIN_START);
		wallCol2 = randomize(WALL_COL_MIN_START, 
							 WALL_ROW_MAX_START * WALL_COL_MIN_START);
	}
	while (wallCol1 == wallCol2);
	
	//Set the wall column positions
	setWall(!isRow, wallCol1, MAX_BOARD_Y, WALL_COL_MIN_SIZE, WALL_COL_MAX_SIZE, 
			WALL_COL_MIN_START, WALL_COL_MAX_START, gameMap);
	setWall(!isRow, wallCol2, MAX_BOARD_Y, WALL_COL_MIN_SIZE, WALL_COL_MAX_SIZE, 
			WALL_COL_MIN_START, WALL_COL_MAX_START, gameMap);

	//Randomize the player/safezones/goose starting positions
	do 
	{
		playerY = randomize(1, WALL_ROW_MIN_START);
		playerX = randomize(1, WALL_ROW_MAX_SIZE);
	
		safeX = randomize(WALL_ROW_MIN_SIZE, 
						  WALL_ROW_MAX_START * WALL_COL_MIN_START);
		safeY = randomize(WALL_ROW_MIN_START, 
						  WALL_ROW_MAX_START / WALL_COL_MIN_START);
	
		gooseY = randomize(WALL_ROW_MIN_START, 
						   WALL_ROW_MIN_START + WALL_ROW_MIN_SIZE);
		gooseX = randomize(WALL_ROW_MAX_SIZE, 
						   WALL_ROW_MAX_SIZE + WALL_ROW_MAX_START);
	}
	while(gameMap[playerY][playerX] == SHALL_NOT_PASS || 
		  gameMap[safeY][safeX] == SHALL_NOT_PASS || 
		  gameMap[gooseY][gooseX] == SHALL_NOT_PASS);

    //make the player
	Actor player(PLAYER_CHAR, playerX, playerY);
	
	//make the monster
	Actor monster(MONSTER_CHAR, gooseX, gooseY);
	
	//Set safe zone position
 	gameMap[safeY][safeX] = WINNER;
 	
	player.set_safeX(safeX);
 	player.set_safeY(safeY);
 	
 	//Set the teleportation pads
 	Actor teleport_pad1(TELEPORT_CHAR, pad1_x, pad1_y);
 	Actor teleport_pad2(TELEPORT_CHAR, pad2_x, pad2_y);
   	
    // Call the function to print the game board
    gameBoard(gameMap, player, monster, teleport_pad1, teleport_pad2);
  	
	// Printing the instructions
    out.writeLine("Escape the Goose! " + monster.get_location_string());
	out.writeLine("Use the arrow keys to move");
	out.writeLine("If the goose catches you, you lose! Beware, the goose can go" 
				  " through walls!");
	out.writeLine("Stand on 3 safe zones to escape!");

	/*
    This is the main game loop.  It continues to let the player give input
    as long as they do not press escape or close, they are not captured by
    the goose, and they didn't reach the win tile
	*/
	
    int keyEntered = TK_A;  //can be any valid value that is not ESCAPE or CLOSE
    
    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
          && hearts != 0 && level != 0)
	{
	    // get player key press
	    keyEntered = terminal_read();
	    
	    /*
	    Print the hearts of the player at the beginning of the game and after 
		each loss of a life
		*/
	    if (count == 1)
		{
			if (hearts == WINNER)
	    	{
	    		out.writeLine("Hearts: H H");
			}
			else if (hearts == 1)
			{
				out.writeLine("Hearts: H");
			}	
		}
		
        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
        	//Only move when allowed key input for arrows keys are entered
        	if (keyEntered == TK_UP || keyEntered == TK_DOWN 
				|| keyEntered == TK_LEFT || keyEntered == TK_RIGHT)
			{
				// move the player, you can modify this function
    	    	movePlayer(keyEntered, player, gameMap);

            	// call the goose's chase function
            	if (freezeGoose == 0)
            		calcDistanceGoose(player, monster, gameMap);
            	else 
            		freezeGoose--;
        		
            	//Check Teleportation pads for player
            	teleporting = checkTeleport(player);
            	
            	if (teleporting)
            		player.teleport(player.get_teleporting_pad());
            		
            	//Check for goose hitting wall
            	if (freezeGoose == 0)
            	{
            		gooseOnWall = checkGooseWall(monster, gameMap);
            	
            		if (gooseOnWall)
            		{
            			calcDistanceGoose(player, monster, gameMap);
            			freezeGoose = FIVE;
					}
				}
            	
            	//Update game board
				gameBoard(gameMap, player, monster, teleport_pad1, 
						  teleport_pad2);
			}
        }
        
        //Create new safe zone after each level is complete
        if (levelComplete(player, gameMap))
        {
        	resetSafeZone(player, gameMap, player.get_safeX(), 
						  player.get_safeY());
        	
        	//Update game board
			gameBoard(gameMap, player, monster, teleport_pad1, teleport_pad2);
			
			level--;
		}
    	   
        count = 0;
        
        //When captured, loose a heart and print the player has been caught
        //Reset location of goose to orignal starting position
		if (captured(player, monster))
        {
        	out.writeLine("You were caught by the goose, loose a life!");
        	monster.reset_location(gooseX, gooseY);
        	hearts--;
        	count = 1;
		}	
  	}

    if (keyEntered != TK_CLOSE)
    {
      	//once we're out of the loop, the game is over
        out.writeLine("Game has ended");
    
        //output why: did the goose get us, or did we get to the win location?
    	if (level == 0)
    		out.writeLine("You escaped the goose!");
    	else 
    		out.writeLine("No lives left!");
	
    	//Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }

	//game is done, close it  
    terminal_close();
}
