#include <iostream>
#include <list>
#include <limits>
#include <algorithm>
#include "Account.h"
#include "UserMenu.h"
#define KEY 'h'
#ifdef _WIN32
#define OS "windows"
#elif __unix__
#define OS "unix"
#endif

//permette di fare il login dell'utente immettendo username e password
std::string login() {
    std::string username;
    int password;
    bool ok = false;

    std::cout << "Welcome to Central Bank! Please log in" << std::endl;
    //immissione di username e password
    do {
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        //se viene inserito alla password un carattere diverso dai numeri, resetto il buffer
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        //cerco l'account
        for (const auto &account : accounts) {
            if (account.getName() == username && account.getPassword() == password) {
                ok = true;
                break;
            }
        }

        clearConsole();

        //se non lo trovo, ripropongo l'inserimento all'utente
        if (!ok) std::cout << "\033[31mWrong username and/or password! Please try again\033[0m" << std::endl;
    } while (!ok);

    return username;
}

int main() {
    std::string username;

    //carico gli account salvati in accounts.txt
    Account::loadAccounts(accounts);
    bool loggedIn;

    //carico le singole transazioni dei vari account salvati in [nomeAccount].txt
    for (auto& account : accounts) {
        if (account.getName()!="admin") account.loadTransactions();
    }

    do {
        clearConsole();
        username = login();

        //chiamo menu admin se entro con account admin
        if (username == "admin") loggedIn = adminMenu();
        //sennò entro nel menu cliente
        else loggedIn = menu(username);
    } while (loggedIn);

    clearConsole();
    //prompt finale saluto cliente
    std::cout << "\033[92mThank you for choosing Central Bank. Have a ";

    //prendo l'ora corrente...
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);

    int hour = local_time->tm_hour;

    //...e a seconda dell'ora, viene augurata una buona mattina, giornata, serata, notte
    if (hour >= 5 && hour < 11) {
        std::cout << "good morning!" << std::endl;
    } else if (hour >= 11 && hour < 17) {
        std::cout << "good day!" << std::endl;
    } else if (hour >= 17 && hour < 22) {
        std::cout << "good evening!" << std::endl;
    } else {
        std::cout << "good night!" << std::endl;
    }

    std::cout << "\033[0m";
    return 0;
}