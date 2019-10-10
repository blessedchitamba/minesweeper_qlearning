#pragma once
// structure to hold a set of Q Values for a particular state and its associated actions

struct QValue {
	//the indices indicating its position
	int x;
	int y;

	//the associated Q values for each action: north, west, south, east
	float north = 0;
	float south = 0;
	float west = 0;
	float east = 0;
};