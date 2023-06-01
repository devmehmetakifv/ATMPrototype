#ifndef AUTHENTICATION_H
#define	AUTHENTICATION_H

#include <vector>
#include <string>
using namespace std;


class Authentication {
private:
	vector<string> accountNumbers;
	vector<string> pins;
public:
	vector<string> getAllAccountNumbers();
	vector<string> getAllPins();
	void setAccountNumber(string number);
	void setPin(string pin);
};
#endif

