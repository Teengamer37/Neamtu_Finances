#include <iostream>
#include <list>
#include <limits>
#include <algorithm>
#include "Account.h"
#include "AdminMenu.h"
#define KEY 'h'
#ifdef _WIN32
#define OS "windows"
#elif __unix__
#define OS "unix"
#endif

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

        clearConsole();

        if (!ok) std::cout << "\033[31mWrong username and/or password! Please try again\033[0m" << std::endl;
    } while (!ok);

    return username;
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