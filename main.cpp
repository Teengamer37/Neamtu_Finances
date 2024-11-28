#include <iostream>
#include <list>
#include "Account.h"

int main() {
    std::list<Account> accounts;
    Account::loadAccounts(accounts);

    for (auto& account : accounts) {
        account.loadTransactions(account.getOwner() + ".txt");
    }

    std::cout << "Transactions:\n";
    for (auto& account : accounts) {
        for (const auto &transaction: account.getTransactions()) {
            std::cout << transaction.toString() << "\n";
        }

        std::cout << "Balance: " << account.getBalance() << "\n";

        account.saveTransactions(account.getOwner() + ".txt");
    }



    Account loadedAccount("JohnDoe", 22);
    loadedAccount.loadTransactions("JohnDoe.txt");

    std::cout << "Loaded Transactions:\n";
    for (const auto& transaction : loadedAccount.getTransactions()) {
        std::cout << transaction.toString() << "\n";
    }

    std::cout << "Loaded Balance: " << loadedAccount.getBalance() << "\n";

    return 0;
}