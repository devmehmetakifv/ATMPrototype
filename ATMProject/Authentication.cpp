#include <iostream>
#include <vector>
#include "Authentication.h"

vector<string> Authentication::getAllAccountNumbers() {
	return Authentication::accountNumbers;
}
vector<string> Authentication::getAllPins() {
	return Authentication::pins;
}
void Authentication::setAccountNumber(string number) {
	Authentication::accountNumbers.push_back(number);
}
void Authentication::setPin(string pin) {
	Authentication::pins.push_back(pin);
}