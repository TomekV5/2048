/**
*
* Solution to course project # 8
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Vasil Tomov
* @idnumber 3MI0600663
* @compiler VC
*
* <code file>
*
*/


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
#define ORANGE "\033[38;5;208m" /* ORANGE */
const char* colors[] = {
	RESET,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	ORANGE,
	BOLDBLACK,
	BOLDRED,
	BOLDGREEN,
	BOLDYELLOW,
	BOLDBLUE,
	BOLDMAGENTA,
	BOLDCYAN,
	BOLDWHITE
};

const int COLOR_COUNT = 17;

const int MinSize = 4;
const int MaxSize = 10;
const int MaxNameSize = 100;

const short MAX_FILENAME_LEN = 34;
const char DELIMITER = ' ';
int BOARD_TILE_WIDTH = 6;
const size_t MAX_NICKNAMES_SCORES_COUNT = 5;

const char DIRECTORY[] = "./leaderboards/leaderboard";
const char TXT_EXTENSION[] = ".txt";

//Helper functions
void clearConsole()
{
	system("cls");
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

void printBoard(const int board[MaxSize][MaxSize], size_t size, const char* nickname)
{
	int score = CalculateScore(board, size);
	std::cout << GREEN << nickname << RESET << "'s score: " << YELLOW << score << RESET << '\n';
	std::cout << ORANGE << "===========================================\n" << RESET;

	for (size_t row = 0; row < size; ++row)
	{
		for (size_t col = 0; col < size; ++col)
		{
			unsigned tile = board[row][col];
			int digits = digitCount(tile);

			if (digits >= BOARD_TILE_WIDTH) {
				BOARD_TILE_WIDTH += 2;
			}

			const char* color = RESET;

			if (tile != 0) {
				int index = getpower(tile) % COLOR_COUNT;
				color = colors[index];
			}

			std::cout << color << tile << RESET;
			printSpaces(BOARD_TILE_WIDTH - digits);
		}
		std::cout << "\n\n";
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

bool MoveTiles(int board[MaxSize][MaxSize], int size, char direction)
{
	int currentBoard[MaxSize][MaxSize];
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			currentBoard[row][col] = board[row][col];
		}
	}

	if (direction == 'w' || direction == 'W') // Up
	{
		for (int col = 0; col < size; col++)
		{
			unsigned mergedMask = 0;
			for (int row = 1; row < size; row++)
			{
				if (board[row][col] == 0) continue;

				while (row > 0 && board[row - 1][col] == 0)
				{
					board[row - 1][col] = board[row][col];
					board[row][col] = 0;
					row--;
				}

				if (row > 0 &&
					board[row - 1][col] == board[row][col] &&
					!(mergedMask & (1u << (row - 1))))
				{
					board[row - 1][col] *= 2;
					board[row][col] = 0;
					mergedMask |= (1u << (row - 1));
				}
			}
		}
	}
	else if (direction == 's' || direction == 'S') // Down
	{
		for (int col = 0; col < size; col++)
		{
			unsigned mergedMask = 0;
			for (int row = size - 2; row >= 0; row--)
			{
				if (board[row][col] == 0) continue;

				while (row < size - 1 && board[row + 1][col] == 0)
				{
					board[row + 1][col] = board[row][col];
					board[row][col] = 0;
					row++;
				}

				if (row < size - 1 &&
					board[row + 1][col] == board[row][col] &&
					!(mergedMask & (1u << (row + 1))))
				{
					board[row + 1][col] *= 2;
					board[row][col] = 0;
					mergedMask |= (1u << (row + 1));
				}
			}
		}
	}
	else if (direction == 'a' || direction == 'A') // Left
	{
		for (int row = 0; row < size; row++)
		{
			unsigned mergedMask = 0;
			for (int col = 1; col < size; col++)
			{
				if (board[row][col] == 0) continue;

				while (col > 0 && board[row][col - 1] == 0)
				{
					board[row][col - 1] = board[row][col];
					board[row][col] = 0;
					col--;
				}

				if (col > 0 &&
					board[row][col - 1] == board[row][col] &&
					!(mergedMask & (1u << (col - 1))))
				{
					board[row][col - 1] *= 2;
					board[row][col] = 0;
					mergedMask |= (1u << (col - 1));
				}
			}
		}
	}
	else if (direction == 'd' || direction == 'D') // Right
	{
		for (int row = 0; row < size; row++)
		{
			unsigned mergedMask = 0;
			for (int col = size - 2; col >= 0; col--)
			{
				if (board[row][col] == 0) continue;

				while (col < size - 1 && board[row][col + 1] == 0)
				{
					board[row][col + 1] = board[row][col];
					board[row][col] = 0;
					col++;
				}

				if (col < size - 1 &&
					board[row][col + 1] == board[row][col] &&
					!(mergedMask & (1u << (col + 1))))
				{
					board[row][col + 1] *= 2;
					board[row][col] = 0;
					mergedMask |= (1u << (col + 1));
				}
			}
		}
	}

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (currentBoard[i][j] != board[i][j])
				return true;

	return false;
}

void PrepareGame(int& size, char* name)
{
	std::cout << BOLDYELLOW << "Enter your name " << RED << "(MAX 100 Characters): " << RESET;
	std::cin >> name;
	std::cout << BOLDYELLOW << "Enter board size " << RED << "(4-10): " << RESET;
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

//Messages
void printWelcomeMessage()
{
	std::cout << ORANGE << "===========================================\n";
	std::cout << "	     WELCOME TO        \n";
	std::cout << "		2048!   \n";
	std::cout << "===========================================\n" << RESET;
}
void printLeaderboardMessage()
{
	std::cout << BOLDGREEN << "===========================================\n";
	std::cout << "	     WELCOME TO        \n";
	std::cout << "	   2048 Leaderboards!   \n";
	std::cout << "===========================================\n" << RESET;
}
void printGoodbyeMessage() {
	std::cout << BOLDGREEN << "===========================================\n";
	std::cout << "	THANK YOU FOR PLAYING 2048!        \n";
	std::cout << "===========================================\n" << RESET;
}
//Leaderboard functions
char* getFilename(size_t dim) {
	char* filename = new char[MAX_FILENAME_LEN]();
	concat(filename, DIRECTORY);

	char strDim[4];
	strDim[0] = toChar(dim % 10);
	strDim[1] = 'X';
	strDim[2] = toChar(dim % 10);
	strDim[3] = '\0';

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
		std::cout << MAGENTA << "Leaderboard is empty!\n" << RESET;
	}
	else {
		printScores((const char**)nicknames, (const unsigned*)scores, count);
	}
	deallocateMatrix(nicknames, MAX_NICKNAMES_SCORES_COUNT);
	delete[] scores;
	std::cout << BOLDYELLOW << "What do you wish to do? \n" << RESET;
	std::cout << RED << "[1]" << CYAN << " Select another leaderboard\n";
	std::cout << RED << "[Another Key]" << CYAN << " Return to Main Menu\n";
	std::cout << BOLDYELLOW << "Enter your choice: " << RESET;
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

//Game loop
void StartGame()
{
	clearConsole();
	printWelcomeMessage();
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
			while (!(std::cin >> move)) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			std::cin.ignore(10000, '\n');
			if (move == 'Q' || move == 'q') {
				quit = true;
				break;
			}
			wrongMoves++;
		} while (!MoveTiles(board, size, move));
		if (quit) {
			std::cout << CYAN << "Thanks for playing, " << GREEN << name << CYAN << "!\n" << RESET;
			appendLeaderboard(size, name, CalculateScore(board, size));
			break;
		}
		if (ended) {
			std::cout << BOLDMAGENTA << "Game Over! No more moves possible.\n" << RESET;
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
		std::cout << RED << "[1]" << CYAN << " Start Game" << std::endl;
		std::cout << RED << "[2]" << CYAN << " Leaderboard" << std::endl;
		std::cout << RED << "[3]" << CYAN << " Exit" << std::endl;
		std::cout << BOLDYELLOW << "Enter your choice: " << RESET;
		char choice;
		while (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
		}
		std::cin.ignore(10000, '\n');

		if (choice == '1')
		{
			StartGame();
		}
		else if (choice == '2')
		{
			showLeaderboard();
		}
		else if (choice == '3')
		{
			break;
		}
		else
		{
			clearConsole();
			std::cout << RED << "Invalid input! Try again!\n " << RESET;
		}
	}
	printGoodbyeMessage();
}