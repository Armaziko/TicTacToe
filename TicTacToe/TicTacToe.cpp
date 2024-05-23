#include <iostream>
#include <cstdlib> 
#include <random>
#include <vector>
#include <utility> 

//Integer to count both, player and computer moves
int countMoves = 0;
//Boolean to check if the player is finished or wants a rematch
bool finished = false;
//Vector to store pair of coordinates of each move
std::vector<std::pair<int, int>> Moves;
//TicTacToe graph(board)
char board[3][3] = { 
					 {'#','#','#'},
					 {'#','#','#'},
					 {'#','#','#'} 
				   };

//Function declarations so that restarting becomes simpler
void go();
void restartGame();

//Function to paint the board to console
void paintBoard() {
	for (int i = 0; i < 3; i++) {
		for (auto incr : board[i]) {
			std::cout << incr << " ";
		}
		std::cout << std::endl;
	}
}

//Function to restart the game
void restartGame() {
	int input = 0;
	std::cout << "Do you wish to restart the game? (1-YES, 2-NO):";
	while (input != 1 && input != 2) {
		std::cin >> input;
	}
	if (input == 1)
	{
		countMoves = 0;
		finished = false;
		Moves.clear();
		for (auto x : board) {
			for (int i = 0; i < 3; i++) {
				x[i] = '#';
			}
		}
		go();
	}
}

