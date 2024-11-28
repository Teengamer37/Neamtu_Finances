#include "Transaction.h"

Transaction::Transaction(Type type, double amount, const std::string& description)
        : type(type), amount(amount), description(description) {}

Transaction::Type Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getDescription() const {
    return description;
}

std::string Transaction::toString() const {
    return (type==DEPOSIT ? std::string("Deposit") : std::string("Withdrawal")) + ": " + std::to_string(amount).substr(0, std::to_string(amount).find('.')+3) + " - " + description;
}