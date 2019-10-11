#include "QValue.h"
#include <vector>
#include <algorithm>
#include "utils.h"

using namespace std;

QValue::QValue()
{
	//visits = { 0, 0, 0, 0 };
}

//returns random action
ROTATION_DIRECTION QValue::randomAction()
{
	int index = RandInt(0, 3);
	//do the switch case to return the string 
	switch (index) {
	case 0:
		visits[0]++;
		return EAST;
		break;
	case 1:
		visits[1]++;
		return NORTH;
		break;
	case 2:
		visits[2]++;
		return WEST;
		break;
	case 3:
		visits[3]++;
		return SOUTH;
		break;
	}
}

//return string representation of the best move from that state based on Q value
ROTATION_DIRECTION QValue::bestAction()
{
	//float qs[] = { east, north, west, south };
	int index = 0;
	float* ptr;

	//check if all are zero first and return any random
	if (allQsZero(qs)) {
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
			visits[0]++;
			return EAST;
			break;
		case 1:
			visits[1]++;
			return NORTH;
			break;
		case 2:
			visits[2]++;
			return WEST;
			break;
		case 3:
			visits[3]++;
			return SOUTH;
			break;
	}
}

//use the q updating formula to update q value for the state action pair
void QValue::updateQ(int reward, float discount, double learning_rate,
	ROTATION_DIRECTION bestMove, float maxQNextState)
{
	//Q(s,a) = reward(s,a) + discount*(Max[Q(next state, all actions)] - Q(s,a))
	float sum = (1-learning_rate)* getQ_s_a(bestMove) + 
		learning_rate * ((float)reward + (discount * maxQNextState));

	//update the relevant q value
	int index = returnIndex(bestMove);
	qs[index] = sum;
}

//returns the maximum q value amongst all the actions from that state
float QValue::maxQ()
{
	float* ptr = max_element(qs, qs + 4);
	return *ptr;
}

//return q value assoicated with an action
float QValue::getQ_s_a(ROTATION_DIRECTION bestMove)
{
	return qs[returnIndex(bestMove)];
}

//return index of action given bestMove
int QValue::returnIndex(ROTATION_DIRECTION move) {
	switch (move) {
	case EAST:
		return 0;
		break;
	case NORTH:
		return 1;
		break;
	case WEST:
		return 2;
		break;
	case SOUTH:
		return 3;
		break;
	}
}

bool QValue::allQsZero(float qs[]) {
	bool allZero = true;
	for (int i = 0; i < 4; i++) {
		if (qs[i] != 0){
			allZero = false;
			break;
		}
	}
	return allZero;
}

double QValue::updateLearningRate(ROTATION_DIRECTION action, double learning_rate) {
	int index = returnIndex(action);
	visits[index]++;
	learning_rate = 1/(1+visits[index]);
	return learning_rate;
}
