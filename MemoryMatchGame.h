#include <string>

class MemoryMatchGame {
public:
	MemoryMatchGame();
	void play();

private:
	int startTime;
	int size;
	int numWords;
	int sleepMS;
	std::string* answer;
	std::string* display;

	void initGame();
	int initLevel();
	int initSpeed();
	int initTheme();
	void initAnswer(int theme);
	void drawGrid();
	char getCharacter(int i, int j);
	int findIndex(int i, int j);
	int guessNumber();
};