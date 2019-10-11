/**
         (                                      
   (     )\ )                                   
 ( )\   (()/(   (    ) (        (        (  (   
 )((_)   /(_)) ))\( /( )(   (   )\  (    )\))(  
((_)_   (_))  /((_)(_)|()\  )\ |(_) )\ )((_))\  
 / _ \  | |  (_))((_)_ ((_)_(_/((_)_(_/( (()(_) 
| (_) | | |__/ -_) _` | '_| ' \)) | ' \)) _` |  
 \__\_\ |____\___\__,_|_| |_||_||_|_||_|\__, |  
                                        |___/   

Refer to Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
for a detailed discussion on Q Learning
*/
#include "CQLearningController.h"
#include "QValue.h"
#include "CCollisionObject.h"


CQLearningController::CQLearningController(HWND hwndMain):
	CDiscController(hwndMain),
	_grid_size_x(CParams::WindowWidth / CParams::iGridCellDim + 1),
	_grid_size_y(CParams::WindowHeight / CParams::iGridCellDim + 1)
{
}
/**
 The update method should allocate a Q table for each sweeper (this can
 be allocated in one shot - use an offset to store the tables one after the other)

 You can also use a boost multiarray if you wish
*/
void CQLearningController::InitializeLearningAlgorithm(void)
{
	//TODO
	//create a 2D matrix of QValue instances
	//first dimension is the number of states and second is number of sweepers
	int vecSize = (CParams::WindowWidth+1) * (CParams::WindowHeight+1);
	for (int i = 0; i < vecSize; i++) {
		QTable.push_back(QValue());
	}
}
/**
 The immediate reward function. This computes a reward upon achieving the goal state of
 collecting all the mines on the field. It may also penalize movement to encourage exploring all directions and 
 of course for hitting supermines/rocks!
*/
double CQLearningController::R(uint x,uint y, uint sweeper_no){
	//TODO: roll your own here!
	/*
	For each action into a mine -> +50
	For each action into a supermine -> -50
	For each other action -> -5
	*/
	CCollisionObject::ObjectType objType;
	int index = findObject(x, y, m_vecObjects);
	if (index < m_vecObjects.size()) {
		objType = m_vecObjects[index]->getType();
		if (!m_vecObjects[index]->isDead() && objType == CCollisionObject::ObjectType::Mine) {
			//the current object in the objects vector is a mine so reward
			return 50;
		}
		else if(!m_vecObjects[index]->isDead() && objType == CCollisionObject::ObjectType::SuperMine){
			//else you've found a super mine: penalize
			return -50;
		}
		else {
			//else the object is dead: just receive a normal -5
			return -5;
		}
	}
	//else not found, meaning the transition is to an empty position
	else {
		return -5;
	}
}
/**
The update method. Main loop body of our Q Learning implementation
See: Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
*/
bool CQLearningController::Update(void)
{
	//m_vecSweepers is the array of minesweepers
	//everything you need will be m_[something] ;)
	uint cDead = std::count_if(m_vecSweepers.begin(),
							   m_vecSweepers.end(),
						       [](CDiscMinesweeper * s)->bool{
								return s->isDead();
							   });
	if (cDead == CParams::iNumSweepers){
		printf("All dead ... skipping to next iteration\n");
		m_iTicks = CParams::iNumTicks;
	}

	SVector2D<int> position, prevPosition;
	ROTATION_DIRECTION bestMove{};
	int reward;
	float maxQ;
	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		/**
		Q-learning algorithm according to:
		Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
		*/
		//1:::Observe the current state:
		//get the m_vposition variable of sweeper
		position = m_vecSweepers[sw]->Position();

		//2:::Select action with highest historic return:
		//epsilon greedy strategy:
		float randomFloat = RandFloat();
		if (randomFloat < epsilon) {
			//explore
			bestMove = QTable[getIndex(position)].randomAction();
		}
		else {
			//get the greedy one
			bestMove = QTable[getIndex(position)].bestAction();
		}
		//update the learning rate to decay
		QTable[getIndex(position)].updateLearningRate(bestMove, learning_rate);

		//based on the best move, update the rotation direction of sweeper
		m_vecSweepers[sw]->setRotation(bestMove);
		//now call the parents update, so all the sweepers fulfill their chosen action
	}

	//update epsilon
	updateEpsilon();
	
	CDiscController::Update(); //call the parent's class update. Do not delete this.
	
	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		//TODO:compute your indexes.. it may also be necessary to keep track of the previous state
		//3:::Observe new state:
		//get new position of sweeper and locate its QValue object in QTable. get reward
		position = m_vecSweepers[sw]->Position();
		reward = R(position.x, position.y, sw);
		//get the maxQ value from the current state to be used in updateQ() for previous state
		maxQ = QTable[getIndex(position)].maxQ();
		//also get previous position so as to access its previous QTable entry
		prevPosition = m_vecSweepers[sw]->PrevPosition();
		//QTable[getIndex(prevPosition)].updateLearningRate(bestMove, learning_rate);
		QTable[getIndex(prevPosition)].updateQ(reward, discount, learning_rate, bestMove, maxQ);
		//4:::Update _Q_s_a accordingly:
		//TODO
	}
	return true;
}

/*
Method checks the vector m_vecObjects to see if it can 
find any with getPosition()==(x,y) and returns its
index position in the array.
getPosition() returns a SVector object
//
*/
int CQLearningController::findObject(uint x, uint y, vector<CDiscCollisionObject*>& m_vecObjects)
{
	int index = 0;
	for (auto obj : m_vecObjects) {
		if (obj->getPosition().x == x && obj->getPosition().y == y) {
			break;
		}
		index++;
	}
	return index;
}

int CQLearningController::getIndex(SVector2D<int> position)
{
	return (position.y * CParams::WindowWidth) + position.x;
}

void CQLearningController::updateEpsilon()
{
	epsilon = 1 / (1 - epsilon);
}

CQLearningController::~CQLearningController(void)
{
	//TODO: dealloc stuff here if you need to	
}
