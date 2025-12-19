// 2048.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

const int MaxSize = 10;
const int MinSize = 4;
const int MaxNameSize = 100;

int CalculateScore(int board[MaxSize][MaxSize], int size)
{
	int score = 0;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			score += board[i][j];
		}
	}
	return score;
}
void printBoard(int board[MaxSize][MaxSize], int size)
{
	int score = CalculateScore(board, size);
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			std::cout << board[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Score: " << score << std::endl;
}

bool placeRandomTile(int board[MaxSize][MaxSize], int size, int& full)
{
	while (true)
	{
		if (full == size * size)
			return false;
		int row = rand() % size;
		int col = rand() % size;
		if (board[row][col] == 0)
		{
			int value = (rand() % 10 == 0) ? 4 : 2;
			board[row][col] = value;
			++full;
			break;
		}
	}
	return true;
}
void clearConsole()
{
	system("cls");
}
void MoveTiles(int board[MaxSize][MaxSize], int size, char direction)
{
	if (direction == 'w' || direction == 'W')
	{
		// Move Up
	}
	else if (direction == 's' || direction == 'S')
	{
		// Move Down
	}
	else if (direction == 'a' || direction == 'A')
	{
		for (int i = 0; i < size; ++i) {
			for (int j = 1; j < size; ++j)
			{
				if (board[i][j - 1] == 0)
				{
					board[i][j - 1] = board[i][j];
					board[i][j] = 0;
				}
				else if (board[i][j - 1] == board[i][j])
				{
					board[i][j - 1] *= 2;
					board[i][j] = 0;
				}
			}
		}
	}
	else if (direction == 'd' || direction == 'D')
	{

	}
}
void printWelcomeMessage()
{
	std::cout << "=========================\n";
	std::cout << "      WELCOME TO        \n";
	std::cout << "        2048!           \n";
	std::cout << "=========================\n";
}
void PrepareGame(int& size, char* name)
{
	std::cout << "Enter your name: (MAX 100 Characters)";
	std::cin >> name;
	std::cout << "Enter board size (4-10): ";
	std::cin >> size;
	while (size < MinSize || size > MaxSize)
	{
		std::cout << "Invalid size! Enter board size (4-10): ";
		std::cin >> size;
	}
}
void StartGame()
{
	int size = 0;
	char name[MaxNameSize] = {};
	PrepareGame(size, name);
	int board[MaxSize][MaxSize] = { 0 };
	int full = 0;
	while (true)
	{
		clearConsole();
		if (placeRandomTile(board, size, full)) {
			printBoard(board, size);
			std::cout << "Use W/A/S/D to move tiles, Q to quit: ";
			char move;
			std::cin >> move;
			if (move == 'Q' || move == 'q')
				break;
			MoveTiles(board, size, move);
		}
		else {
			std::cout << "Game Over! No more moves possible.\n";
			break;
		}
	}
	std::cout << BLUE << "hello world" << RESET << std::endl;
}

int main()
{
	int i;
	for (i = 0; i < 900000000; i++) {
		//std::cout << i << "\n";
		int* p = new int(5);
	}
	std::cout << i;
	/*while (true)
	{
		printWelcomeMessage();
		std::cout << "[1] Start Game" << std::endl;
		std::cout << "[2] Leaderboard" << std::endl;
		std::cout << "[3] Exit" << std::endl;
		std::cout << "Enter your choice: ";
		int choice;
		std::cin >> choice;
		if (choice == 1)
			clearConsole(), printWelcomeMessage(),
			StartGame();
		else if (choice == 2)
		{
			clearConsole();
			std::cout << "Leaderboard is empty!\n";
		}
		else if (choice == 3)
		{
			return 0;
		}
		else
		{
			clearConsole();
			std::cout << "Invalid choice! Try again.\n";
		}

	}
	std::cout << "Hello World!\n";*/


}