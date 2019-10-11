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
		//double learning_rate = 0.7;

		//array with number of times the particular s,a pair is being visited.
		//to be used in decaying learning rate
		int visits[4] = { 0,0, 0, 0 };

		//the associated Q values for each action: east, north, west, south
		float qs[4] = { 0, 0, 0, 0 };

		//methods needed
		//constructor
		QValue();
		//returns the best action to take from this state based on which has highest q val
		ROTATION_DIRECTION bestAction();

		//returns a random action
		ROTATION_DIRECTION randomAction();

		//updates the q value of this state associated with a particular action
		void updateQ(int reward, float discount, double learning_rate,
			ROTATION_DIRECTION bestMove, float maxQNextState);

		//returns the maxQ value for the current state to all directions
		float maxQ();

		//returns the Q(s,a) value for an action based on the bestMove
		float getQ_s_a(ROTATION_DIRECTION bestMove);

		//returns true if all the q values are 0
		bool allQsZero(float qs[]);

		int returnIndex(ROTATION_DIRECTION move);

		//update the learning rate
		double updateLearningRate(ROTATION_DIRECTION action, double learning_rate);
};
