#include "Screen.h"
#include "ATM.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>

using namespace std;
using json = nlohmann::json;

void ATM::run() {
	//Load the database
	ATM::loadDatabase("database.json");
	ATM::atm_running_state = true;
}

void ATM::shutdownATM() {
	ATM::atm_running_state = false;
	exit(0);
}

bool ATM::isATMRunning() {
	return ATM::atm_running_state;
}

bool ATM::checkIfUserExists(string accountNumber) {
	vector<string> accountNumbers = ATM::authentication.getAllAccountNumbers();
	for (auto& accountNum : accountNumbers) {
		if (accountNum == accountNumber) {
			return true;
		}
	}
	return false;
}
bool ATM::authenticateUser(string accountNumber, string pin) {
	vector<string> accountNumbers = ATM::authentication.getAllAccountNumbers();
	vector<string> pins = ATM::authentication.getAllPins();
	for (string accNum : accountNumbers) {
		if (accNum == accountNumber) {
			for (string accPin : pins) {
				if (accPin == pin) {
					ATM::user_authentication_state = true;
					return true;
				}
			}
		}
	}
	return false;
}
bool ATM::registerUser(User user) {

	ifstream ifs("database.json");
	if (!ifs.is_open()) {
		ATM::screen.displayMessageLine("Failed to load the database. We'll solve the issue ASAP!");
	}
	json j;
	ifs >> j;

	//Check if user already exists in the bank.
	for (const auto& d : j["users"]) {
		if (d["identity_number"] == user.getIdentityNumber()) {
			return false;
		}
	}

	//Create a new user table
	json newUser = {
		{"account_number", user.getAccountNumber()},
		{"name", user.getName()},
		{"surname", user.getSurname()},
		{"age", user.getAge()},
		{"birth_place", user.getBirthPlace()},
		{"identity_number", user.getIdentityNumber()},
		{"pin", user.getPIN()},
		{"total_balance", user.getTotalBalance()}
	};

	//Push the user to database
	j["users"].push_back(newUser);
	ofstream ofs("database.json");
	ofs << j.dump(4);

	//Check if user is registered to database
	for (const auto& data : j["users"]) {
		if (user.getAccountNumber() == data["account_number"]) {
			return true;
		}
	}
	return false;
}

User ATM::getUser(string accountNumber) {
	string age, birthBlace, identity, name, pin, surname;
	long int totalBalance;

	ifstream ifs("database.json");
	if (!ifs.is_open()) {
		ATM::screen.displayMessageLine("Failed to load the database. We'll solve the issue ASAP!");
	}
	json j;
	ifs >> j;
	for (auto& d : j["users"]) {
		if (accountNumber == d["account_number"]) {
			age = d["age"];
			birthBlace = d["birth_place"];
			identity = d["identity_number"];
			name = d["name"];
			pin = d["pin"];
			surname = d["surname"];
			totalBalance = d["total_balance"];
		}
	}
	User user(accountNumber, name, surname, age, birthBlace, identity, pin, totalBalance);
	return user;
}

int ATM::getAvailableBalance() {
	return ATM::ATMBalance;
}

void ATM::increaseAvailableATMBalance(int amount) {
	int currBalance = ATM::ATMBalance;
	ATM::ATMBalance = currBalance + ATM::cashReceiver.receiveCash(amount);
	ATM::screen.displayMessage("ATM has ");
	ATM::screen.displayMessage((to_string)(ATM::ATMBalance));
	ATM::screen.displayMessageLine("$ loaded in it!");
}
void ATM::decreaseAvailableATMBalance(int amount) {
	int currBalance = ATM::ATMBalance;
	ATM::ATMBalance = currBalance - ATM::cashDispenser.dispenseCash(amount);
	ATM::screen.displayMessage("ATM has ");
	ATM::screen.displayMessage((to_string)(ATM::ATMBalance));
	ATM::screen.displayMessage("$ loaded in it!");
}

string ATM::generateAccountNumber() {
	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<int> distribution(10000, 99999);

	string accountNumber = (to_string)(distribution(generator)); //Account number generated. Let's check if it already exists in database.

	ifstream ifs("database.json");
	if (!ifs.is_open()) {
		ATM::screen.displayMessageLine("Failed to load the database. We'll solve the issue ASAP!");
	}
	json j;
	ifs >> j;
	while (true) {
		bool accountNumberExists = false;
		for (const auto& accNums : j["users"]) {
			if (accountNumber == accNums["account_number"]) {
				accountNumberExists = true;
				break;
			}
		}
		if (accountNumberExists) {
			accountNumber = to_string(distribution(generator)); // Generate a new account number
		}
		else {
			return accountNumber; // Return the account number if it doesn't exist in the database
		}
	}
}

void ATM::loadDatabase(string dbName) {
	ifstream ifs(dbName);
	if (!ifs.is_open()) {
		ATM::screen.displayMessageLine("Failed to load the database. We'll solve the issue ASAP!");
	}
	json j;
	ifs >> j;
	for (auto& user : j["users"]) {
		ATM::authentication.setAccountNumber(user["account_number"]);
		ATM::authentication.setPin(user["pin"]);
	}
}

void ATM::deauthenticateUser() {
	ATM::user_authentication_state = false;
}

bool ATM::isUserAuthenticated() {
	return ATM::user_authentication_state;
}

void ATM::takeMoneyFromUser(string accountNumber, long int amount) {
	ifstream ifs("database.json");
	if (!ifs.is_open()) {
		ATM::screen.displayMessageLine("Failed to load the database. We'll solve the issue ASAP!");
	}
	json j;
	ifs >> j;

	for (auto& d : j["users"]) {
		if (d["account_number"] == accountNumber) {
			d["total_balance"] = d["total_balance"] + amount;
		}
	}
	ofstream ofs("database.json");
	ofs << j.dump(4);
}
void ATM::giveMoneyToUser(string accountNumber, long int amount) {
	ifstream ifs("database.json");
	if (!ifs.is_open()) {
		ATM::screen.displayMessageLine("Failed to load the database. We'll solve the issue ASAP!");
	}
	json j;
	ifs >> j;
	for (auto& d : j["users"]) {
		if (d["account_number"] == accountNumber) {
			d["total_balance"] = d["total_balance"] - amount;
		}
	}
	ofstream ofs("database.json");
	ofs << j.dump(4);
}

bool ATM::transferMoney(string senderAccNum, string receiverAccNum, long int amount) {
	long int beforeTransfer = {};
	long int afterTransfer = {};
	ifstream ifs("database.json");
	if (!ifs.is_open()) {
		ATM::screen.displayMessageLine("Failed to load the database. We'll solve the issue ASAP!");
	}
	json j;
	ifs >> j;
	for (auto& d : j["users"]) {
		if (d["account_number"] == senderAccNum) {
			if (d["total_balance"] > amount) {
				d["total_balance"] = d["total_balance"] - amount;
			}
			else {
				ATM::screen.displayMessageLine("Your balance is not enough for this transfer! Please retry later when you deposit some funds!");
				return false;
			}
		}
	}
	for (auto& d : j["users"]) {
		if (d["account_number"] == receiverAccNum) {
			beforeTransfer = d["total_balance"];
			d["total_balance"] = d["total_balance"] + amount;
			afterTransfer = d["total_balance"];
		}
	}
	ofstream ofs("database.json");
	ofs << j.dump(4);

	if (afterTransfer > beforeTransfer) {
		return true;
	}
	else {
		return false;
	}

}