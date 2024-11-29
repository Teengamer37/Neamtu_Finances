#include <list>
#include "Account.h"
#define KEY 'h'

Account::Account() {}

Account::Account(Account* a) : name(a->name), password(a->password)  {}

Account::Account(const Account &a) : name(a.name), password(a.password) {}

Account::Account(const std::string& name, int p) : name(name), password(p) {}

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

int Account::getNumTransactions() const {
    return transactions.size();
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
        description = description.substr(1);
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
    std::string name, password;
    while (std::getline(file, name) && std::getline(file, password)) {
        auto* p = new Account(name, std::stoi(xorEncryptDecrypt(password, KEY)));
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

bool Account::saveAccounts(const std::list<Account> &accounts) {
    std::ofstream file("accounts.txt");
    if (!file.is_open()) {
        return false;
    }

    for (const auto& account : accounts) {
        file << account.getName() << "\n" << xorEncryptDecrypt(std::to_string(account.getPassword()), KEY) << "\n";
    }

    file.close();
    return true;
}

bool Account::operator==(const Account &other) const {
    if (this->name == other.name && this->password == other.password) return true;
    return false;
}

const std::string &Account::getName() const {
    return name;
}

std::string Account::xorEncryptDecrypt(const std::string& data, char key) {
    std::string result = data;
    for (char& c : result) {
        c ^= key; // XOR each character with the key
    }
    return result;
}

int Account::getPassword() const {
    return password;
}

void Account::setPassword(int password) {
    Account::password = password;
}
