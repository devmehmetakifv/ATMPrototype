#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
private:
	string account_number;
	string name;
	string surname;
	string age;
	string birth_place;
	string identity_number;
	string pin;
	long int total_balance;
public:
	User(string account_number, string name, string surname,
		string age, string birth_place, string identity_number
		, string pin, long int total_balance);
	string getAccountNumber();
	string getName();
	string getSurname();
	string getAge();
	string getBirthPlace();
	string getIdentityNumber();
	string getPIN();
	long int getTotalBalance();
};
#endif




