#include "Account.h"

Account::Account(const std::string& owner) : owner(owner) {}

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

bool Account::loadFromFile(const std::string& filename) {
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

bool Account::saveToFile(const std::string& filename) const {
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