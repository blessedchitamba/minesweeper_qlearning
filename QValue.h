#pragma once
#include <string>
#include "CDiscMinesweeper.h"
//class to hold a set of Q Values for a particular state and its associated actions

class QValue
{
	public:
		//the indices indicating its position
		int x;
		int y;

		//enum for direction
		//enum ROTATION_DIRECTION { NORTH = 1, SOUTH = 3, EAST = 0, WEST = 2 };

		//the associated Q values for each action: north, west, south, east
		float north = 0;
		float south = 0;
		float west = 0;
		float east = 0;

		//methods needed
		//returns the best action to take from this state based on which has highest q val
		ROTATION_DIRECTION bestAction();

		//updates the q value of this state associated with a particular action
		void updateQ(int reward, float discount, ROTATION_DIRECTION bestMove, float maxQNextState);

		//returns the maxQ value for the current state to all directions
		float maxQ();

		//returns the Q(s,a) value for an action based on the bestMove
		float getQ_s_a(ROTATION_DIRECTION bestMove);
};
