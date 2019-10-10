#include "QValue.h"
#include <vector>
#include <algorithm>

using namespace std;

//return string representation of the best move from that state based on Q value
ROTATION_DIRECTION QValue::bestAction()
{
	float qs[] = { east, north, west, south };
	int index = 0;
	float* ptr;

	//check if all are zero first and return any random
	if (north==0 && south==0 && east==0 && west==0) {
		index = rand() % 4;
	}
	else {
		//else return the max of qs
		ptr = max_element(qs, qs + 4);

		//determine the index position of the pointed to 
		for (int i = 0; i < 4; i++) {
			if (qs[i] == *ptr) {
				index = i;
				break;
			}
		}
	}

	//do the switch case to return the string 
	switch (index) {
		case 0:
			return EAST;
			break;
		case 1:
			return NORTH;
			break;
		case 2:
			return WEST;
			break;
		case 3:
			return SOUTH;
			break;
	}
}

//use the q updating formula to update q value for the state action pair
void QValue::updateQ(int reward, float discount, ROTATION_DIRECTION bestMove, float maxQNextState)
{
	//Q(s,a) = reward(s,a) + discount*(Max[Q(next state, all actions)] - Q(s,a))
	float sum = (float)reward + discount * (maxQNextState - getQ_s_a(bestMove));

	//update the relevant q value
	switch (bestMove) {
	case EAST:
		east=sum;
		break;
	case NORTH:
		north=sum;
		break;
	case WEST:
		west=sum;
		break;
	case SOUTH:
		south=sum;
		break;
	}
}

//returns the maximum q value amongst all the actions from that state
float QValue::maxQ()
{
	float qs[] = { east, north, west, south };
	float* ptr = max_element(qs, qs + 4);
	return *ptr;
}

float QValue::getQ_s_a(ROTATION_DIRECTION bestMove)
{
	switch (bestMove) {
	case EAST:
		return east;
		break;
	case NORTH:
		return north;
		break;
	case WEST:
		return west;
		break;
	case SOUTH:
		return south;
		break;
	}
}
