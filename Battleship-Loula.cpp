// ************************************************************
//
// Battleship-Loula.cpp - CS1210 Nineth Homework Program - Battleship
// Author: Trevor Loula
// Box Number: 3421
// Created: 2017-11-01
//
// ************************************************************

#include "battleship.h"

int main() {

	// variable declarations (you'll need others, of course)
	// Note: a good idea is the declare them as you need them to limit scope
	bool done = false;
	Board humanBoard;
	Board computerBoard;
	int humanRow;
	int humanCol;
	int computerRow;
	int computerCol;
	int humanPoints = 0;
	int computerPoints = 0;

	// Welcome the player to the game
	welcome();

	// Initialize the game boards
	initializeBoard(humanBoard);
	initializeBoard(computerBoard);

	// Play the game until one player has sunk the other's ships
	while (!done) {

		// Clear the screen to prepare show the game situation before the moves
		clearTheScreen();

		// Display the game board situation
		displayBoard(1, 1, HUMAN, humanBoard);
		displayBoard(1, 35, COMPUTER, computerBoard);

		// Get and validate the human player's move
		string humanMove = getResponse(16, 0, "Please enter your move: ");
		clearTheLine(16);
		while (checkMove(humanMove, computerBoard, humanRow, humanCol) != VALID_MOVE) {

			if (checkMove(humanMove, computerBoard, humanRow, humanCol) == ILLEGAL_FORMAT) {
				clearTheLine(16);
				writeMessage(16, 0, "The move has an illegal format");
			}
			else if (checkMove(humanMove, computerBoard, humanRow, humanCol) == REUSED_MOVE) {
				clearTheLine(16);
				writeMessage(16, 0, "The move " + humanMove + " has already been used");
			}
			pauseForEnter();
			clearTheLine(16);
			humanMove = getResponse(16, 0, "Please enter your move: ");
		}

		// Get and validate the computer's move
		string computerMove = randomMove();
		while (checkMove(computerMove, humanBoard, computerRow, computerCol) != VALID_MOVE) {
			computerMove = randomMove();
		}

		// Execute both moves
		int humanResult = playMove(humanRow, humanCol, computerBoard);
		int computerResult = playMove(computerRow, computerCol, humanBoard);
		
		// Clear the screen to show the new game situation after the moves
		clearTheScreen();

		// Display the new game board situation
		displayBoard(1, 1, HUMAN, humanBoard);
		displayBoard(1, 35, COMPUTER, computerBoard);

		// Display the move choices each player made
		writeMessage(16, 0, "You chose: " + humanMove);

		writeMessage(17, 0, "The computer chose: " + computerMove);

		// Show the results of the moves
		writeResult(19, 0, humanResult, HUMAN);
		writeResult(20, 0, computerResult, COMPUTER);

		// Take note if there are any sunken ships
		if (isASunk(humanResult)) { ++humanPoints; }
		if (isASunk(computerResult)) { ++computerPoints; }

		// determine if we have a winner
		if (computerPoints >= 5 || humanPoints >= 5) {
			// if one of the player's has sunk five ships the game is over
			done = true;
		}
		else {
			// pause to let the user assess the situation
			pauseForEnter();
		}
	}

	// Announce the winner
	if (humanPoints == 5) {
		writeMessage(22, 0, "You won!");
	}
	else if (computerPoints == 5) {
		writeMessage(22, 0, "The computer won :(");
	}
	else {
		writeMessage(22, 0, "The game was a tie");
	}

	// pause to let the result of the game sink in
	pauseForEnter();

	return 0;
}