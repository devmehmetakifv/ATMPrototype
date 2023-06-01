#include "Screen.h"
#include "ATM.h"
#include "User.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <conio.h>
#include <Windows.h>
#include <cctype>

#define ACC_NUMBER_LENGTH 5
#define MIN_AGE_TO_CREATE_ACC 18

using namespace std;

//Simple function to pause the program between menus.
void wait(int seconds) {
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int main()
{
    ATM atm;
    atm.run();
	while (atm.isATMRunning()) {
		system("Color 0A");
		atm.screen.writeATM();
		atm.screen.displayMessageLine("");
		atm.screen.displayMessageLine("Welcome to the ATM!");
		atm.screen.displayMessageLine("Please enter your account number: ");
		string accountNumber = atm.screen.getInput();
		//Check if the entered accountNumber is a valid integer.
		for (char& c : accountNumber) {
			if (!isdigit(c)) {
				atm.screen.displayMessageLine("You should've entered an integer value! Why are you like this??! Terminating the session!");
				atm.shutdownATM();
			}
		}
		//Let's check if the account number is 5-digit and it exists in database.
		if (atm.checkIfUserExists(accountNumber) && accountNumber.length() == ACC_NUMBER_LENGTH) {
			atm.screen.displayMessageLine("Please enter your PIN: ");
			string pin = atm.screen.getInput();
			//Check if the pin associated with the account number is true. Log the user in if it is. Otherwise, terminate the session.
			if (atm.authenticateUser(accountNumber, pin)) {
				atm.screen.cleanScreen();
				atm.screen.displayMessageLine("You are successfully logged in!");
				atm.screen.displayMessageLine("Let's take you to main menu...");
				wait(4);
				//Loop the rest of the program based on the authentication state of the current user.
				while (atm.isUserAuthenticated()) {
					atm.screen.cleanScreen();
					//Each time this menu loads, the newest version of user data will be pulled from the database and associated with a User object. This way, I can use this object to display their name, total balance etc.
					User user = atm.getUser(accountNumber);
					atm.screen.displayMessageLine("-----------------------------------------------------------------------------------------------------------------");
					atm.screen.displayMessageLine("(1) View account balance");
					atm.screen.displayMessageLine("(2) Withdraw cash");
					atm.screen.displayMessageLine("(3) Deposit funds");
					atm.screen.displayMessageLine("(4) Transfer money");
					atm.screen.displayMessageLine("-----------------------------------------------------------------------------------------------------------------");
					atm.screen.displayMessage("Welcome, ");
					atm.screen.displayMessage(user.getName());
					atm.screen.displayMessage(" ");
					atm.screen.displayMessage(user.getSurname());
					atm.screen.displayMessageLine(" . Please enter the number corresponding to the process you want to do: ");
					string inp = atm.screen.getInput();
					switch (stoi(inp)) {
					case 1: {
						bool isViewingBalance = true;
						while (isViewingBalance) {
							atm.screen.cleanScreen();
							atm.screen.displayMessage("Your total balance is: ");
							//Let's use the data we pulled from database and associated with the user object.
							atm.screen.displayMessage((to_string)(user.getTotalBalance()));
							atm.screen.displayMessageLine("$");
							atm.screen.displayMessageLine("-------------------------");
							atm.screen.displayMessageLine("(1) Back to main menu");
							atm.screen.displayMessageLine("-------------------------");
							atm.screen.displayMessageLine("Waiting for input: ");
							string input = atm.screen.getInput();
							if (input == "1") {
								isViewingBalance = false;
							}
							else {
								atm.screen.displayMessageLine("That's an unknown option. Terminating the session!");
								isViewingBalance = false;
								atm.shutdownATM();
								
							}
						}
					}
						break;
					case 2: {
						bool isWithdrawingCash = true;
						while (isWithdrawingCash) {
							atm.screen.cleanScreen();
							atm.screen.displayMessageLine("-----------------------------------------------");
							atm.screen.displayMessage("(1) 25$ ----- ");
							atm.screen.displayMessageLine(" (4) 200$");
							atm.screen.displayMessage("(2) 50$ ----- ");
							atm.screen.displayMessageLine(" (5) 500$");
							atm.screen.displayMessage("(3) 100$ ----- ");
							atm.screen.displayMessageLine("(6) 1000$");
							atm.screen.displayMessageLine("-----------------------------------------------");
							atm.screen.displayMessageLine("Enter the amount of money you want to withdraw:");
							string inp = atm.screen.getInput();
							switch (stoi(inp)) {
							case 1: {
								//Check if the ATM has available balance to make this request.
								if (atm.getAvailableBalance() < 25) {
									atm.screen.displayMessageLine("ATM balance isn't enough for this withdraw! Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//And of course, I should check if the user has available balance to withdraw the amount. We don't want to lose money, eh?
								else if (user.getTotalBalance() < 25) {
									atm.screen.displayMessageLine("You don't have enough money for this process. Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//Cash dispenser dispense the cash andd...
								int amountToGive = atm.cashDispenser.dispenseCash(25);
								//... give money to the user. We also should decrease the amount from their balance on database. This function also does it : ]
								atm.giveMoneyToUser(user.getAccountNumber(), amountToGive);
								// And let's decrease the ATM's available balance.
								atm.decreaseAvailableATMBalance(amountToGive);
								atm.screen.displayMessageLine("You have successfully withdraw the money. We'll take you back in 5 seconds...");
								wait(5);
								isWithdrawingCash = false;
							}
								  break;
							case 2: {
								//Check if the ATM has available balance to make this request.
								if (atm.getAvailableBalance() < 50) {
									atm.screen.displayMessageLine("ATM balance isn't enough for this withdraw! Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//And of course, I should check if the user has available balance to withdraw the amount. We don't want to lose money, eh?
								else if (user.getTotalBalance() < 50) {
									atm.screen.displayMessageLine("You don't have enough money for this process. Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//Cash dispenser dispense the cash andd...
								int amountToGive = atm.cashDispenser.dispenseCash(50);
								//... give money to the user. We also should decrease the amount from their balance on database. This function also does it : ]
								atm.giveMoneyToUser(user.getAccountNumber(), amountToGive);
								// And let's decrease the ATM's available balance.
								atm.decreaseAvailableATMBalance(amountToGive);
								atm.screen.displayMessageLine("You have successfully withdraw the money. We'll take you back in 5 seconds...");
								wait(5);
								isWithdrawingCash = false;
							}
								  break;
							case 3: {
								//Check if the ATM has available balance to make this request.
								if (atm.getAvailableBalance() < 100) {
									atm.screen.displayMessageLine("ATM balance isn't enough for this withdraw! Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//And of course, I should check if the user has available balance to withdraw the amount. We don't want to lose money, eh?
								else if (user.getTotalBalance() < 100) {
									atm.screen.displayMessageLine("You don't have enough money for this process. Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//Cash dispenser dispense the cash andd...
								int amountToGive = atm.cashDispenser.dispenseCash(100);
								//... give money to the user. We also should decrease the amount from their balance on database. This function also does it : ]
								atm.giveMoneyToUser(user.getAccountNumber(), amountToGive);
								// And let's decrease the ATM's available balance.
								atm.decreaseAvailableATMBalance(amountToGive);
								atm.screen.displayMessageLine("You have successfully withdraw the money. We'll take you back in 5 seconds...");
								wait(5);
								isWithdrawingCash = false;
							}
								  break;
							case 4: {
								//Check if the ATM has available balance to make this request.
								if (atm.getAvailableBalance() < 200) {
									atm.screen.displayMessageLine("ATM balance isn't enough for this withdraw! Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//And of course, I should check if the user has available balance to withdraw the amount. We don't want to lose money, eh?
								else if (user.getTotalBalance() < 200) {
									atm.screen.displayMessageLine("You don't have enough money for this process. Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//Cash dispenser dispense the cash andd...
								int amountToGive = atm.cashDispenser.dispenseCash(200);
								//... give money to the user. We also should decrease the amount from their balance on database. This function also does it : ]
								atm.giveMoneyToUser(user.getAccountNumber(), amountToGive);
								// And let's decrease the ATM's available balance.
								atm.decreaseAvailableATMBalance(amountToGive);
								atm.screen.displayMessageLine("You have successfully withdraw the money. We'll take you back in 5 seconds...");
								wait(5);
								isWithdrawingCash = false;
							}
								  break;
							case 5: {
								//Check if the ATM has available balance to make this request.
								if (atm.getAvailableBalance() < 500) {
									atm.screen.displayMessageLine("ATM balance isn't enough for this withdraw! Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//And of course, I should check if the user has available balance to withdraw the amount. We don't want to lose money, eh?
								else if (user.getTotalBalance() < 500) {
									atm.screen.displayMessageLine("You don't have enough money for this process. Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//Cash dispenser dispense the cash andd...
								int amountToGive = atm.cashDispenser.dispenseCash(500);
								//... give money to the user. We also should decrease the amount from their balance on database. This function also does it : ]
								atm.giveMoneyToUser(user.getAccountNumber(), amountToGive);
								// And let's decrease the ATM's available balance.
								atm.decreaseAvailableATMBalance(amountToGive);
								atm.screen.displayMessageLine("You have successfully withdraw the money. We'll take you back in 5 seconds...");
								wait(5);
								isWithdrawingCash = false;
							}
								  break;
							case 6: {
								//Check if the ATM has available balance to make this request.
								if (atm.getAvailableBalance() < 1000) {
									atm.screen.displayMessageLine("ATM balance isn't enough for this withdraw! Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//And of course, I should check if the user has available balance to withdraw the amount. We don't want to lose money, eh?
								else if (user.getTotalBalance() < 1000) {
									atm.screen.displayMessageLine("You don't have enough money for this process. Taking you back to main menu in 5 seconds...");
									isWithdrawingCash = false;
									wait(5);
									break;
								}
								//Cash dispenser dispense the cash andd...
								int amountToGive = atm.cashDispenser.dispenseCash(1000);
								//... give money to the user. We also should decrease the amount from their balance on database. This function also does it : ]
								atm.giveMoneyToUser(user.getAccountNumber(), amountToGive);
								// And let's decrease the ATM's available balance.
								atm.decreaseAvailableATMBalance(amountToGive);
								atm.screen.displayMessageLine("You have successfully withdraw the money. We'll take you back in 5 seconds...");
								wait(5);
								isWithdrawingCash = false;
							}
								  break;
							}
						}

					}
						break;
					case 3: {
						bool isDepositingCash = true;
						while (isDepositingCash) {
							atm.screen.cleanScreen();
							atm.screen.displayMessageLine("-----------------------------------------------");
							atm.screen.displayMessage("(1) 25$ ----- ");
							atm.screen.displayMessageLine(" (4) 200$");
							atm.screen.displayMessage("(2) 50$ ----- ");
							atm.screen.displayMessageLine(" (5) 500$");
							atm.screen.displayMessage("(3) 100$ ----- ");
							atm.screen.displayMessageLine("(6) 1000$");
							atm.screen.displayMessageLine("-----------------------------------------------");
							atm.screen.displayMessageLine("Enter the amount of money you want to deposit:");
							string inp = atm.screen.getInput();
							switch (stoi(inp)) {
							case 1: {
								//Cash receiver receives the money from user...
								int amountToGet = atm.cashReceiver.receiveCash(25);
								//... and load it to the user's balance by updating the total_balance data in the database.
								atm.takeMoneyFromUser(user.getAccountNumber(), amountToGet);
								//We also should increase the available ATM balance since we received money from an out source.
								atm.increaseAvailableATMBalance(amountToGet);
								atm.screen.displayMessageLine("You have successfully deposited the money. We'll take you back in 5 seconds...");
								wait(5);
								isDepositingCash = false;
							}
								  break;
							case 2: {
								//Cash receiver receives the money from user...
								int amountToGet = atm.cashReceiver.receiveCash(50);
								//... and load it to the user's balance by updating the total_balance data in the database.
								atm.takeMoneyFromUser(user.getAccountNumber(), amountToGet);
								//We also should increase the available ATM balance since we received money from an out source.
								atm.increaseAvailableATMBalance(amountToGet);
								atm.screen.displayMessageLine("You have successfully deposited the money. We'll take you back in 5 seconds...");
								wait(5);
								isDepositingCash = false;
							}
								  break;
							case 3: {
								//Cash receiver receives the money from user...
								int amountToGet = atm.cashReceiver.receiveCash(100);
								//... and load it to the user's balance by updating the total_balance data in the database.
								atm.takeMoneyFromUser(user.getAccountNumber(), amountToGet);
								//We also should increase the available ATM balance since we received money from an out source.
								atm.increaseAvailableATMBalance(amountToGet);
								atm.screen.displayMessageLine("You have successfully deposited the money. We'll take you back in 5 seconds...");
								wait(5);
								isDepositingCash = false;
							}
								  break;
							case 4: {
								//Cash receiver receives the money from user...
								int amountToGet = atm.cashReceiver.receiveCash(200);
								//... and load it to the user's balance by updating the total_balance data in the database.
								atm.takeMoneyFromUser(user.getAccountNumber(), amountToGet);
								//We also should increase the available ATM balance since we received money from an out source.
								atm.increaseAvailableATMBalance(amountToGet);
								atm.screen.displayMessageLine("You have successfully deposited the money. We'll take you back in 5 seconds...");
								wait(5);
								isDepositingCash = false;
							}
								  break;
							case 5: {
								//Cash receiver receives the money from user...
								int amountToGet = atm.cashReceiver.receiveCash(500);
								//... and load it to the user's balance by updating the total_balance data in the database.
								atm.takeMoneyFromUser(user.getAccountNumber(), amountToGet);
								//We also should increase the available ATM balance since we received money from an out source.
								atm.increaseAvailableATMBalance(amountToGet);
								atm.screen.displayMessageLine("You have successfully deposited the money. We'll take you back in 5 seconds...");
								wait(5);
								isDepositingCash = false;
							}
								  break;
							case 6: {
								//Cash receiver receives the money from user...
								int amountToGet = atm.cashReceiver.receiveCash(1000);
								//... and load it to the user's balance by updating the total_balance data in the database.
								atm.takeMoneyFromUser(user.getAccountNumber(), amountToGet);
								//We also should increase the available ATM balance since we received money from an out source.
								atm.increaseAvailableATMBalance(amountToGet);
								atm.screen.displayMessageLine("You have successfully deposited the money. We'll take you back in 5 seconds...");
								wait(5);
								isDepositingCash = false;
							}
								  break;
							}
						}

					}
						  break;
					case 4: {
						bool isTransferringMoney = true;
						while (isTransferringMoney) {
							atm.screen.displayMessageLine("Enter the account number you want to transfer money to: ");
							string receiverAccNum = atm.screen.getInput();
							atm.screen.displayMessageLine("Enter the amount you want to transfer to this account: ");
							string amount = atm.screen.getInput();
							for (char c : amount) {
								if (!isdigit(c)) {
									atm.screen.displayMessageLine("Wrong format! Terminating the session!");
									atm.shutdownATM();
								}
							}
							bool isSuccess = atm.transferMoney(user.getAccountNumber(), receiverAccNum, stoi(amount));
							if (isSuccess) {
								atm.screen.displayMessage("You have successfully sent money to account: ");
								atm.screen.displayMessageLine(receiverAccNum);
								atm.screen.displayMessageLine("Taking you back to main menu in 5 seconds...");
								wait(5);
								isTransferringMoney = false;
							}
							else {
								atm.screen.displayMessage("There was an error sending the money to the account: ");
								atm.screen.displayMessageLine(receiverAccNum);
								atm.screen.displayMessageLine("Please check the account number and try again...");
								wait(5);
								isTransferringMoney = false;
							}
						}
					}
						  break;
					default: {
						atm.screen.displayMessageLine("You entered an unknown option. Terminating the session!");
						atm.deauthenticateUser();
						atm.shutdownATM();
						}
					}
				}
			}
			else {
				atm.screen.displayMessageLine("Your PIN is incorrect! Terminating the session...");
				atm.shutdownATM();
			}
		}
		else {
			//Here, if the account number is not in the correct format or it doesn't exist in the database, we ask user if they are registered or not. If they are not, they can register from this panel.
			atm.screen.cleanScreen();
			atm.screen.displayMessageLine("We couldn't find this account number in the database. Are you sure you entered correct account number?");
			atm.screen.displayMessageLine("-----------------------------------------------------------------------------------------------------------------");
			atm.screen.displayMessageLine("(1) Register new account");
			atm.screen.displayMessageLine("(2) Try again");
			atm.screen.displayMessageLine("-----------------------------------------------------------------------------------------------------------------");
			string choice = atm.screen.getInput();
			//Let the registration process begin. Woooooooooo!
			if (choice == "1") {
				atm.screen.cleanScreen();
				//We take the neccessary information from the user. We also take the identity number and check if it already exists in database, to prevent a user from creating multiple accounts.
				//Well, of course they can enter a random identity number right now but come on, this is just a prototype hehe. 
				atm.screen.displayMessageLine("Welcome! Thanks for choosing us.");
				atm.screen.displayMessage("What's your name? (e.x John) : ");
				string name = atm.screen.getInput();
				atm.screen.displayMessage("What's your surname? (e.x Dalton) : ");
				string surname = atm.screen.getInput();
				atm.screen.displayMessage("How old are you? : ");
				string age = atm.screen.getInput();
				//Age must be equal to or over 18!!!
				if (stoi(age) >= MIN_AGE_TO_CREATE_ACC) {
					atm.screen.displayMessage("Where did you born? : ");
					string birth_place = atm.screen.getInput();
					atm.screen.displayMessage("Enter your identity number : ");
					string identity_number = atm.screen.getInput();
					atm.screen.displayMessage("Choose a 5-digit security PIN :");
					string pin = atm.screen.getInput();
					if (pin.length() == 5) {
						//generateAccountNumber() function generates a random 5-digit number and checks if it already exists in database. If it is, then generate another one. If it isn't, then we are good to go!
						string generatedAccountNumber = atm.generateAccountNumber();
						//We create a User object using the constructor to assign the values we got to the class.
						User user(generatedAccountNumber, name, surname, age, birth_place, identity_number, pin, 0);
						//Let's register the user using the registerUser() function. This function directly interacts with the database (in our case, our lovely and poor JSON file) and inserts the new user data as a JSON column.
						bool isUserCreated = atm.registerUser(user);
						//The function also returns a boolean value. This way we can check if the user is actually registered or some sort of error threw during the process.
						if (isUserCreated) {
							atm.screen.displayMessageLine("-----------------------------------------------");
							atm.screen.displayMessageLine("Your account has been successfully created!");
							atm.screen.displayMessageLine("-----------------------------------------------");
							atm.screen.displayMessageLine("Your account number is: ");
							atm.screen.displayMessageLine(generatedAccountNumber);
							atm.screen.displayMessageLine("Your PIN is: ");
							atm.screen.displayMessageLine(pin);
							atm.screen.displayMessageLine("-----------------------------------------------");
							atm.screen.displayMessageLine("Please close this program and login using your credentials. Thanks for choosing us!");
							getchar();
							exit(0);
							//Give the user their generated account number and the PIN they chose. Thanks them and sign off!
						}
						else {
							atm.screen.displayMessageLine("There was a problem creating your account. Are you sure you aren't registered? If so, please try again.");
							atm.shutdownATM();
						}
					}
					else {
						atm.screen.displayMessageLine("You should've entered 5 digits! Terminating the session!");
						atm.shutdownATM();
					}
				}
				else {
					atm.screen.displayMessageLine("You have to be over 18 years old to register to this bank! Sorry!");
					atm.shutdownATM();
				}

			}
			else if (choice == "2") {
				atm.screen.displayMessageLine("Thanks for using our ATM. Take care!");
				atm.shutdownATM();
			}
			else {
				atm.screen.displayMessageLine("Incorrect choice. Terminating the session.");
				atm.shutdownATM();
			}
		}
	}
}