// Function to return the result of the current game (win = 2, lose = 1, draw = 0, unfinished = -1)
int checkResult() {
	// Horizontally checking if there is 3 'X' or 3 'O'
	for (int i = 0; i < 3; i++) {
		int xonlines = 0, oonlines = 0;
		for (int b = 0; b < 3; b++) {
			if (board[i][b] == 'X') xonlines++;
			else if (board[i][b] == 'O') oonlines++;
		}
		if (xonlines == 3) return 2;
		if (oonlines == 3) return 1;
	}

	// Vertically checking if there is 3 'X' or 3 'O'
	for (int i = 0; i < 3; i++) {
		int xonlines = 0, oonlines = 0;
		for (int b = 0; b < 3; b++) {
			if (board[b][i] == 'X') xonlines++;
			else if (board[b][i] == 'O') oonlines++;
		}
		if (xonlines == 3) return 2;
		if (oonlines == 3) return 1;
	}

	// Diagonally checking if there is 3 'X' or 3 'O'
	int xonlines = 0, oonlines = 0;
	for (int i = 0; i < 3; i++) {
		if (board[i][i] == 'X') xonlines++;
		else if (board[i][i] == 'O') oonlines++;
	}
	if (xonlines == 3) return 2;
	if (oonlines == 3) return 1;

	xonlines = 0, oonlines = 0;
	for (int i = 0; i < 3; i++) {
		if (board[i][2 - i] == 'X') xonlines++;
		else if (board[i][2 - i] == 'O') oonlines++;
	}
	if (xonlines == 3) return 2;
	if (oonlines == 3) return 1;

	// Check if the board is full
	if (countMoves == 9) return 0;

	// Game is still ongoing
	return -1;
}
//Function that calculates computer's next move
void computerMove() {
	int result = checkResult();
	std::pair<int, int> temp;
	if (result == -1) {
		//Integer that resembles amount of 'X's on a line on a graph(board)
		bool moveMade = false;
		//Horizontally checking for risky future moves of player
		if (!moveMade) {
			for (int i = 0; i < 3; i++) {
				int xonlines = 0, xpos = 0, ypos = 0;
				for (int b = 0; b < 3; b++) {
					if (board[i][b] == 'X') xonlines++;
					else { xpos = i; ypos = b; }
				}
				if (xonlines == 2 && board[xpos][ypos] != 'O') { board[xpos][ypos] = 'O'; moveMade = true; temp.first = xpos; temp.second = ypos; break; }
				else { xonlines = 0; continue; }
			}
		}
		//Vertically checking for risky future moves of player
		if (!moveMade) {
			for (int i = 0; i < 3; i++) {
				int xonlines = 0, xpos = 0, ypos = 0;
				for (int b = 0; b < 3; b++) {
					if (board[b][i] == 'X') xonlines++;
					else { xpos = b; ypos = i; }
				}
				if (xonlines == 2 && board[xpos][ypos] != 'O') { board[xpos][ypos] = 'O'; moveMade = true; temp.first = xpos; temp.second = ypos;  break; } // Shouldn't place X on one place twice
				else { xonlines = 0; continue; }
			}
		}
		//Diagonally checking for risky future moves of player
		if (!moveMade) {
			int xonlines = 0, xypos = 0;
			for (int i = 0; i < 3; i++) {
				if (board[i][i] == 'X') xonlines++;
				else { xypos = i; }
			}
			if (xonlines == 2 && board[xypos][xypos] != 'O') { board[xypos][xypos] = 'O'; temp.first = xypos; temp.second = xypos; moveMade = true; }
		}
		if (!moveMade) {
			int xonlines = 0, xpos = 0, ypos = 0;
			for (int i = 2; i >= 0; i--) {
				if (board[i][2 - i] == 'X') xonlines++;
				else { xpos = i; ypos = 2 - i; }
			}
			if (xonlines == 2 && board[xpos][ypos] != 'O') { board[xpos][ypos] = 'O'; temp.first = xpos; temp.second = ypos; moveMade = true; }
		}
		if (!moveMade) {
			// Vector to store free positions
			std::vector<std::pair<int, int>> freePositions;

			// Collect all free positions
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if (board[i][j] == '#') {
						freePositions.push_back({ i, j });
					}
				}
			}

			if (!freePositions.empty()) {
				// Create a random device to seed the random number generator
				std::random_device rd;
				// Create a Mersenne Twister random number generator
				std::mt19937 gen(rd());
				// Define a uniform integer distribution for the index of the freePositions vector
				std::uniform_int_distribution<> distrib(0, freePositions.size() - 1);

				// Generate a random index to select a free position
				int randomIndex = distrib(gen);
				int ran1 = freePositions[randomIndex].first;
				int ran2 = freePositions[randomIndex].second;

				// Place 'O' at the randomly selected free position
				board[ran1][ran2] = 'O';
				temp.first = ran1; temp.second = ran2;
				moveMade = true;
			}
		}
		countMoves++;
		Moves.push_back(temp);
	}
	else {
		if (result == 0) std::cout << "It's a draw!!" << std::endl;
		else if (result == 1) std::cout << "You lost! Good luck next time!" << std::endl;
		else std::cout << "You won! Great job!" << std::endl;
		paintBoard();
		finished = true;
		restartGame();
		system("pause");
	}
	result = checkResult();
	if (!finished) {
		if (result == 0) std::cout << "It's a draw!!" << std::endl;
		else if (result == 1) std::cout << "You lost! Good luck next time!" << std::endl;
		else if(result == 2)std::cout << "You won! Great job!" << std::endl; 
		if(result != -1){
			finished = true;
		}
		paintBoard();
	}
}

//Funtcion that takes user input
void playerMove() {
	if (!finished) {
		std::cout << "It is your move! Choose where to place X(input should be like this: 0 0):" << std::endl;

		paintBoard();
		bool alreadyUsed = true;
		int x = -1, y = -1;
		while (alreadyUsed) {
			x = -1, y = -1; 
			while (x != 0 && x != 1 && x != 2) {
				std::cout << "Please input X location: ";
				std::cin >> x;
				std::cout << std::endl;
			}
			while (y != 0 && y != 1 && y != 2) {
				std::cout << "Please input Y location: ";
				std::cin >> y;
				std::cout << std::endl;
			}
			for (auto coor : Moves) {
				if (coor.first == x && coor.second == y) {
					alreadyUsed = true;
					break;
				}
				else alreadyUsed = false;
			}
			if (countMoves == 0) alreadyUsed = false;
		}
		std::pair<int, int> temp;
		temp.first = x;
		temp.second = y;
		Moves.push_back(temp);
		board[x][y] = 'X';
		countMoves++;
		computerMove();
		system("cls");
	}
}
//Function that starts or restarts the game
void go() {
	while (!finished) playerMove();
}
//Main function
int main()
{
	go();
}
