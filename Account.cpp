#include <list>
#include "Account.h"

Account::Account(Account* a) : owner(a->owner), password(a->password)  {}

Account::Account(const std::string& o, int p) : owner(o), password(p) {}

void Account::addTransaction(const Transaction& transaction) {
    transactions.push_back(transaction);
}

void Account::removeTransaction(size_t index) {
    if (index < transactions.size()) {
        transactions.erase(transactions.begin() + index);
    }
}

const std::vector<Transaction>& Account::getTransactions() const {
    return transactions;
}

double Account::getBalance() const {
    double balance = 0.0;
    for (const auto& transaction : transactions) {
        if (transaction.getType() == Transaction::DEPOSIT) {
            balance += transaction.getAmount();
        } else {
            balance -= transaction.getAmount();
        }
    }
    return balance;
}

bool Account::loadTransactions(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    transactions.clear();
    std::string typeStr, description;
    double amount;
    while (file >> typeStr >> amount) {
        std::getline(file, description);
        Transaction::Type type = (typeStr == "Deposit") ? Transaction::DEPOSIT : Transaction::WITHDRAWAL;
        transactions.emplace_back(type, amount, description);
    }

    file.close();
    return true;
}

bool Account::loadAccounts(std::list<Account>& accounts) {
    std::ifstream file("accounts.txt");
    if (!file.is_open()) {
        return false;
    }

    if (!accounts.empty()) accounts.clear();
    std::string owner;
    int password;
    while (file >> owner >> password) {
        auto* p = new Account(owner, password);
        accounts.emplace_back(p);
        delete p;
    }

    file.close();
    return true;
}

bool Account::saveTransactions(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& transaction : transactions) {
        file << (transaction.getType() == Transaction::DEPOSIT ? "Deposit" : "Withdrawal") << " "
             << transaction.getAmount() << " " << transaction.getDescription() << "\n";
    }

    file.close();
    return true;
}

const std::string &Account::getOwner() const {
    return owner;
}
