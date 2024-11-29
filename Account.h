#ifndef NEAMTU_FINANCES_ACCOUNT_H
#define NEAMTU_FINANCES_ACCOUNT_H

#include <vector>
#include <fstream>
#include "Transaction.h"

class Account {
public:
    Account();
    Account(Account* a);
    Account(const Account& a);
    Account(const std::string& name, int password);

    void addTransaction(const Transaction& transaction);
    void removeTransaction(size_t index);
    const std::vector<Transaction>& getTransactions() const;
    int getNumTransactions() const;
    double getBalance() const;

    bool loadTransactions(const std::string& filename);
    static bool loadAccounts(std::list<Account>& accounts);
    bool saveTransactions(const std::string& filename) const;
    static bool saveAccounts(const std::list<Account>& accounts);
    const std::string &getName() const;
    int getPassword() const;
    void setPassword(int password);

    bool operator==(const Account& other) const;

    static std::string xorEncryptDecrypt(const std::string& data, char key);

private:
    std::string name;
    int password;
    std::vector<Transaction> transactions;
};

#endif //NEAMTU_FINANCES_ACCOUNT_H