#ifndef NEAMTU_FINANCES_TRANSACTION_H
#define NEAMTU_FINANCES_TRANSACTION_H

#include <string>
#include <iostream>

class Transaction {
public:
    enum Type { DEPOSIT, WITHDRAWAL };

    Transaction(Type type, double amount, const std::string& description);

    Type getType() const;
    double getAmount() const;
    std::string getDescription() const;

    std::string toString() const;

private:
    Type type;
    double amount;
    std::string description;
};

#endif //NEAMTU_FINANCES_TRANSACTION_H