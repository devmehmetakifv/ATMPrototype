#include <string>
#include <iostream>
#include "User.h"
using namespace std;

#include "User.h"

User::User(string account_number, string name, string surname, string age, string birth_place, string identity_number, string pin, long int total_balance)
    : account_number(account_number),
    name(name),
    surname(surname),
    age(age),
    birth_place(birth_place),
    identity_number(identity_number),
    pin(pin),
    total_balance(total_balance)
{
}
string User::getAccountNumber() {
    return account_number;
}
string User::getAge() {
    return age;
}
string User::getBirthPlace() {
    return birth_place;
}
string User::getIdentityNumber() {
    return identity_number;
}
string User::getName() {
    return name;
}
string User::getPIN() {
    return pin;
}
string User::getSurname() {
    return surname;
}
long int User::getTotalBalance() {
    return total_balance;
}