#ifndef MEMORY_H
#define MEMORY_H

#include "battleship.h"

using namespace std;

#define RANDOM  1
#define SEARCH  2
#define DESTROY 3

#define NONE    0
#define NORTH   1
#define SOUTH   2
#define EAST    3
#define WEST    4

struct ComputerMemory
{
	int  hitRow, hitCol;
	int  hitShip;
	int  fireDir;
	int  fireDist;
	int  lastResult;
	int  mode;
	char grid[BOARDSIZE][BOARDSIZE];

	// optional attributes for students wanting to keep track of hits on
	// multiple ships
	int  depth;						// Don't care... Let's use this to remember how many moves just for fun
	int  hitRows[5], hitCols[5];	// Save the initial hit of each ship
	int  hitShips[5];				// Save what ships have been hit IN ORDER TO make sure that each ship is completely sunk before returning to search mode
	int  fireDirs[5];				// Save orientation of each ship
	int  fireDists[5];				// Don't care...
	int  lastResults[5];			// Don't care...
	int  modes[5];					// Let's use this to remember what ships have been sunk; 0 for alive, 1 for sunk
};

#endif // MEMORY_H
