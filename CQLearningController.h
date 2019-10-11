#pragma once
#include "cdisccontroller.h"
#include "CParams.h"
#include "QValue.h"
#include "CDiscCollisionObject.h"
#include <cmath>

typedef unsigned int uint;
class CQLearningController :
	public CDiscController
{
private:
	uint _grid_size_x;
	uint _grid_size_y;
	vector<QValue> QTable;
	double const discount = 0.8;
	double learning_rate = 0.7;
	double epsilon = 1;	//used for epsilon greedy strategy
public:
	CQLearningController(HWND hwndMain);
	virtual void InitializeLearningAlgorithm(void);
	double R(uint x, uint y, uint sweeper_no);
	virtual bool Update(void);
	virtual ~CQLearningController(void);
	virtual int findObject(uint x, uint y, vector<CDiscCollisionObject*>& m_vecObjects);
	virtual int getIndex(SVector2D<int> position);	//return the index corresponding to the qtable vector
	virtual void updateEpsilon();	//update the epsilon value and make it lower
};

