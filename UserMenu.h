#ifndef NEAMTU_FINANCES_USERMENU_H
#define NEAMTU_FINANCES_USERMENU_H
#include "AdminMenu.h"
#include <memory>

//variabile globale per fare le varie modifiche sull'account corrente
Account* acc;

//permette di fare un deposito in questo account
void makeDeposit() {
    std::string description;
    double deposit;
    bool ok = false;

    clearConsole();

    do {
        //inserimento somma da depositare
        do {
            std::cout << "Insert the amount to deposit: ";
            std::cin >> deposit;

            //se l'utente inserisce un carattere diverso dai numeri, resetto il buffer input e chiedo se si desidera ritentare
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string abort;
                clearConsole();
                std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            //se l'utente inserisce un valore minore o uguale di zero, avviso dell'errore e chiedo se vuole ritentare
            } else if (deposit <= 0) {
                std::string abort;
                clearConsole();
                std::cout
                        << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else {
                ok = true;
            }
        } while (!ok);

        ok = false;

        //inserimento descrizione del deposito
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please add a description: ";
        std::getline(std::cin, description);

        //inserisco il deposito nelle transazioni
        acc->addTransaction(Transaction(Transaction::DEPOSIT, deposit, description));
        acc->saveTransactions();

        //stampo il riepilogo della transazione
        std::cout << std::endl << "Deposit added!" << std::endl
                  << "\033[32mDeposit\033[0m - " << deposit << " \033[93mEuro\033[0m - \033[94m" << description
                  << "\033[0m" << std::endl;

        //propongo se si vuole fare un altro deposito
        std::string c;
        std::cout << "Would you like to add another deposit? (type 'y' to continue) ";
        std::cin >> c;
        if (!caseInsensitiveCompare(c, "y")) ok = true;
        clearConsole();
    } while (!ok);
}

//permette di fare un prelievo da questo account
void makeWithdrawal() {
    std::string description, selection;
    double withdrawal;
    bool ok = false;

    clearConsole();

    do {
        //verifico se l'account ha un bilancio >0
        if (acc->getBalance() <= 0) {
            //se non soddisfa il criterio, fermo l'operazione avvisando il cliente
            std::cout << "\033[31mYour account balance is in red! You cannot make withdrawals!\033[0m" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Press Enter to continue... ";
            std::getline(std::cin, selection);
            clearConsole();
            return;
        }

        //faccio inserire l'importo da prelevare
        do {
            std::cout << "Insert the amount to withdraw: ";
            std::cin >> withdrawal;

            //se l'utente inserisce un carattere diverso dai numeri, resetto il buffer input e chiedo se si desidera ritentare
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string abort;
                clearConsole();
                std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            //se l'utente inserisce un valore minore o uguale di zero, avviso dell'errore e chiedo se vuole ritentare
            } else if (withdrawal <= 0) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            //se l'utente inserisce un valore troppo grosso rispetto al bilancio disponibile,
            //avviso di ciò e chiedo se vuole ritentare
            } else if (acc->getBalance()-withdrawal < 0) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mInsufficient funds! Your current balance: " << acc->getBalance() << " Euro\033[0m"
                          << std::endl << "Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else {
                ok = true;
            }
        } while (!ok);

        ok = false;

        //faccio aggiungere una descrizione del prelievo
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please add a description: ";
        std::getline(std::cin, description);

        //aggiungo la transazione e la salvo in [nomeAccount].txt
        acc->addTransaction(Transaction(Transaction::WITHDRAWAL, withdrawal, description));
        acc->saveTransactions();

        std::cout << std::endl << "Withdrawal added!" << std::endl
                  << "\033[31mWithdrawal\033[0m - " << withdrawal << " \033[93mEuro\033[0m - \033[94m" << description
                  << "\033[0m" << std::endl;

        //propongo se si vuol fare altre transazioni
        std::string c;
        std::cout << "Would you like to add another withdrawal? (type 'y' to continue) ";
        std::cin >> c;
        if (!caseInsensitiveCompare(c, "y")) ok = true;
        clearConsole();
    } while (!ok);
}

//visualizza tutte le transazioni fatte dall'utente
void viewUserTransactions() {
    std::string selection;
    clearConsole();

    //stampa transazioni
    for (const auto &transaction: acc->getTransactions()) {
        std::cout << transaction.toString() << std::endl;
    }

    //se non è stata fatta nessuna transazione, avviso l'utente
    if (acc->getTransactions().empty()) std::cout << "\033[33mNo transactions made.\033[0m" << std::endl;
    else {
        //se viene fatta almeno una transazione, stampo il bilancio
        std::cout << "Account balance: ";
        if (acc->getBalance() <= 0) std::cout << "\033[31m" << acc->getBalance() << " Euro\033[0m" << std::endl;
        else std::cout << "\033[32m" << acc->getBalance() << " Euro\033[0m" << std::endl;
    }
    std::cout << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press Enter to continue... ";
    std::getline(std::cin, selection);
    clearConsole();
}

//menu utente
bool menu(const std::string& username) {
    int selection;

    //cerco l'account e lo salvo nella variabile globale acc
    for (auto& account : accounts) {
        if (account.getName() == username) {
            acc = &account;
            break;
        }
    }

    clearConsole();

    //stampo il menu
    do {
        std::cout << "Welcome \033[93m" << username << "\033[0m" << std::endl << std::endl;
        std::cout << "\033[36m----- Main menu -----\033[0m" << std::endl;
        std::cout << "\033[32m1 -\033[0m Make a deposit" << std::endl;
        std::cout << "\033[32m2 -\033[0m Make a withdrawal" << std::endl;
        std::cout << "\033[32m3 -\033[0m View all transactions and balance" << std::endl;
        std::cout << "\033[32m4 -\033[0m Log out" << std::endl;
        std::cout << "\033[32m5 -\033[0m Log out and return to OS" << std::endl;
        std::cout << "Enter your selection: ";
        std::cin >> selection;

        //verifico la scelta fatta e chiamo la funzione selezionata
        switch (selection) {
            case 1:
                makeDeposit();
                break;
            case 2:
                makeWithdrawal();
                break;
            case 3:
                viewUserTransactions();
                break;
            case 4:
                return true;
            case 5:
                return false;
            default:
                //se viene inserito un carattere diverso da un numero, resetto il buffer input
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                clearConsole();
                std::cout << "\033[31mYour selection is not a valid one. Please try again\033[0m" << std::endl;
        }
    } while (true);
}

#endif //NEAMTU_FINANCES_USERMENU_H