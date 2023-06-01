#ifndef ATM_H
#define ATM_H
#include "Screen.h"
#include "Authentication.h"
#include "User.h"
#include "CashReceiver.h"
#include "CashDispenser.h"


class ATM {
private:
	bool user_authentication_state = false;
	int ATMBalance = 10000;
	bool atm_running_state = false;
public:
	//Main parts of the ATM machine.
	Screen screen;
	Authentication authentication;
	CashReceiver cashReceiver;
	CashDispenser cashDispenser;

	//Function members that states the main ATM status.
	void run();
	void shutdownATM();
	bool isATMRunning();

	//Function members about the user actions.
	bool checkIfUserExists(string);
	bool authenticateUser(string, string);
	void deauthenticateUser();
	bool isUserAuthenticated();
	User getUser(string accountNumber);
	void giveMoneyToUser(string accountNumber, long int amount);
	void takeMoneyFromUser(string accountNumber, long int amount);
	bool registerUser(User);
	bool transferMoney(string senderAccNum, string receiverAccNum, long int amount);
	string generateAccountNumber();

	//Function members about the balance available in ATM machine.
	int getAvailableBalance();
	void decreaseAvailableATMBalance(int amount);
	void increaseAvailableATMBalance(int amount);

	void loadDatabase(string);
};

#endif