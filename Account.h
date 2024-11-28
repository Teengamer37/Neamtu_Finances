#ifndef NEAMTU_FINANCES_ACCOUNT_H
#define NEAMTU_FINANCES_ACCOUNT_H

#include <vector>
#include <fstream>
#include "Transaction.h"

class Account {
public:
    Account(const std::string& owner);

    void addTransaction(const Transaction& transaction);
    void removeTransaction(size_t index);
    const std::vector<Transaction>& getTransactions() const;
    double getBalance() const;

    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

private:
    std::string owner;
    std::vector<Transaction> transactions;
};

#endif //NEAMTU_FINANCES_ACCOUNT_H