#include <iostream>
#include "Account.h"

int main() {
    Account account("John Doe");

    account.addTransaction(Transaction(Transaction::DEPOSIT, 1000.71, "Salary"));
    account.addTransaction(Transaction(Transaction::WITHDRAWAL, 280.00, "Groceries"));

    std::cout << "Transactions:\n";
    for (const auto& transaction : account.getTransactions()) {
        std::cout << transaction.toString() << "\n";
    }

    std::cout << "Balance: " << account.getBalance() << "\n";

    account.saveToFile("John Doe.txt");

    Account loadedAccount("John Doe");
    loadedAccount.loadFromFile("John Doe.txt");

    std::cout << "Loaded Transactions:\n";
    for (const auto& transaction : loadedAccount.getTransactions()) {
        std::cout << transaction.toString() << "\n";
    }

    std::cout << "Loaded Balance: " << loadedAccount.getBalance() << "\n";

    return 0;
}