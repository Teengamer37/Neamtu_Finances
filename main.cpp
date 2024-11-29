#include <iostream>
#include <list>
#include <limits>
#include <algorithm>
#include "Account.h"
#define KEY 'h'

std::list<Account> accounts;

std::string login() {
    std::string username;
    int password;
    bool ok = false;

    std::cout << "Welcome to Central Bank! Please log in" << std::endl;
    do {
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        for (const auto &account : accounts) {
            if (account.getName() == username && account.getPassword() == password) {
                ok = true;
                break;
            }
        }

        system("cls");

        if (!ok) std::cout << "\033[31mWrong username and/or password! Please try again\033[0m" << std::endl;
    } while (!ok);

    return username;
}

void addAccount() {
    std::string newUsername;
    int newPassword;
    bool ok;
    system("cls");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    do {
        ok = true;
        std::cout << "Choose a username \033[31m(no spaces allowed)\033[0m: ";
        std::getline(std::cin, newUsername);

        if (std::count(newUsername.begin(), newUsername.end(), ' ')!=0) {
            system("cls");
            std::cout << "\033[31mUsername cannot have spaces! Try again\033[0m" << std::endl;
            ok = false;
        } else {
            for (const auto& account : accounts) {
                if (account.getName() == newUsername) {
                    ok = false;
                    system("cls");
                    std::cout << "\033[31mUsername already exists! Try again\033[0m" << std::endl;
                    break;
                }
            }
        }
    } while (!ok);

    do {
        ok = true;
        std::cout << "Choose a password \033[31m(numbers only)\033[0m: ";
        std::cin >> newPassword;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            std::cout << "\033[31mPassword must contain numbers only! Try again\033[0m" << std::endl;
            ok = false;
        }
    } while (!ok);

    accounts.push_back(new Account(newUsername, newPassword));
    Account::saveAccounts(accounts);

    system("cls");
    std::cout << "\033[32mNew account added! Username: " << newUsername << " - Password: " << newPassword << "\033[0m" << std::endl;
}

void removeAccount() {

}

void viewAccounts() {

}

void changeAccPw() {

}

void addAccTransaction() {

}

void viewTransactions() {

}

void changeAdminPw() {

}

bool adminMenu() {
    int selection;
    bool exit = false;

    system("cls");

    do {
        std::cout << "Welcome \033[93madmin\033[0m" << std::endl << std::endl;
        std::cout << "\033[36m----- Main menu -----\033[0m" << std::endl;
        std::cout << "\033[32m1 -\033[0m Add account" << std::endl;
        std::cout << "\033[32m2 -\033[0m Remove account" << std::endl;
        std::cout << "\033[32m3 -\033[0m View all accounts" << std::endl;
        std::cout << "\033[32m4 -\033[0m Change an account's password" << std::endl;
        std::cout << "\033[32m5 -\033[0m Add a transaction on an account" << std::endl;
        std::cout << "\033[32m6 -\033[0m View transaction history on one or all accounts" << std::endl;
        std::cout << "\033[32m7 -\033[0m Change your password" << std::endl;
        std::cout << "\033[32m8 -\033[0m Log out" << std::endl;
        std::cout << "\033[32m9 -\033[0m Log out and return to OS" << std::endl;
        std::cout << "Enter your selection: ";
        std::cin >> selection;

        switch (selection) {
            case 1:
                addAccount();
                break;
            case 2:
                removeAccount();
                break;
            case 3:
                viewAccounts();
                break;
            case 4:
                changeAccPw();
                break;
            case 5:
                addAccTransaction();
                break;
            case 6:
                viewTransactions();
                break;
            case 7:
                changeAdminPw();
                break;
            case 8:
                return true;
            case 9:
                return false;
            default:
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                system("cls");
                std::cout << "\033[31mYour selection is not a valid one. Please try again\033[0m" << std::endl << std::endl;
        }
    } while (true);
}

bool menu(std::string username) {

}

int main() {
    std::string username;

    Account::loadAccounts(accounts);
    bool loggedIn = true;

    for (auto& account : accounts) {
        if (account.getName()!="admin") account.loadTransactions(account.getName() + ".txt");
    }

    do {
        username = login();
        if (username == "admin") loggedIn = adminMenu();
        else loggedIn = menu(username);
    } while (loggedIn);












    std::cout << "Transactions:\n";
    for (auto& account : accounts) {
        for (const auto &transaction: account.getTransactions()) {
            std::cout << transaction.toString() << "\n";
        }

        std::cout << "Balance: " << account.getBalance() << "\n";

        account.saveTransactions(account.getName() + ".txt");
    }



    Account loadedAccount("BrianMay", 22);
    loadedAccount.loadTransactions("BrianMay.txt");

    std::cout << "Loaded Transactions:\n";
    for (const auto& transaction : loadedAccount.getTransactions()) {
        std::cout << transaction.toString() << "\n";
    }

    std::cout << "Loaded Balance: " << loadedAccount.getBalance() << "\n";

    return 0;
}