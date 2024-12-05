#include "Transaction.h"

//definizione costruttore
Transaction::Transaction(Type type, double amount, const std::string& description)
        : type(type), amount(amount), description(description) {}

//ritorna il tipo di transazione (ovvero 1 = deposito o 2 = prelievo)
Transaction::Type Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getDescription() const {
    return description;
}

//ritorna a stringa la transazione nel formato tipo - somma - descrizione
std::string Transaction::toString() const {
    return (type==DEPOSIT ? std::string("\x1B[32mDeposit\x1B[0m") : std::string("\033[31mWithdrawal\033[0m")) + ": " +
    std::to_string(amount).substr(0, std::to_string(amount).find('.')+3) + " \033[93mEuro\033[0m - \033[94m" + description + "\033[0m";
}