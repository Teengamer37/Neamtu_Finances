#include <iostream>
#include <list>
#include <limits>
#include <algorithm>
#include "Account.h"
#define KEY 'h'

std::list<Account> accounts;

bool caseInsensitiveCompare(const std::string& str1, const std::string& str2) {
    std::string lowerStr1 = str1;
    std::string lowerStr2 = str2;

    // Convert both strings to lowercase
    std::transform(lowerStr1.begin(), lowerStr1.end(), lowerStr1.begin(), ::tolower);
    std::transform(lowerStr2.begin(), lowerStr2.end(), lowerStr2.begin(), ::tolower);

    return lowerStr1 == lowerStr2;
}

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
            ok = false;
            std::string abort;
            system("cls");
            std::cout << "\033[31mUsername cannot have spaces!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                system("cls");
                return;
            }
        } else {
            for (const auto& account : accounts) {
                if (account.getName() == newUsername) {
                    std::string abort;
                    ok = false;
                    system("cls");
                    std::cout << "\033[31mUsername already exists!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
                    std::cin >> abort;
                    if (caseInsensitiveCompare(abort, "n")) {
                        system("cls");
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
            system("cls");
            std::cout << "\033[31mPassword must contain numbers only!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                system("cls");
                return;
            }
            ok = false;
        }
    } while (!ok);

    accounts.push_back(new Account(newUsername, newPassword));
    Account::saveAccounts(accounts);

    system("cls");
    std::cout << "\033[32mNew account added! Username: " << newUsername << " - Password: " << newPassword << "\033[0m" << std::endl;
}

void removeAccount() {
    std::string oldUsername;
    int oldPassword;
    bool ok;
    system("cls");

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
            system("cls");
            std::cout << "\033[31mYou cannot delete the admin profile!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                system("cls");
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
                system("cls");
                std::cout << "\033[31mUsername does not exist or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    system("cls");
                    return;
                }
            } else {
                accounts.remove(removedAccount);
                Account::saveAccounts(accounts);

                system("cls");
                std::cout << "\033[32mAccount removed! Old username: " << removedAccount.getName() << " - Old password: " << removedAccount.getPassword() << "\033[0m" << std::endl;
            }
        }
    } while (!ok);
}

void viewAccounts() {
    char c;
    system("cls");
    for (const auto& account : accounts) {
        if (account.getName() != "admin")
        std::cout << "Username: " << account.getName() << " - Password: " << account.getPassword() << " - No. of transactions: " << account.getNumTransactions() << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl << "Press any key and then Enter to return to menu... " << std::flush;
    std::cin.get();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    system("cls");
}

void changeAccPw() {
    std::string user;
    int pwd;
    bool ok;
    system("cls");

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
            system("cls");
            std::cout << "\033[31mUsername not found or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                system("cls");
                return;
            }
        } else if (user == "admin") {
            ok = false;

            std::string abort;
            system("cls");
            std::cout << "\033[31mWrong option to change admin's password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                system("cls");
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
                            system("cls");
                            std::cout << "\033[31mPassword must contain numbers only!\033[0m" << std::endl;
                        } else {
                            account.setPassword(newPwd);
                            Account::saveAccounts(accounts);
                            ok = true;

                            system("cls");
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

    system("cls");
    do {
        ok = false;
        bool found = false;
        std::cout << "Enter username: ";
        std::cin >> user;

        if (user == "admin") {
            std::string abort;
            system("cls");
            std::cout << "\033[31mYou cannot add transactions to admin account!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                system("cls");
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
                            system("cls");
                            std::cout << "\033[31mYou entered a non-valid option!\033[0m Would you like to try again? (type 'n' to abort) ";
                            std::cin >> abort;
                            if (caseInsensitiveCompare(abort, "n")) {
                                system("cls");
                                return;
                            }
                        } else if (type != 1 && type != 2) {
                            std::string abort;
                            system("cls");
                            std::cout << "\033[31mYou entered a non-valid option!\033[0m Would you like to try again? (type 'n' to abort) ";
                            std::cin >> abort;
                            if (caseInsensitiveCompare(abort, "n")) {
                                system("cls");
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
                            system("cls");
                            std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                            std::cin >> abort;
                            if (caseInsensitiveCompare(abort, "n")) {
                                system("cls");
                                return;
                            }
                        } else if (amount <= 0) {
                            std::string abort;
                            system("cls");
                            std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                            std::cin >> abort;
                            if (caseInsensitiveCompare(abort, "n")) {
                                system("cls");
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
                    system("cls");
                    break;
                }
            }

            if (!found) {
                std::string abort;
                system("cls");
                std::cout << "\033[31mNo username found!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    system("cls");
                    return;
                }
            }
        }
    } while (!ok);

    system("cls");
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