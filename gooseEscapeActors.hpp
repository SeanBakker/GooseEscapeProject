/*
---------------------------------------------------------------------
gooseEscapeActors.hpp Source File
---------------------------------------------------------------------
*/

#ifndef GOOSE_ESCAPE_ACTORS
#define GOOSE_ESCAPE_ACTORS
#include <cmath>
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"

/*
    Modify this class to contain more characteristics of the "actor".  Add
    functions that will be useful for playing the game that are specific to
    the Actor.
*/

class Actor
{
  private:
    int actorChar;      
    int location_x, location_y;
    
    //Data Members
    bool teleporting;
    int safeX, safeY;

  public:
    Actor()
    {
        actorChar = int('A');
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        teleporting = false;
        safeX = MIN_SCREEN_X;
        safeY = MIN_SCREEN_Y;
        put_actor();
    }

    Actor(char initPlayerChar, int x0, int y0)
    {
        change_char(initPlayerChar);
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        update_location(x0,y0);
    }
    
    int get_x() const
    {
        return location_x;
    }
    
    int get_y() const
    {
        return location_y;
    }
    
    //Set functions for setting the player safe zones
    int set_safeX(int newSafeX)
    {
    	safeX = newSafeX;
	}
	
	int set_safeY(int newSafeY)
    {
    	safeY = newSafeY;
	}
	
	//Get functions for getting the player safe zones
	int get_safeX() const
    {
        return safeX;
    }
    
    int get_safeY() const
    {
        return safeY;
    }
    
    //Function for getting the teleporting data member for the player
    bool get_teleporting_pad() const
    {
    	return teleporting;
	}
    
    string get_location_string() const
    {
        char buffer[80];
        itoa(location_x,buffer,10);
        string formatted_location = "(" + string(buffer) + ",";
        itoa(location_y,buffer,10);
        formatted_location += string(buffer) + ")";
        return formatted_location;
    }
    
    void change_char(char new_actor_char)
    {
        actorChar = min(int('~'),max(int(new_actor_char),int(' ')));
    }

    bool can_move(int delta_x, int delta_y) const
    {
        int new_x = location_x + delta_x;
        int new_y = location_y + delta_y;

        return new_x >= MIN_BOARD_X && new_x <= MAX_BOARD_X
          && new_y >= MIN_BOARD_Y && new_y <= MAX_BOARD_Y;
    }

    void update_location(int delta_x, int delta_y)
    {
        if (can_move(delta_x, delta_y))
        {
            terminal_clear_area(location_x, location_y, 1, 1);
            location_x += delta_x;
            location_y += delta_y;
            put_actor();
        }
    }
    
    //Function for teleporting the player when reached a teleportation pad
    void teleport(bool firstPad)
    {
    	if (firstPad)
		{
    		location_x = pad2_x;
    		location_y = pad2_y;
		}
    	else 
    	{
    		location_x = pad1_x;
    		location_y = pad1_y;
		}
	}
	
	//Function for setting the pad which the player is teleporting
	void teleport_pad(bool pad1)
	{
		teleporting = pad1;
	}
	
	//Reset location
	void reset_location(int new_x, int new_y)
	{
		location_x = new_x;
		location_y = new_y;
	}
    
    void put_actor() const
    {
        terminal_put(location_x, location_y, actorChar);
        terminal_refresh();
    }
    
};
#endif
