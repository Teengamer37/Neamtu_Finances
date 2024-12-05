#include <list>
#include "Account.h"
#define KEY 'h'     //questa chiave serve per crittare/decrittare le password salvate su accounts.txt

//definizione dei costruttori
Account::Account() {}

Account::Account(Account* a) : name(a->name), password(a->password)  {}

Account::Account(const Account& other) : name(other.name), password(other.password), transactions(other.transactions) {}

Account::Account(const std::string& name, int p) : name(name), password(p) {}

//metodo che aggiunge una transazione all'account
bool Account::addTransaction(const Transaction& transaction) {
    if ((transaction.getType()==Transaction::DEPOSIT || transaction.getType()==Transaction::WITHDRAWAL) && transaction.getAmount()>0) {
        transactions.push_back(transaction);
        return true;
    }
    return false;
}

//metodo che fornisce un vector di transazioni effettuate dall'account
const std::vector<Transaction>& Account::getTransactions() const {
    return transactions;
}

//ritorna il numero di transazioni effettuate dall'account
int Account::getNumTransactions() const {
    return transactions.size();
}

//calcola il bilancio del conto scorrendo tra le transazioni effettuate
double Account::getBalance() const {
    double balance = 0.0;
    for (const auto& transaction : transactions) {
        //se è un DEPOSIT, allora viene aggiunta la somma al bilancio...
        if (transaction.getType() == Transaction::DEPOSIT) {
            balance += transaction.getAmount();
        //...al contrario, se è un WITHDRAWAL, si sottrae la somma dal bilancio
        } else {
            balance -= transaction.getAmount();
        }
    }
    return balance;
}

//carica le transazioni dell'account da un file [nomeAccount].txt
bool Account::loadTransactions() {
    std::ifstream file(name + ".txt");

    //se il file è aperto, allora annullo l'operazione, visto che non posso accedere al file
    if (!file.is_open()) {
        return false;
    }

    //elimino le transazioni correnti (se ce ne sono) e scorro nel file salvando le transazioni
    transactions.clear();
    std::string typeStr, description;
    double amount;
    while (file >> typeStr >> amount) {
        std::getline(file, description);
        Transaction::Type type = (typeStr == "Deposit") ? Transaction::DEPOSIT : Transaction::WITHDRAWAL;
        description = description.substr(1);
        transactions.emplace_back(type, amount, description);
    }

    //chiudo il file
    file.close();
    return true;
}

//metodo statico che mi permette di caricare in una List vari account salvati nel file accounts.txt
bool Account::loadAccounts(std::list<Account>& accounts) {
    std::ifstream file("accounts.txt");

    //se il file è aperto, allora annullo l'operazione, visto che non posso accedere al file
    if (!file.is_open()) {
        return false;
    }

    //elimino gli account correnti (se ce ne sono) e scorro nel file salvando i vari account decrittando le password
    accounts.clear();
    std::string name, password;
    while (std::getline(file, name) && std::getline(file, password)) {
        auto* p = new Account(name, std::stoi(xorEncryptDecrypt(password, KEY)));
        accounts.emplace_back(p);
        delete p;
    }

    //chiudo il file
    file.close();
    return true;
}

//salva le transazioni dell'account nel file [nomeAccount].txt
bool Account::saveTransactions() const {
    std::ofstream file(name + ".txt");

    //se il file è aperto, allora termino l'operazione, visto che non posso accedere al file
    if (!file.is_open()) {
        return false;
    }

    //immetto ad ogni riga il tipo di transazione, la somma e la descrizione
    for (const auto& transaction : transactions) {
        file << (transaction.getType() == Transaction::DEPOSIT ? "Deposit" : "Withdrawal") << " "
             << transaction.getAmount() << " " << transaction.getDescription() << "\n";
    }

    //chiudo il file
    file.close();
    return true;
}

//metodo statico che salva gli account in accounts.txt
bool Account::saveAccounts(const std::list<Account> &accounts) {
    std::ofstream file("accounts.txt");

    //se il file è aperto, allora termino l'operazione, visto che non posso accedere al file
    if (!file.is_open()) {
        return false;
    }

    //nelle righe dispari ci sono i nomi utenti, nelle righe pari le password crittate
    for (const auto& account : accounts) {
        file << account.getName() << "\n" << xorEncryptDecrypt(std::to_string(account.getPassword()), KEY) << "\n";
    }

    //chiudo il file
    file.close();
    return true;
}

//faccio un override dell'operatore == affinché possa rilevare se un account è uguale all'altro (che potrei erroneamente
//immettere nella lista di account) senza verificare le transazioni
bool Account::operator==(const Account &other) const {
    if (this->name == other.name && this->password == other.password) return true;
    return false;
}

const std::string &Account::getName() const {
    return name;
}

int Account::getPassword() const {
    return password;
}

void Account::setPassword(int password) {
    Account::password = password;
}

//metodo statico che critta/decritta attraverso lo XOR una stringa data
//in questo programma, lo usiamo per crittare/decrittare le password quando andiamo a salvare/prelevare dal file
std::string Account::xorEncryptDecrypt(const std::string& data, char key) {
    std::string result = data;

    //faccio lo XOR a ciascun carattere con la chiave passata
    for (char& c : result) {
        c ^= key;
    }

    return result;
}