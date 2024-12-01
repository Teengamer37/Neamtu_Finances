#include <utility>

#ifndef NEAMTU_FINANCES_ADMINMENU_H
#define NEAMTU_FINANCES_ADMINMENU_H
#ifdef _WIN32
#define OS "windows"
#elif __unix__
#define OS "unix"
#endif

std::list<Account> accounts;

void clearConsole() {
    if (OS=="windows") {
        system("cls");
    } else {
        system("clear");
    }
}

bool caseInsensitiveCompare(const std::string& str1, const std::string& str2) {
    std::string lowerStr1 = str1;
    std::string lowerStr2 = str2;

    // Convert both strings to lowercase
    std::transform(lowerStr1.begin(), lowerStr1.end(), lowerStr1.begin(), ::tolower);
    std::transform(lowerStr2.begin(), lowerStr2.end(), lowerStr2.begin(), ::tolower);

    return lowerStr1 == lowerStr2;
}

void addAccount() {
    std::string newUsername;
    int newPassword;
    bool ok;
    clearConsole();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    do {
        ok = true;
        std::cout << "Choose a username \033[31m(no spaces allowed)\033[0m: ";
        std::getline(std::cin, newUsername);

        if (std::count(newUsername.begin(), newUsername.end(), ' ')!=0) {
            ok = false;
            std::string abort;
            clearConsole();
            std::cout << "\033[31mUsername cannot have spaces!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            for (const auto& account : accounts) {
                if (account.getName() == newUsername) {
                    std::string abort;
                    ok = false;
                    clearConsole();
                    std::cout << "\033[31mUsername already exists!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
                    std::cin >> abort;
                    if (caseInsensitiveCompare(abort, "n")) {
                        clearConsole();
                        return;
                    }
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
            std::string abort;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearConsole();
            std::cout << "\033[31mPassword must contain numbers only!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
            ok = false;
        }
    } while (!ok);

    accounts.push_back(new Account(newUsername, newPassword));
    Account::saveAccounts(accounts);

    clearConsole();
    std::cout << "\033[32mNew account added! Username: " << newUsername << " - Password: " << newPassword << "\033[0m" << std::endl;
}

void removeAccount() {
    std::string oldUsername;
    int oldPassword;
    bool ok;
    clearConsole();

    do {
        ok = true;
        std::cout << "Enter the username you want to delete: ";
        std::cin >> oldUsername;
        std::cout << "Enter the password of that account: ";
        std::cin >> oldPassword;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (caseInsensitiveCompare(oldUsername, "admin")) {
            std::string abort;
            ok = false;
            clearConsole();
            std::cout << "\033[31mYou cannot delete the admin profile!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            ok = false;
            Account removedAccount;
            for (const auto& account : accounts) {
                if (account.getName() == oldUsername && account.getPassword() == oldPassword) {
                    ok = true;
                    removedAccount = account;
                    break;
                }
            }
            if (!ok) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mUsername does not exist or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else {
                accounts.remove(removedAccount);
                Account::saveAccounts(accounts);

                clearConsole();
                std::cout << "\033[32mAccount removed! Old username: " << removedAccount.getName() << " - Old password: " << removedAccount.getPassword() << "\033[0m" << std::endl;
            }
        }
    } while (!ok);
}

void viewAccounts() {
    char c;
    std::string selection;
    clearConsole();
    for (const auto& account : accounts) {
        if (account.getName() != "admin")
            std::cout << "Username: " << account.getName() << " - Password: " << account.getPassword() << " - No. of transactions: " << account.getNumTransactions() << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press Enter to continue... ";
    std::getline(std::cin, selection);
    clearConsole();
}

void changeAccPw() {
    std::string user;
    int pwd;
    bool ok;
    clearConsole();

    do {
        std::cout << "Enter username: ";
        std::cin >> user;
        std::cout << "Enter current password: ";
        std::cin >> pwd;

        if (std::cin.fail()) {
            ok = false;
            std::string abort;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearConsole();
            std::cout << "\033[31mUsername not found or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else if (user == "admin") {
            ok = false;

            std::string abort;
            clearConsole();
            std::cout << "\033[31mWrong option to change admin's password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            ok = false;
            for (auto& account : accounts) {
                if (account.getName() == user && account.getPassword() == pwd) {
                    int newPwd;
                    do {
                        std::cout << "Enter new password \033[31m(numbers only)\033[0m: ";
                        std::cin >> newPwd;

                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            clearConsole();
                            std::cout << "\033[31mPassword must contain numbers only!\033[0m" << std::endl;
                        } else {
                            account.setPassword(newPwd);
                            Account::saveAccounts(accounts);
                            ok = true;

                            clearConsole();
                            std::cout << "\033[32mPassword for " << account.getName() << " changed! New password: " << newPwd << "\033[0m" << std::endl;
                        }
                    } while (!ok);
                    break;
                }
            }
        }
    } while (!ok);
}

void addAccTransaction() {
    std::string user;
    bool ok;

    clearConsole();
    do {
        ok = false;
        bool found = false;
        std::cout << "Enter username: ";
        std::cin >> user;

        if (user == "admin") {
            std::string abort;
            clearConsole();
            std::cout << "\033[31mYou cannot add transactions to admin account!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            for (auto& account : accounts) {
                if (account.getName() == user) {
                    found = true;
                    int type;
                    double amount;
                    std::string description;

                    do {
                        std::cout << "Would you like to add a \033[32mdeposit\033[0m (1) or a \033[31mwithdrawal\033[0m (2)? ";
                        std::cin >> type;

                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                            std::string abort;
                            clearConsole();
                            std::cout << "\033[31mYou entered a non-valid option!\033[0m Would you like to try again? (type 'n' to abort) ";
                            std::cin >> abort;
                            if (caseInsensitiveCompare(abort, "n")) {
                                clearConsole();
                                return;
                            }
                        } else if (type != 1 && type != 2) {
                            std::string abort;
                            clearConsole();
                            std::cout << "\033[31mYou entered a non-valid option!\033[0m Would you like to try again? (type 'n' to abort) ";
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

                    do {
                        std::cout << "Please add an amount: ";
                        std::cin >> amount;

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
                        } else if (amount <= 0) {
                            std::string abort;
                            clearConsole();
                            std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
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

                    account.addTransaction(Transaction((type == 1 ? Transaction::DEPOSIT : Transaction::WITHDRAWAL), amount, description));
                    account.saveTransactions(account.getName() + ".txt");

                    std::cout << std::endl << "Transaction added to " << account.getName() << "!" << std::endl
                              << (type==1 ? "\033[32mDeposit\033[0m" : "\033[31mWithdrawal\033[0m") << " - " << amount << " - " << description << std::endl;

                    std::string c;
                    std::cout << "Would you like to add other transactions? (type 'y' to continue) ";
                    std::cin >> c;
                    if (!caseInsensitiveCompare(c, "y")) ok = true;
                    clearConsole();
                    break;
                }
            }

            if (!found) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mNo username found!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            }
        }
    } while (!ok);

    clearConsole();
}

void viewTransactions() {
    std::string selection;
    clearConsole();

    std::cout << "Do you want to print ALL accounts history or specific ones? (type 'a' to print all) ";
    std::cin >> selection;
    if (caseInsensitiveCompare(selection, "a")) {
        clearConsole();
        for (const auto& account : accounts) {
            if (account.getName() != "admin") {
                std::cout << "Account name: " << account.getName() << std::endl;
                for (const auto &transaction: account.getTransactions()) {
                    std::cout << transaction.toString() << std::endl;
                }
                if (account.getTransactions().empty()) std::cout << "\033[33mNo transactions made.\033[0m" << std::endl;
                else {
                    std::cout << "Account balance: ";
                    if (account.getBalance() <= 0) std::cout << "\033[31m" << account.getBalance() << " Euro\033[0m" << std::endl;
                    else std::cout << "\033[32m" << account.getBalance() << " Euro\033[0m" << std::endl;
                }
                std::cout << std::endl;
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Press Enter to continue... ";
        std::getline(std::cin, selection);
        clearConsole();
    } else {
        std::string name;
        bool cycle = true;

        do {
            std::cout << "Enter account name: ";
            std::cin >> name;
            if (!caseInsensitiveCompare(name, "admin")) {
                bool found = false;
                for (const auto &account: accounts) {
                    if (caseInsensitiveCompare(account.getName(), name)) {
                        found = true;
                        for (const auto &transaction: account.getTransactions()) {
                            std::cout << transaction.toString() << std::endl;
                        }
                        if (account.getTransactions().empty()) std::cout << "\033[33mNo transactions made.\033[0m" << std::endl;
                        else {
                            std::cout << "Account balance: ";
                            if (account.getBalance() <= 0) std::cout << "\033[31m" << account.getBalance() << " Euro\033[0m" << std::endl;
                            else std::cout << "\033[32m" << account.getBalance() << " Euro\033[0m" << std::endl;
                        }
                        std::cout << std::endl;
                        break;
                    }
                }
                if (!found) std::cout << "\033[31mNo account found with that name!\033[0m ";
                std::string choice;
                std::cout << "Would you like to check other accounts? (type 'y' and Enter to continue) ";
                std::cin >> choice;
                if (!caseInsensitiveCompare(choice, "y")) cycle = false;
                clearConsole();
            } else {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mYou cannot see transactions to admin account!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            }
        } while (cycle);
    }
}

void changeAdminPw() {
    int pwd, newPwd;
    bool ok = true;
    clearConsole();

    do {
        std::cout << "Enter current password: ";
        std::cin >> pwd;

        if (std::cin.fail()) {
            ok = false;
            std::string abort;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearConsole();
            std::cout << "\033[31mUsername not found or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            for (auto& account : accounts) {
                if (account.getName() == "admin" && account.getPassword() == pwd) {
                    do {
                        ok = false;

                        std::cout << "Enter new password \033[31m(numbers only)\033[0m: ";
                        std::cin >> newPwd;

                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            clearConsole();
                            std::cout << "\033[31mPassword must contain numbers only!\033[0m" << std::endl;
                        } else {
                            account.setPassword(newPwd);
                            Account::saveAccounts(accounts);
                            ok = true;

                            clearConsole();
                            std::cout << "\033[32mPassword changed! New password: " << newPwd << "\033[0m" << std::endl;
                        }
                    } while (!ok);
                    break;
                } else if (account.getName() == "admin" && account.getPassword() != pwd) {
                    ok = false;
                    std::string abort;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    clearConsole();
                    std::cout << "\033[31mWrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
                    std::cin >> abort;
                    if (caseInsensitiveCompare(abort, "n")) {
                        clearConsole();
                        return;
                    }
                }
            }
        }
    } while (!ok);
}

bool adminMenu() {
    int selection;
    bool exit = false;

    clearConsole();

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

                clearConsole();
                std::cout << "\033[31mYour selection is not a valid one. Please try again\033[0m" << std::endl;
        }
    } while (true);
}

#endif //NEAMTU_FINANCES_ADMINMENU_H
