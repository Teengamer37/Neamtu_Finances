#ifndef NEAMTU_FINANCES_USERMENU_H
#define NEAMTU_FINANCES_USERMENU_H
#include "AdminMenu.h"
#include <memory>

Account* acc;

void makeDeposit() {
    std::string description;
    double deposit;
    bool ok = false;

    clearConsole();

    do {
        do {
            std::cout << "Insert the amount to deposit: ";
            std::cin >> deposit;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string abort;
                clearConsole();
                std::cout
                        << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else if (deposit <= 0) {
                std::string abort;
                clearConsole();
                std::cout
                        << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else {
                ok = true;
            }
        } while (!ok);

        ok = false;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please add a description: ";
        std::getline(std::cin, description);

        acc->addTransaction(Transaction(Transaction::DEPOSIT, deposit, description));
        acc->saveTransactions(acc->getName() + ".txt");

        std::cout << std::endl << "Deposit added!" << std::endl
                  << "\033[32mDeposit\033[0m - " << deposit << " \033[93mEuro\033[0m - \033[94m" << description
                  << "\033[0m" << std::endl;

        std::string c;
        std::cout << "Would you like to add another deposit? (type 'y' to continue) ";
        std::cin >> c;
        if (!caseInsensitiveCompare(c, "y")) ok = true;
        clearConsole();
    } while (!ok);
}

void makeWithdrawal() {
    std::string description, selection;
    double withdrawal;
    bool ok = false;

    clearConsole();

    do {
        if (acc->getBalance() <= 0) {
            std::cout << "\033[31mYour account balance is in red! You cannot make withdrawals!\033[0m" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Press Enter to continue... ";
            std::getline(std::cin, selection);
            clearConsole();
            return;
        }

        do {
            std::cout << "Insert the amount to withdraw: ";
            std::cin >> withdrawal;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string abort;
                clearConsole();
                std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else if (withdrawal <= 0) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else if (acc->getBalance()-withdrawal < 0) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mInsufficient funds! Your current balance: " << acc->getBalance() << " Euro\033[0m"
                          << std::endl << "Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else {
                ok = true;
            }
        } while (!ok);

        ok = false;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please add a description: ";
        std::getline(std::cin, description);

        acc->addTransaction(Transaction(Transaction::WITHDRAWAL, withdrawal, description));
        acc->saveTransactions(acc->getName() + ".txt");

        std::cout << std::endl << "Withdrawal added!" << std::endl
                  << "\033[31mWithdrawal\033[0m - " << withdrawal << " \033[93mEuro\033[0m - \033[94m" << description
                  << "\033[0m" << std::endl;

        std::string c;
        std::cout << "Would you like to add another withdrawal? (type 'y' to continue) ";
        std::cin >> c;
        if (!caseInsensitiveCompare(c, "y")) ok = true;
        clearConsole();
    } while (!ok);
}

void viewUserTransactions() {
    std::string selection;
    clearConsole();

    for (const auto &transaction: acc->getTransactions()) {
        std::cout << transaction.toString() << std::endl;
    }
    if (acc->getTransactions().empty()) std::cout << "\033[33mNo transactions made.\033[0m" << std::endl;
    else {
        std::cout << "Account balance: ";
        if (acc->getBalance() <= 0) std::cout << "\033[31m" << acc->getBalance() << " Euro\033[0m" << std::endl;
        else std::cout << "\033[32m" << acc->getBalance() << " Euro\033[0m" << std::endl;
    }
    std::cout << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press Enter to continue... ";
    std::getline(std::cin, selection);
    clearConsole();
}


bool menu(std::string username) {
    int selection;
    bool exit = false;

    for (auto& account : accounts) {
        if (account.getName() == username) {
            acc = &account;
            break;
        }
    }

    clearConsole();

    do {
        std::cout << "Welcome \033[93m" << username << "\033[0m" << std::endl << std::endl;
        std::cout << "\033[36m----- Main menu -----\033[0m" << std::endl;
        std::cout << "\033[32m1 -\033[0m Make a deposit" << std::endl;
        std::cout << "\033[32m2 -\033[0m Make a withdrawal" << std::endl;
        std::cout << "\033[32m3 -\033[0m View all transactions and balance" << std::endl;
        std::cout << "\033[32m4 -\033[0m Log out" << std::endl;
        std::cout << "\033[32m5 -\033[0m Log out and return to OS" << std::endl;
        std::cout << "Enter your selection: ";
        std::cin >> selection;

        switch (selection) {
            case 1:
                makeDeposit();
                break;
            case 2:
                makeWithdrawal();
                break;
            case 3:
                viewUserTransactions();
                break;
            case 4:
                return true;
            case 5:
                return false;
            default:
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                clearConsole();
                std::cout << "\033[31mYour selection is not a valid one. Please try again\033[0m" << std::endl;
        }
    } while (true);
}

#endif //NEAMTU_FINANCES_USERMENU_H