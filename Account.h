#ifndef NEAMTU_FINANCES_ACCOUNT_H
#define NEAMTU_FINANCES_ACCOUNT_H

#include <vector>
#include <fstream>
#include "Transaction.h"

class Account {
public:
    //costruttori
    Account();
    Account(Account* a);
    Account(const Account& a);
    Account(const std::string& name, int password);

    //operazioni su singolo oggetto
    bool addTransaction(const Transaction& transaction);
    const std::vector<Transaction>& getTransactions() const;
    int getNumTransactions() const;
    double getBalance() const;

    //metodi per lettura/scrittura file
    bool loadTransactions();
    static bool loadAccounts(std::list<Account>& accounts);
    bool saveTransactions() const;
    static bool saveAccounts(const std::list<Account>& accounts);

    //getter/setter
    const std::string &getName() const;
    int getPassword() const;
    void setPassword(int password);

    //override operatore ==
    bool operator==(const Account& other) const;

    //metodo XOR per crittare/decrittare stringhe
    static std::string xorEncryptDecrypt(const std::string& data, char key);

private:
    std::string name;
    int password;
    std::vector<Transaction> transactions;
};

#endif //NEAMTU_FINANCES_ACCOUNT_H