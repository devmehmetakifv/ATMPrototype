#include "Screen.h"
#include <iostream>
using namespace std;

void Screen::displayMessage(string msg) {
	cout << msg;
}
void Screen::displayMessageLine(string msg) {
	cout << msg << endl;
}
string Screen::getInput() {
	string inp;
	cin >> inp;
	return inp;
}
void Screen::cleanScreen() {
	system("cls");
}
void Screen::writeATM() {
	cout << "  _____________________________________  " << endl;
	cout << " /______________________________________\\" << endl;
	cout << "||                                      ||" << endl;
	cout << "||      /\\    ___________   ___    ___  ||" << endl;
	cout << "||     /  \\  |____    ___| |   \\  /   | ||" << endl;
	cout << "||    //_\\ \\      |  |     | |\\ \\/ /| | ||" << endl;
	cout << "||   /      \\     |  |     | | \\  / | | ||" << endl;
	cout << "||  /   /\\   \\    |  |     | |  \\/  | | ||" << endl;
	cout << "|| /___/  \\___\\   |__|     |_|      |_| ||" << endl;
	cout << "||______________________________________||" << endl;
	cout << " \\______________________________________/  " << endl;
}