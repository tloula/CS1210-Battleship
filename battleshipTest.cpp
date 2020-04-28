#include "battleship_type_definitions.h"
#include "memory.h"
#include "battleship.h"
#include "memory_functions_loula.h"

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int main(){

	ComputerMemory memory;

	int nextRow;
	int nextCol;
	bool lost;
	string nextMove = "";

	memory.mode = RANDOM;
	memory.hitRow = -1;
	memory.hitCol = -1;
	memory.hitShip = NONE;
	memory.fireDir = NONE;
	memory.fireDist = 1;
	memory.lastResult = NONE;

	std::memset(memory.modes,0,5*sizeof(int));// Memory.modes will be used to remember which ships have been sunk
							// Index 0 is for Aircraft Carrier, 1 for Battleship, etc

	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			memory.grid[i][j] = EMPTY_MARKER;
		}
	}

	/******************** CALCULATE PROBABILITY DENSITY ********************/
	/*
	This algorithm will calculate the probability of each cell on the board containing a ship.
	*/
	int shipSize;										// Size of the current ship being checked
	int probabilityDensity[BOARDSIZE][BOARDSIZE];		// Initialize probability density grid
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			probabilityDensity[i][j] = 0;
		}
	}

	for (int h = 0; h < 5; h++) {		// Scan through ships
		if (memory.modes[h] == 0) {		// If the ship is alive, check probabilty
										// Set shipSize
			if (h + 1 == AC) { shipSize = AC_SIZE; }
			if (h + 1 == BS) { shipSize = BS_SIZE; }
			if (h + 1 == CR) { shipSize = CR_SIZE; }
			if (h + 1 == SB) { shipSize = SB_SIZE; }
			if (h + 1 == DS) { shipSize = DS_SIZE; }

			for (int i = 0; i < BOARDSIZE; i++) {		// Scan through rows
				for (int j = 0; j < BOARDSIZE; j++) {	// Scan through columns



					for (int k = 0; k < shipSize; k++) {		// Start with the ship facing north, and then scan through all possible orientations until it is facing south
						for (int l = 0; l < shipSize; l++) {	// See if the ship fits
							if (memory.grid[i - k + l][j] == EMPTY_MARKER && i - k + l < BOARDSIZE && i - k + l >= 0) {		// If there are no obstacles, continue; if there are, break
								if (l  + 1 == shipSize) { probabilityDensity[i][j] = probabilityDensity[i][j] + shipSize*shipSize; }	// if ship fits in current orientation, increase probability density for that spot
							}
							else {
								break;
							}
						}
					}
					for (int k = 0; k < shipSize; k++) {		// Start with the ship facing east, and then scan through all possible orientations until it is facing west
						for (int l = 0; l < shipSize; l++) {	// See if the ship fits
							if (memory.grid[i][j - k + l] == EMPTY_MARKER && j - k + l < BOARDSIZE && j - k + l >= 0) {		// If there are no obstacles, continue; if there are, break
								if (l + 1 == shipSize) { probabilityDensity[i][j] = probabilityDensity[i][j] + shipSize*shipSize; }	// if ship fits in current orientation, increase probability density for that spot
							}
							else {
								break;
							}
						}
					}
				}
			}
		}
	}
	// Find the cell with the highest probability
	int highestProbability = 0;
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			cout << probabilityDensity[i][j] << " ";
			if (probabilityDensity[i][j] > highestProbability) {
				highestProbability =  probabilityDensity[i][j];
				nextRow = i;
				nextCol = j;
			}
		}
		cout << endl;
	}

	return 0;

}