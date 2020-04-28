/**********************************************************************/
// memory_functions_loula.cpp - CS1210 Nineth Homework Program - Battleship AI
// Author: Trevor Loula
// Box Number: 3421
// Created: 2017-11-22
//
// About:
// My algorithm uses probability density to calculate which spot to chose when in random mode.
// It remembers one spot on each ship that has been hit.
// It changes modes independent of the last move.
// 
// Fun fact: There are 2,480 different ship layout possibilities at the beginning of the game.
//			 This was calculated by my probability density algorithm.
//
/**********************************************************************/
#include "memory_functions_loula.h"
#include "battleship_type_definitions.h" // TEMP - I put this here for Visual Studio to reference stuff properly

using namespace std;

/******************** INITIALIZE MEMORY FUNCTION ********************/
void initMemoryloula(ComputerMemory &memory) {
	memory.mode = RANDOM;

	// Initialize memories to zero, for some reason { 0 }; doesn't work
	// Index 0 is the AC, 1 is the BS, 2 is the CR, 3 is the SB, 4 is the DS.
	for (int i = 0; i < 5; i++) {
		memory.hitRows[i] = 0;	// Remembers the row that was last hit of each ship
		memory.hitCols[i] = 0;	// Remembers the column that was last hit of each ship
		memory.hitShips[i] = 0;	// Remembers which ships have been hit
		memory.fireDirs[i] = 0;	// Remembers each ships orientation
		memory.modes[i] = 0;	// Memory.modes will be used to remember which ships have been sunk
	}

	for (int i = 0; i < BOARDSIZE; i++) {		// Initialize grid
		for (int j = 0; j < BOARDSIZE; j++) {
			memory.grid[i][j] = EMPTY_MARKER;
		}
	}
}

