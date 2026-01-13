// 2048.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include<ctime>
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <ctime>

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
const char* colors[] = {
	RESET,
	/*BLACK,*/
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	BOLDBLACK,
	BOLDRED,
	BOLDGREEN,
	BOLDYELLOW,
	BOLDBLUE,
	BOLDMAGENTA,
	BOLDCYAN,
	BOLDWHITE
};

const int COLOR_COUNT = 16;


std::mt19937 randomGenerator(std::random_device{}());

const int MaxSize = 10;
const int MinSize = 4;
const int MaxNameSize = 100;

int getpow(int number) {
	int power = 0;
	while (number > 1) {
		number /= 2;
		power++;
	}
	return power;
}
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
			std::cout << colors[getpow(board[i][j])] << board[i][j] << colors[0] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Score: " << score << std::endl;
}

void placeRandomTile(int board[MaxSize][MaxSize], int size)
{
	srand(time(nullptr));
	int emptyCount = 0;

	// броим празните клетки
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0)
				emptyCount++;
		}
	}

	// ако няма празни клетки

	// избираме случайна празна позиция
	int target = rand() % emptyCount;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				if (target == 0) {
					board[i][j] = (rand() % 10 == 0) ? 4 : 2;
					return;
				}
				target--;
			}
		}
	}
}
void clearConsole()
{
	system("cls");
}

bool MoveTiles(int board[MaxSize][MaxSize], int size, char direction)
{
	int curentboard[MaxSize][MaxSize] = { 0 };
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			curentboard[r][c] = board[r][c];
		}
	}

	if (direction == 'w' || direction == 'W') // Up
	{
		for (int j = 0; j < size; j++) // column
		{
			bool merged[MaxSize] = { false }; // track merged tiles
			for (int i = 1; i < size; i++)
			{
				if (board[i][j] == 0) continue;
				int row = i;
				while (row > 0 && board[row - 1][j] == 0)
				{
					board[row - 1][j] = board[row][j];
					board[row][j] = 0;
					row--;
				}
				if (row > 0 && board[row - 1][j] == board[row][j] && !merged[row - 1])
				{
					board[row - 1][j] *= 2;
					board[row][j] = 0;
					merged[row - 1] = true;
				}
			}
		}
	}
	else if (direction == 's' || direction == 'S') // Down
	{
		for (int j = 0; j < size; j++)
		{
			bool merged[MaxSize] = { false };
			for (int i = size - 2; i >= 0; i--)
			{
				if (board[i][j] == 0) continue;
				int row = i;
				while (row < size - 1 && board[row + 1][j] == 0)
				{
					board[row + 1][j] = board[row][j];
					board[row][j] = 0;
					row++;
				}
				if (row < size - 1 && board[row + 1][j] == board[row][j] && !merged[row + 1])
				{
					board[row + 1][j] *= 2;
					board[row][j] = 0;
					merged[row + 1] = true;
				}
			}
		}
	}
	else if (direction == 'a' || direction == 'A') // Left
	{
		for (int i = 0; i < size; i++)
		{
			bool merged[MaxSize] = { false };
			for (int j = 1; j < size; j++)
			{
				if (board[i][j] == 0) continue;
				int col = j;
				while (col > 0 && board[i][col - 1] == 0)
				{
					board[i][col - 1] = board[i][col];
					board[i][col] = 0;
					col--;
				}
				if (col > 0 && board[i][col - 1] == board[i][col] && !merged[col - 1])
				{
					board[i][col - 1] *= 2;
					board[i][col] = 0;
					merged[col - 1] = true;
				}
			}
		}
	}
	else if (direction == 'd' || direction == 'D') // Right
	{
		for (int i = 0; i < size; i++)
		{
			bool merged[MaxSize] = { false };
			for (int j = size - 2; j >= 0; j--)
			{
				if (board[i][j] == 0) continue;
				int col = j;
				while (col < size - 1 && board[i][col + 1] == 0)
				{
					board[i][col + 1] = board[i][col];
					board[i][col] = 0;
					col++;
				}
				if (col < size - 1 && board[i][col + 1] == board[i][col] && !merged[col + 1])
				{
					board[i][col + 1] *= 2;
					board[i][col] = 0;
					merged[col + 1] = true;
				}
			}
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (curentboard[i][j] != board[i][j]) {
				return true;
			}
		}
	}
	return false;
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
	std::cout << "Enter your name: (MAX 100 Characters) ";
	std::cin >> name;
	std::cout << "Enter board size (4-10): ";
	std::cin >> size;

	while (size < MinSize || size > MaxSize)
	{
		std::cout << "Invalid size! Enter board size (4-10): ";
		std::cin >> size;
	}
}
bool IsGameOver(int board[MaxSize][MaxSize], int size)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (board[i][j] == 0)
				return false;
		}
	}
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size - 1; ++j)
		{
			if (board[i][j] == board[i][j + 1])
				return false;
		}
	}
	for (int j = 0; j < size; ++j)
	{
		for (int i = 0; i < size - 1; ++i)
		{
			if (board[i][j] == board[i + 1][j])
				return false;
		}
	}
	return true;
}
void StartGame()
{
	int size = 0;
	char name[MaxNameSize] = {};
	PrepareGame(size, name);
	//size = 2;
	int board[MaxSize][MaxSize] = { 0 };
	int full = 0;
	bool quit = false;
	bool ended = false;
	while (true)
	{
		clearConsole();
		placeRandomTile(board, size);
		char move;
		do {
			printBoard(board, size);
			if (IsGameOver(board, size)) {
				ended = true;
				break;
			}
			std::cout << "Use W/A/S/D to move tiles, Q to quit: ";
			std::cin >> move;
			if (move == 'Q' || move == 'q') {
				quit = true;
				break;
			}
		} while (!MoveTiles(board, size, move));
		if (quit) {
			std::cout << "Thanks for playing, " << name << "!\n";
			break;
		}
		if (ended) {
			std::cout << "Game Over! No more moves possible.\n";
			break;
		}
	}
	std::cout << BLUE << "hello world" << RESET << std::endl;
}

int main()
{
	/*int i;
	for (i = 0; i < 900000000; i++) {
		std::cout << i << "\n";
		int* p = new int(5);
	}
	std::cout << i;*/
	//std::ifstream ifs("./file.txt");
	//std::ofstream out("./pt.txt");
	//if (!ifs) {
	//	std::cout << "Proble";
	//}
	////if (!out) {
	//	out << "S";
	//	std::cout << "Problem";
	////}
	//	out.close();

	while (true)
	{
		printWelcomeMessage();
		std::cout << "[1] Start Game" << std::endl;
		std::cout << "[2] Leaderboard" << std::endl;
		std::cout << "[3] Exit" << std::endl;
		std::cout << "Enter your choice: ";
		int choice;
		std::cin >> choice;
		if (choice == 1) {
			clearConsole();
			printWelcomeMessage();
			StartGame();
		}
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
	std::cout << "Hello World!\n";


}