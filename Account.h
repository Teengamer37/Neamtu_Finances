#ifndef NEAMTU_FINANCES_ACCOUNT_H
#define NEAMTU_FINANCES_ACCOUNT_H

#include <vector>
#include <fstream>
#include "Transaction.h"

class Account {
public:
    Account(Account* a);
    Account(const std::string& owner, int password);

    void addTransaction(const Transaction& transaction);
    void removeTransaction(size_t index);
    const std::vector<Transaction>& getTransactions() const;
    double getBalance() const;

    bool loadTransactions(const std::string& filename);
    static bool loadAccounts(std::list<Account>& accounts);
    bool saveTransactions(const std::string& filename) const;
    const std::string &getOwner() const;

private:
    std::string owner;
    int password;
    std::vector<Transaction> transactions;
};

#endif //NEAMTU_FINANCES_ACCOUNT_H