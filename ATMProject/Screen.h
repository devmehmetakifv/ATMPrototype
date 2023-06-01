#ifndef SCREEN_H
#define SCREEN_H
#include <string>
using namespace std;

class Screen {
public:
	void displayMessage(string);
	void displayMessageLine(string);
	string getInput();
	void cleanScreen();
	void writeATM();
};
#endif


