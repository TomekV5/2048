// 2048.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

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

const int MaxSize = 10;
const int MinSize = 4;
const int MaxNameSize = 100;

const short MAX_FILENAME_LEN = 34;
const char DELIMITER = ' ';
int BOARD_TILE_WIDTH = 6;
const size_t MAX_NICKNAMES_SCORES_COUNT = 5;

const char DIRECTORY[] = "./leaderboards/leaderboard";
const char TXT_EXTENSION[] = ".txt";


void clearInput() {
	std::cin.clear();
	std::cin.ignore(10000, '\n');
}

int safeReadInt(int min, int max) {
	int value;
	while (true) {
		std::cin >> value;

		if (!std::cin.fail() && value >= min && value <= max) {
			clearInput();
			return value;
		}

		std::cout << RED << "Invalid input! Try again: " << RESET;
		clearInput();
	}
}

int getpower(int number) {
	int power = 0;
	while (number > 1) {
		number /= 2;
		power++;
	}
	return power;
}
int CalculateScore(const int board[MaxSize][MaxSize], int size)
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
int digitCount(unsigned value) {
	int count = 1;
	while (value /= 10) {
		count++;
	}
	return count;
}
void printSpaces(int count) {
	while (count--) {
		std::cout << ' ';
	}
}
void printBoard(const int board[MaxSize][MaxSize], size_t size, const char* nickname) {
	//clearConsole();
	int score = CalculateScore(board, size);
	std::cout << nickname << "'s score: " << score << '\n';

	for (size_t row = 0; row < size; ++row) {
		for (size_t col = 0; col < size - 1; ++col) {
			unsigned tile = board[row][col];
			int digits = digitCount(tile);

			// when the default width is changed we board will
			if (digits >= BOARD_TILE_WIDTH) {
				BOARD_TILE_WIDTH += 2;
			}

			std::cout << colors[getpower(tile)] << tile << colors[0];
			printSpaces(BOARD_TILE_WIDTH - digits);
		}
		std::cout << colors[getpower(board[row][size - 1])] << board[row][size - 1] << colors[0] << std::endl << std::endl;
	}
}
int getMaxTile(const int board[MaxSize][MaxSize], int size)
{
	int maxTile = 0;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (board[i][j] > maxTile) maxTile = board[i][j];
		}
	}
	return maxTile;
}
int chooseNewTileValue(const int board[MaxSize][MaxSize], int size)
{
	int maxTile = getMaxTile(board, size);
	int maxPower = (maxTile > 1) ? getpower(maxTile) : 1;

	double p2 = 0.90;
	double p4 = 0.09;
	double p8 = 0.01;

	if (maxPower > 3) {
		int extraLevels = maxPower - 3;
		
		double inc4 = 0.03 * extraLevels;
		double inc8 = 0.01 * extraLevels;

		if (inc4 > 0.30) inc4 = 0.30;
		if (inc8 > 0.20) inc8 = 0.20;

		double dec2 = inc4 + inc8;
		p2 = p2 - dec2;
		if (p2 < 0.0) p2 = 0.0;

		p4 += inc4;
		p8 += inc8;
	}

	double sum = p2 + p4 + p8;
	if (sum <= 0.0) { p2 = 1.0; p4 = p8 = 0.0; sum = 1.0; }
	p2 /= sum; p4 /= sum; p8 /= sum;

	
	double r = (double)rand() / RAND_MAX;

	if (r < p2) return 2;
	if (r < p2 + p4) return 4;
	return 8;
}
void placeRandomTile(int board[MaxSize][MaxSize], int size)
{
	int emptyCount = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0)
				emptyCount++;
		}
	}

	int target = rand() % emptyCount;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				if (target == 0) {
					board[i][j] = board[i][j] = chooseNewTileValue(board, size);
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
	size = safeReadInt(MinSize, MaxSize);
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
void swap(unsigned& x, unsigned& y) {
	unsigned temp = x;
	x = y;
	y = temp;
}
void copy(const char* from, char* to) {
	if (from == nullptr || to == nullptr) {
		return;
	}
	while (*from != '\0') {
		*to = *from;
		++to;
		++from;
	}
	*to = '\0';
}
void swap(char* str1, char* str2) {
	char temp[MaxNameSize];
	copy(str1, temp);
	copy(str2, str1);
	copy(temp, str2);
}
void deallocateMatrix(char** matrix, size_t rows) {
	for (size_t row = 0; row < rows; ++row) {
		delete[] matrix[row];
	}
	delete[] matrix;
}
char toChar(size_t digit) {
	if (digit > 9) {
		return '0';
	}
	return (char)('0' + digit);
}
size_t strlen(const char* str) {
	if (str == nullptr) {
		return 0;
	}
	size_t len = 0;
	while (str[len++] != '\0');
	return --len;
}
void concat(char* str1, const char* str2) {
	if (str1 == nullptr || str2 == nullptr) {
		return;
	}
	size_t len = strlen(str1);
	while (*str2 != '\0') {
		str1[len++] = *str2;
		++str2;
	}
	str1[len] = '\0';
}
int myStrcmp(const char* a, const char* b)
{
	int i = 0;

	while (a[i] != '\0' && b[i] != '\0') {
		if (a[i] != b[i]) {
			return a[i] - b[i];
		}
		i++;
	}

	return a[i] - b[i];
}
void printLeaderboardMessage()
{
	std::cout << BOLDGREEN << "===========================================\n";
	std::cout << "	     WELCOME TO        \n";
	std::cout << "	   2048 Leaderboards!   \n";
	std::cout << "===========================================\n" << RESET;
}

char* getFilename(size_t dim) {
	char* filename = new char[MAX_FILENAME_LEN]();
	concat(filename, DIRECTORY);

	char strDim[2];
	strDim[0] = toChar(dim % 10);
	strDim[1] = '\0';

	concat(filename, strDim);
	concat(filename, TXT_EXTENSION);
	return filename;
}

void getNicknamesScores(size_t dim, char** nicknames, unsigned* scores, size_t& count) {
	char* filename = getFilename(dim);

	std::ifstream leaderboardFile(filename);

	if (!leaderboardFile.is_open()) {
		delete[] filename;
		return;
	}
	count = 0;

	while (leaderboardFile >> nicknames[count] >> scores[count]) {
		++count;
		if (count == MAX_NICKNAMES_SCORES_COUNT) {
			break;
		}
	}

	if (nicknames[0][0] == '\0') {
		count = 0;
	}

	leaderboardFile.close();
	delete[] filename;
}
void addNicknameScore(char* nickname, unsigned score, char** nicknames, unsigned* scores, size_t& count) {
	for (size_t i = 0; i < count; ++i) {
		if (myStrcmp(nickname, nicknames[i]) == 0) {
			if (score > scores[i]) {
				scores[i] = score;
				size_t idx = i;
				while (idx > 0 && scores[idx] > scores[idx - 1]) {
					swap(scores[idx], scores[idx - 1]);
					swap(nicknames[idx], nicknames[idx - 1]);
					--idx;
				}
			}
			return;
		}
	}

	if (count == 0) {
		copy(nickname, nicknames[count]);
		scores[count] = score;
		++count;
		return;
	}

	if (count == MAX_NICKNAMES_SCORES_COUNT && score <= scores[count - 1]) {
		return;
	}

	if (count == MAX_NICKNAMES_SCORES_COUNT) {
		copy(nickname, nicknames[count - 1]);
		scores[count - 1] = score;
	}
	else {
		copy(nickname, nicknames[count]);
		scores[count] = score;
		++count;
	}

	int idx = (int)count - 1;
	while (idx > 0 && scores[idx] > scores[idx - 1]) {
		swap(scores[idx], scores[idx - 1]);
		swap(nicknames[idx], nicknames[idx - 1]);
		--idx;
	}
}

char** allocateMatrix(size_t rows, size_t cols) {
	char** matrix = new char* [rows];
	for (size_t col = 0; col < rows; ++col) {
		matrix[col] = new char[cols];
	}
	return matrix;
}

void appendLeaderboard(size_t dim, char* nickname, unsigned score) {

	size_t count = 0;
	char** nicknames = allocateMatrix(MAX_NICKNAMES_SCORES_COUNT, MaxNameSize);
	unsigned* scores = new unsigned[MAX_NICKNAMES_SCORES_COUNT];

	getNicknamesScores(dim, nicknames, scores, count);

	addNicknameScore(nickname, score, nicknames, scores, count);

	char* filename = getFilename(dim);
	std::ofstream leaderboardFile(filename);

	if (!leaderboardFile.is_open()) {
		delete[] filename;
		delete[] scores;
		deallocateMatrix(nicknames, MAX_NICKNAMES_SCORES_COUNT);
		return;
	}

	for (size_t idx = 0; idx < count - 1; ++idx) {
		leaderboardFile << nicknames[idx] << DELIMITER << scores[idx] << DELIMITER;
	}
	leaderboardFile << nicknames[count - 1] << DELIMITER << scores[count - 1];
	leaderboardFile.close();

	delete[] filename;
	delete[] scores;
	deallocateMatrix(nicknames, MAX_NICKNAMES_SCORES_COUNT);
}

void printScores(const char** nicknames, const unsigned* scores, size_t count) {
	for (size_t idx = 0; idx < count; ++idx) {
		std::cout << idx + 1 << ". " << nicknames[idx] << "'s high score: " << scores[idx] << '\n';
	}
	std::cout << std::endl;
}

void showLeaderboard() {
	clearConsole();
	printLeaderboardMessage();
	std::cout << "Leaderboards: \n\n" << BOLDCYAN;
	for (int s = MinSize; s <= MaxSize; ++s) {
		std::cout << "[" << s << "X" << s << "] ";
	}
	std::cout << BOLDYELLOW << "\n\nEnter board size to view leaderboard: " << RESET;
	size_t dim = 0;
	dim = safeReadInt(4, 10);
	std::cout << std::endl;
	size_t count = 0;

	char** nicknames = allocateMatrix(MAX_NICKNAMES_SCORES_COUNT, MaxNameSize);
	unsigned* scores = new unsigned[MAX_NICKNAMES_SCORES_COUNT];

	getNicknamesScores(dim, nicknames, scores, count);

	if (count == 0) {
		std::cout << "Leaderboard is empty!\n";
	}
	else {
		printScores((const char**)nicknames, (const unsigned*)scores, count);
	}
	std::cout << std::endl;
	deallocateMatrix(nicknames, MAX_NICKNAMES_SCORES_COUNT);
	delete[] scores;
	std::cout << BOLDYELLOW << "What do you wish to do? \n" << RESET;
	std::cout << RED << "[1]" << CYAN << " Select another leaderboard\n";
	std::cout << RED << "[Another Key]" << CYAN << " Return to Main Menu\n";
	std::cout << "Enter your choice: " << RESET;
	char choi;
	while (!(std::cin >> choi)) {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}
	std::cin.ignore(10000, '\n');

	if (choi == '1') {
		showLeaderboard();
	}
	else {
		clearConsole();
	}	
}
void StartGame()
{
	int size = 0;
	char name[MaxNameSize] = {};
	PrepareGame(size, name);
	int board[MaxSize][MaxSize] = { 0 };
	int full = 0;
	bool quit = false;
	bool ended = false;
	while (true)
	{
		clearConsole();
		placeRandomTile(board, size);
		char move;
		int wrongMoves = 0;
		do {
			if (wrongMoves > 0)
			{
				clearConsole();
				std::cout << RED << "Invalid move! Try again.\n" << RESET;
			}
			printBoard(board, size, name);
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
			wrongMoves++;
		} while (!MoveTiles(board, size, move));
		if (quit) {
			std::cout << "Thanks for playing, " << name << "!\n";
			appendLeaderboard(size, name, CalculateScore(board, size));
			break;
		}
		if (ended) {
			std::cout << "Game Over! No more moves possible.\n";
			appendLeaderboard(size, name, CalculateScore(board, size));
			break;
		}
	}
}

int main()
{
	srand(time(nullptr));
	while (true)
	{
		printWelcomeMessage();
		std::cout << "[1] Start Game" << std::endl;
		std::cout << "[2] Leaderboard" << std::endl;
		std::cout << "[3] Exit" << std::endl;
		std::cout << "Enter your choice: ";
		char choice;
		while (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
		}
		std::cin.ignore(10000, '\n');

		if (choice == '1') {
			clearConsole();
			printWelcomeMessage();
			StartGame();
		}
		else if (choice == '2')
		{
			showLeaderboard();
		}
		else if (choice == '3')
		{
			return 0;
		}
		else
		{
			clearConsole();
			std::cout << RED << "Invalid input! Try again!\n " << RESET;
		}
	}
}