/******************** SMARTMOVE FUNCTION ********************/
string smartMoveloula(const ComputerMemory &memory) {
	int nextRow;			// The next row to shoot
	int nextCol;			// The next column to shoot
	string nextMove = "";	// The formatted next move

	/******************** "RANDOM" AKA PROBABILITY DENSITY MODE ********************/
	if (memory.mode == RANDOM) {

		/********** INITIATE PROBABILITY DENSITY CALCULATION VARIABLES **********/
		int shipSize;									// Size of the current ship being checked
		int probabilityDensity[BOARDSIZE][BOARDSIZE];	// Initialize probability density grid
		for (int i = 0; i < BOARDSIZE; i++) {
			for (int j = 0; j < BOARDSIZE; j++) {
				probabilityDensity[i][j] = 0;			// Set all probabilities to 0
			}
		}

		/********** CALCULATE PROBABILITY DENSITY **********/
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
								if (memory.grid[i - k + l][j] == EMPTY_MARKER &&	// If there are no obstacles
									i - k + l < BOARDSIZE && i - k + l >= 0) {		// If in range
									if (l + 1 == shipSize) {						// If ship fits in current orientation
										probabilityDensity[i][j] = probabilityDensity[i][j] + shipSize*shipSize; // Increase probability density
									}
								}
								else {
									break;	// Ship doesn't fit
								}
							}
						}
						for (int k = 0; k < shipSize; k++) {		// Start with the ship facing east, and then scan through all possible orientations until it is facing west
							for (int l = 0; l < shipSize; l++) {	// See if the ship fits
								if (memory.grid[i][j - k + l] == EMPTY_MARKER &&	// If there are no obstacles
									j - k + l < BOARDSIZE && j - k + l >= 0) {		// If in range
									if (l + 1 == shipSize) {						// if ship fits in current orientation
										probabilityDensity[i][j] = probabilityDensity[i][j] + shipSize*shipSize;	// Increase probability density
									}
								}
								else {
									break;	// Ship doesn't fit
								}
							}
						}
					}
				}
			}
		}
		/********** FIND THE CELL WITH THE HIGHEST PROBABILITY **********/
		int highestProbability = 0;				// Remember what the highest probability is
		for (int i = 0; i < BOARDSIZE; i++) {	// Scan through entire probability grid
			for (int j = 0; j < BOARDSIZE; j++) {
				if (probabilityDensity[i][j] > highestProbability) {	// If the current cells probability is higher than the current highest
					highestProbability = probabilityDensity[i][j];		// Update the highest probability
					nextRow = i;	// Set the next row to shoot
					nextCol = j;	// Set the next column to shoot
				}
			}
		}
	}
	/******************** SEARCH MODE ********************/
	if (memory.mode == SEARCH) {

		int searchRow;	// The row containing the ship being searched for
		int searchCol;	// The column containing the ship being searched for
		int shipSize;	// The size of the ship being searched for
		char shipMarker;// The marker of the ship being searched for

		for (int i = 0; i < 5; i++) {			// Scan through all ships to find one to locate
			if (memory.modes[i] == 0 &&			// If ship is not sunk
				memory.fireDirs[i] == 0 &&		// If ship is not found
				memory.hitShips[i] == 1) {		// If ship has been hit
				searchRow = memory.hitRows[i];	// Set row to search
				searchCol = memory.hitCols[i];	// Set column to search
				if (i + 1 == AC) { shipSize = AC_SIZE; }		// Get ship size
				if (i + 1 == BS) { shipSize = BS_SIZE; }
				if (i + 1 == CR) { shipSize = CR_SIZE; }
				if (i + 1 == SB) { shipSize = SB_SIZE; }
				if (i + 1 == DS) { shipSize = DS_SIZE; }
				if (i + 1 == AC) { shipMarker = AC_MARKER; }	// Get ship marker
				if (i + 1 == BS) { shipMarker = BS_MARKER; }
				if (i + 1 == CR) { shipMarker = CR_MARKER; }
				if (i + 1 == SB) { shipMarker = SB_MARKER; }
				if (i + 1 == DS) { shipMarker = DS_MARKER; }
				break;	// We got what we need, so stop scanning through ships
			}
		}

		for (int k = 0; k < shipSize; k++) {		// Start with the ship facing north, and then scan through all possible orientations until it is facing south
			for (int l = 0; l < shipSize; l++) {	// See if the ship fits
				if (searchRow - k + l < BOARDSIZE && searchRow - k + l >= 0) {			// If in range
					if (memory.grid[searchRow - k + l][searchCol] == EMPTY_MARKER ||	// If there are no obstacles
						memory.grid[searchRow - k + l][searchCol] == shipMarker) {		// It's fine if it contains its own ship marker
						if (l + 1 == shipSize) {	// If ship fits in current orientation
							// Fire North
							if (memory.grid[searchRow - 1][searchCol] == EMPTY_MARKER && searchRow - 1 >= 0) {
								nextRow = searchRow - 1;
								nextCol = searchCol;
							}
							// Fire South
							else if (memory.grid[searchRow + 1][searchCol] == EMPTY_MARKER && searchRow + 1 < BOARDSIZE) {
								nextRow = searchRow + 1;
								nextCol = searchCol;
							}
						}
					}
					else {
						break;	// Break if there are obstacles
					}
				}
				else {
					break;		// Break if not in range on the grid
				}
			}
		}
		for (int k = 0; k < shipSize; k++) {		// Start with the ship facing east, and then scan through all possible orientations until it is facing west
			for (int l = 0; l < shipSize; l++) {	// See if the ship fits
				if (searchCol - k + l < BOARDSIZE && searchCol - k + l >= 0) {			// If in range
					if (memory.grid[searchRow][searchCol - k + l] == EMPTY_MARKER ||	// If there are no obstacles
						memory.grid[searchRow][searchCol - k + l] == shipMarker) {		// It's fine if it contains its own ship marker
						if (l + 1 == shipSize) {	// if ship fits in current orientation
							// Fire East
							if (memory.grid[searchRow][searchCol - 1] == EMPTY_MARKER && searchCol - 1 >= 0) {
								nextRow = searchRow;
								nextCol = searchCol - 1;
							}
							// Fire West
							else if (memory.grid[searchRow][searchCol + 1] == EMPTY_MARKER && searchCol + 1 < BOARDSIZE) {
								nextRow = searchRow;
								nextCol = searchCol + 1;
							}
						}
					}
					else {
						break;	// Break if there are obstacles
					}
				}
				else {
					break;		// Break if not in range on the grid
				}
			}
		}
	}
	/******************** DESTROY MODE ********************/
	if (memory.mode == DESTROY) {

		/********** GET SHIP INFORMATION **********/
		int shipSize;												// Size of ship
		char shipMarker;											// Marker of ship
		int fireDirection;											// Direction of ship
		int hitRow;													// Row of ship
		int hitCol;													// Column of ship
		for (int i = 0; i < 5; i++) {								// Scan through all ships to find one ready to be destroyed
			if (memory.fireDirs[i] != 0 && memory.modes[i] == 0) {	// If ship has been found and not sunk
				fireDirection = memory.fireDirs[i];					// Get fire direction found by search mode
				if (i + 1 == AC) { shipSize = AC_SIZE; }			// Get ship size
				if (i + 1 == BS) { shipSize = BS_SIZE; }
				if (i + 1 == CR) { shipSize = CR_SIZE; }
				if (i + 1 == SB) { shipSize = SB_SIZE; }
				if (i + 1 == DS) { shipSize = DS_SIZE; }
				if (i + 1 == AC) { shipMarker = AC_MARKER; }		// Get ship marker
				if (i + 1 == BS) { shipMarker = BS_MARKER; }
				if (i + 1 == CR) { shipMarker = CR_MARKER; }
				if (i + 1 == SB) { shipMarker = SB_MARKER; }
				if (i + 1 == DS) { shipMarker = DS_MARKER; }
				hitRow = memory.hitRows[i];							// Set row of ship
				hitCol = memory.hitCols[i];							// Set column of ship
				break;												// We got what we need, so break
			}
		}
		/********** CALCULATE NEXT SHOT **********/
		bool northCap = true;	// Keeps it from shooting past a miss marker
		bool southCap = true;
		bool eastCap = true;
		bool westCap = true;

		if (fireDirection == NORTH || fireDirection == SOUTH) {				// Vertically oriented ship
			for (int i = 1; i <= shipSize; i++) {
				// Check for Miss Marker to the NORTH
				if (hitRow - i >= 0 && northCap) {							// If in bounds and not already failed
					if (memory.grid[hitRow - i][hitCol] != EMPTY_MARKER &&	// If not empty
						memory.grid[hitRow - i][hitCol] != shipMarker) {	// If not the ships own marker
						northCap = false;									// There is an obstacle, so tell it not to fire north
					}	
				}
				// Check for Miss Marker to the SOUTH
				if (hitRow + i >= 0 && southCap) {							// If in bounds and not already failed
					if (memory.grid[hitRow + i][hitCol] != EMPTY_MARKER &&	// If not empty
						memory.grid[hitRow + i][hitCol] != shipMarker) {	// If not the ships own marker
						southCap = false;									// There is an obstacle, so tell it not to fire south
					}
				}
				if (memory.grid[hitRow - i][hitCol] == EMPTY_MARKER && hitRow - i >= 0 && northCap) {				// Fire NORTH
					nextRow = hitRow - i;
					nextCol = hitCol;
					break;	// Fire directions given, so break
				} else if (memory.grid[hitRow + i][hitCol] == EMPTY_MARKER && hitRow + i < BOARDSIZE && southCap) {	// Fire SOUTH
					nextRow = hitRow + i;
					nextCol = hitCol;
					break;	// Fire direction given, so break
				}
			}
		}
		if (fireDirection == EAST || fireDirection == WEST) {				// Horizantally oriented ship
			for (int i = 1; i <= shipSize; i++) {
				// Check for Miss Marker to the EAST
				if (hitRow - i >= 0 && eastCap) {							// If in bounds and not already failed
					if (memory.grid[hitRow][hitCol + i] != EMPTY_MARKER &&	// If not empty
						memory.grid[hitRow][hitCol + i] != shipMarker) {	// If not the ships own marker
						eastCap = false;									// There is an obstacle, so tell it nor to fire east
					}
				}
				// Check for Miss Marker to the WEST
				if (hitRow + i >= 0 && westCap) {							// If in bounds and not already failed
					if (memory.grid[hitRow][hitCol - i] != EMPTY_MARKER &&	// If not empty
						memory.grid[hitRow][hitCol - i] != shipMarker) {	// If not the ships own marker
						westCap = false;									// There is an obstacle, so tell it not to fire south
					}
				}
				if (memory.grid[hitRow][hitCol + i] == EMPTY_MARKER && hitCol + i < BOARDSIZE && eastCap) {		// Fire EAST
					nextRow = hitRow;
					nextCol = hitCol + i;
					break;	// Fire directions given, so break
				} else if (memory.grid[hitRow][hitCol - i] == EMPTY_MARKER && hitCol - i >= 0 && westCap) {		// Fire WEST
					nextRow = hitRow;
					nextCol = hitCol - i;
					break;	// Fire direction given, so break
				}
			}
		}
	}
	/***** FORMAT AND RETURN NEXTMOVE *****/
	nextMove = nextMove + (char)('A' + nextRow);
	if (nextCol == 9) {
		nextMove = nextMove + "10";
	}
	else {
		nextMove = nextMove + (char)('1' + nextCol);
	}
	return nextMove;
	}

	/******************** UPDATE MEMORY FUNCTION  *************************/
	void updateMemoryloula(int row, int col, int result, ComputerMemory &memory) {

		/*************** IF MISS ***************/
		if (isAMiss(result)) {
			memory.grid[row][col] = MISS_MARKER;	// Add miss marker to the grid
		}

		/*************** IF HIT ***************/
		if (isAHit(result)) {

			// Once SEARCH finds direction of ship, store direction
			if (memory.mode == SEARCH) {							// If we got a hit in search mode, that means that the ship has two hits, so we can find its direction
				int hitShip = isShip(result);						// Ship that was just hit
				if (memory.hitShips[hitShip - 1] == 1) {			// If the ship that we just hit has already been hit
					if (memory.hitRows[hitShip - 1] - 1 == row) {	// If the last hit on the ship - 1 is where the current hit was
						memory.fireDirs[hitShip - 1] = NORTH;		// The ship is facing NORTH, or well Vertically is all that matters
					}
					if (memory.hitRows[hitShip - 1] + 1 == row) {
						memory.fireDirs[hitShip - 1] = SOUTH;
					}
					if (memory.hitCols[hitShip - 1] + 1 == col) {
						memory.fireDirs[hitShip - 1] = EAST;
					}
					if (memory.hitCols[hitShip - 1] - 1 == col) {
						memory.fireDirs[hitShip - 1] = WEST;
					}
				}
			}

			// Update grid markers for the ship that was just hit
			if (isShip(result) == AC) {				// Aircraft Carrier
				memory.hitRows[0] = row;
				memory.hitCols[0] = col;
				memory.hitShips[0] = 1;
				memory.grid[row][col] = AC_MARKER;
			}
			else if (isShip(result) == BS) {		// Battleship
				memory.hitRows[1] = row;
				memory.hitCols[1] = col;
				memory.hitShips[1] = 1;
				memory.grid[row][col] = BS_MARKER;
			}
			else if (isShip(result) == CR) {		// Cruiser
				memory.hitRows[2] = row;
				memory.hitCols[2] = col;
				memory.hitShips[2] = 1;
				memory.grid[row][col] = CR_MARKER;
			} 
			else if (isShip(result) == SB) {		// Submarine
				memory.hitRows[3] = row;
				memory.hitCols[3] = col;
				memory.hitShips[3] = 1;
				memory.grid[row][col] = SB_MARKER;
			}
			else if (isShip(result) == DS) {		// Destroyer
				memory.hitRows[4] = row;
				memory.hitCols[4] = col;
				memory.hitShips[4] = 1;
				memory.grid[row][col] = DS_MARKER;
			}
		}

		/*************** IF SUNK ***************/
		if (isASunk(result)) {
			int sunkShip = isShip(result);		// Ship that was just sunk
			memory.modes[sunkShip - 1] = 1;		// Remember that ship has been sunk
		}

		/*************** DECIDE WHICH MODE TO ENTER NEXT ***************/
		// Now that everything has been updated, this decides which mode to enter next based on the current information
		for (int i = 0; i < 5; i++) {									// Scan through all ships
			/********** ENTER SEARCH MODE **********/
			if (memory.hitShips[i] == 1 && memory.fireDirs[i] == 0) {	// If a ship has been hit but not found
				memory.mode = SEARCH;	// Enter SEARCH mode
				break;	// We know what we are doing, so break
			}
			/********** ENTER DESTROY MODE **********/
			else if (memory.fireDirs[i] != 0 && memory.modes[i] == 0) {	// If a ship has been found but not sunk
				memory.mode = DESTROY;	// Enter DESTROY mode
				break;	// We know what we are doint, so break
			}
			else {	// If all ships that have been hit have been found and sunk, return to random mode
				memory.mode = RANDOM;	// Enter RANDOM mode
			}
		}
	}