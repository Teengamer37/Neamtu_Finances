#include <utility>
#include <filesystem>
#ifndef NEAMTU_FINANCES_ADMINMENU_H
#define NEAMTU_FINANCES_ADMINMENU_H
#ifdef _WIN32
#define OS "windows"
#elif __unix__
#define OS "unix"
#endif

//variabile globale che per tutta la durata del programma mi mantiene in memoria i vari account
std::list<Account> accounts;

//funzione che serve a cancellare tutti i comandi della console verificando se si sta eseguendo il programma su
//macchina Windows o Unix-like
void clearConsole() {
    if (OS=="windows") {
        system("cls");
    } else {
        system("clear");
    }
}

//funzione che verifica se due stringhe sono uguali senza mettere in considerazione se i caratteri sono minuscoli/maiuscoli
bool caseInsensitiveCompare(const std::string& str1, const std::string& str2) {
    std::string lowerStr1 = str1;
    std::string lowerStr2 = str2;

    //attraverso questi due comandi, vado a trasformare le stringhe facendole avere caratteri tutti minuscoli
    std::transform(lowerStr1.begin(), lowerStr1.end(), lowerStr1.begin(), ::tolower);
    std::transform(lowerStr2.begin(), lowerStr2.end(), lowerStr2.begin(), ::tolower);

    return lowerStr1 == lowerStr2;
}

//aggiunge un account
void addAccount() {
    std::string newUsername;
    int newPassword;
    bool ok;

    clearConsole();
    //serve per "catturare" lo spazio se l'utente erroneamente lo preme
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    do {
        //input username nuovo
        ok = true;
        std::cout << "Choose a username \033[31m(no spaces allowed)\033[0m: ";
        std::getline(std::cin, newUsername);

        //se contiene spazi, mi fermo e chiedo all'utente se vuole ritentare
        if (std::count(newUsername.begin(), newUsername.end(), ' ')!=0) {
            ok = false;
            std::string abort;
            clearConsole();
            std::cout << "\033[31mUsername cannot have spaces!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            //controllo se esiste un utente già salvato con lo username inserito
            for (const auto& account : accounts) {
                //vado a comparare i vari username usando la funzione creata a riga 24 (caseInsensitiveCompare())
                //se uno degli username combacia, mi fermo e chiedo all'utente se vuole ritentare
                if (caseInsensitiveCompare(account.getName(), newUsername)) {
                    std::string abort;
                    ok = false;
                    clearConsole();
                    std::cout << "\033[31mUsername already exists!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
                    std::cin >> abort;
                    if (caseInsensitiveCompare(abort, "n")) {
                        clearConsole();
                        return;
                    }
                    break;
                }
            }
        }
    } while (!ok);

    do {
        //input password nuova
        ok = true;
        std::cout << "Choose a password \033[31m(numbers only)\033[0m: ";
        std::cin >> newPassword;

        //se l'utente immette caratteri e non numeri, mi fermo e chiedo all'utente se vuole ritentare
        if (std::cin.fail()) {
            std::string abort;

            //cancello e resetto il buffer input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            clearConsole();
            std::cout << "\033[31mPassword must contain numbers only!\033[0m Would you like to try again? (type 'n' to abort) " << std::endl;
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
            ok = false;
        }
    } while (!ok);

    //inserisco nella List il nuovo account e salvo il tutto nel file accounts.txt
    accounts.push_back(new Account(newUsername, newPassword));
    Account::saveAccounts(accounts);

    clearConsole();
    std::cout << "\033[32mNew account added! Username: " << newUsername << " - Password: " << newPassword << "\033[0m" << std::endl;
}

//rimuove un account inserendo lo username e la password dell'account da eliminare
void removeAccount() {
    std::string oldUsername;
    int oldPassword;
    bool ok;
    clearConsole();

    do {
        //input username e password
        std::cout << "Enter the username you want to delete: ";
        std::cin >> oldUsername;
        std::cout << "Enter the password of that account: ";
        std::cin >> oldPassword;

        //se alla password viene inserito un carattere e non solo numeri, resetto il buffer input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        //se si tenta di cancellare l'account admin, fermo il tentativo senza controllare la password e chiedo se
        //si vuole ritentare l'operazione
        if (caseInsensitiveCompare(oldUsername, "admin")) {
            std::string abort;
            ok = false;
            clearConsole();
            std::cout << "\033[31mYou cannot delete the admin profile!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            ok = false;
            Account removedAccount(oldUsername, oldPassword);
            //cerco tra tutti gli account e se trovo uno con username e password uguali, proseguo con l'eliminazione
            for (const auto& account : accounts) {
                if (account == removedAccount) {
                    ok = true;
                    removedAccount = account;
                    break;
                }
            }

            //se non trovo l'account, mi fermo e chiedo all'utente se vuole ritentare l'operazione
            if (!ok) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mUsername does not exist or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            } else {
                //sennò, elimino il file di salvataggio del corrispettivo account (se esiste) e successivamente rimuovo
                //l'account dalla List e da accounts.txt
                try {
                    std::filesystem::remove(removedAccount.getName() + ".txt");
                    accounts.remove(removedAccount);
                    Account::saveAccounts(accounts);
                    clearConsole();
                    std::cout << "\033[32mAccount removed! Old username: " << removedAccount.getName() << " - Old password: " << removedAccount.getPassword() << "\033[0m" << std::endl;
                } catch (const std::filesystem::filesystem_error& e) {
                    //se succede qualche errore nella cancellazione del file, annullo l'operazione
                    clearConsole();
                    std::cout << "\033[31mAccount not removed! Error: " << e.what() << std::endl <<"Please try again\033[0m" << std::endl;
                }
            }
        }
    } while (!ok);
}

//visualizzo tutti gli account (tranne admin)
void viewAccounts() {
    std::string selection;
    clearConsole();
    for (const auto& account : accounts) {
        if (account.getName() != "admin")
            std::cout << "Username: \033[35m" << account.getName() << "\033[0m - Password: \033[35m" << account.getPassword()
            << "\033[0m - No. of transactions: \033[35m" << account.getNumTransactions() << "\033[0m" << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press Enter to continue... ";
    std::getline(std::cin, selection);
    clearConsole();
}

//cambia la password di un account
void changeAccPw() {
    std::string user;
    int pwd;
    bool ok;
    clearConsole();

    do {
        //l'utente fornisce username e password dell'account da modificare
        std::cout << "Enter username: ";
        std::cin >> user;
        std::cout << "Enter current password: ";
        std::cin >> pwd;

        //se viene inserito un carattere diverso da un numero alla password, resetto il buffer input e chiedo all'utente
        //se vuole ritentare
        if (std::cin.fail()) {
            ok = false;
            std::string abort;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearConsole();
            std::cout << "\033[31mUsername not found or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        //se si prova a cambiare password all'admin, mi fermo avvisando che c'è l'opzione apposta per farlo
        } else if (user == "admin") {
            ok = false;

            std::string abort;
            clearConsole();
            std::cout << "\033[31mWrong option to change admin's password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            ok = false;
            //cerco l'account con stesso username e password
            for (auto& account : accounts) {
                if (account.getName() == user && account.getPassword() == pwd) {
                    int newPwd;
                    do {
                        std::cout << "Enter new password \033[31m(numbers only)\033[0m: ";
                        std::cin >> newPwd;

                        //se viene inserito un carattere diverso dai numeri, avviso l'utente e lo faccio ritentare
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            clearConsole();
                            std::cout << "\033[31mPassword must contain numbers only!\033[0m" << std::endl;
                        } else {
                            //imposto la nuova password e salvo in accounts.txt
                            account.setPassword(newPwd);
                            Account::saveAccounts(accounts);
                            ok = true;

                            clearConsole();
                            std::cout << "\033[32mPassword for " << account.getName() << " changed! New password: " << newPwd << "\033[0m" << std::endl;
                        }
                    } while (!ok);
                    break;
                }
            }

            //se non ho trovato account, allora avviso l'utente e gli chiedo se vuole ritentare
            if (!ok) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mUsername not found or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            }
        }
    } while (!ok);
}

//aggiunge depositi o prelievi ad un determinato account
void addAccTransaction() {
    std::string user;
    bool ok;

    clearConsole();
    do {
        //faccio inserire lo username
        ok = false;
        bool found = false;
        std::cout << "Enter username: ";
        std::cin >> user;

        //se si prova ad aggiungere transazioni all'admin, allora fermo l'esecuzione e chiedo se si vuole ritentare
        if (user == "admin") {
            std::string abort;
            clearConsole();
            std::cout << "\033[31mYou cannot add transactions to admin account!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            //cerco l'account con quel username
            for (auto& account : accounts) {
                if (account.getName() == user) {
                    found = true;
                    int type;
                    double amount;
                    std::string description;

                    //faccio scegliere la tipologia di transazione (deposito o prelievo)
                    do {
                        std::cout << "Would you like to add a \033[32mdeposit\033[0m (1) or a \033[31mwithdrawal\033[0m (2)? ";
                        std::cin >> type;

                        //se viene inserito un carattere diverso dal numero, resetto il buffer input e chiedo all'utente se vuole ritentare
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                            std::string abort;
                            clearConsole();
                            std::cout << "\033[31mYou entered a non-valid option!\033[0m Would you like to try again? (type 'n' to abort) ";
                            std::cin >> abort;
                            if (caseInsensitiveCompare(abort, "n")) {
                                clearConsole();
                                return;
                            }
                        //se l'utente non ha scelto tra le due opzioni, lo avviso e gli chiedo se vuole ritentare
                        } else if (type != 1 && type != 2) {
                            std::string abort;
                            clearConsole();
                            std::cout << "\033[31mYou entered a non-valid option!\033[0m Would you like to try again? (type 'n' to abort) ";
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

                    //faccio inserire la somma
                    do {
                        std::cout << "Please add an amount: ";
                        std::cin >> amount;

                        //se l'utente inserisce caratteri diversi dai numeri, resetto il buffer input e chiedo all'utente se vuole ritentare
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
                        //se l'utente inserisce un importo negativo, lo avviso e gli chiedo se vuole ritentare
                        } else if (amount <= 0) {
                            std::string abort;
                            clearConsole();
                            std::cout << "\033[31mYou must enter a positive number!\033[0m Would you like to try again? (type 'n' to abort) ";
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

                    //viene aggiunta una descrizione per la transazione
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Please add a description: ";
                    std::getline(std::cin, description);

                    //aggiungo e salvo la transazione
                    account.addTransaction(Transaction((type == 1 ? Transaction::DEPOSIT : Transaction::WITHDRAWAL), amount, description));
                    account.saveTransactions();

                    std::cout << std::endl << "Transaction added to " << account.getName() << "!" << std::endl
                              << (type==1 ? "\033[32mDeposit\033[0m" : "\033[31mWithdrawal\033[0m") << " - " << amount
                              << " \033[93mEuro\033[0m - \033[94m" << description << "\033[0m" << std::endl;

                    //propongo all'utente se vuole inserire nuove transazioni
                    std::string c;
                    std::cout << "Would you like to add other transactions? (type 'y' to continue) ";
                    std::cin >> c;
                    if (!caseInsensitiveCompare(c, "y")) ok = true;
                    clearConsole();
                    break;
                }
            }

            //se non trovo utenti, avviso l'utente e chiedo se vuole ritentare
            if (!found) {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mNo username found!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            }
        }
    } while (!ok);

    clearConsole();
}

//visualizza a schermo le transazioni di uno specifico account o di tutti gli account
void viewTransactions() {
    std::string selection;
    clearConsole();

    //chiedo all'utente se vuole visualizzare tutte le transazioni di un account o di tutti gli account
    std::cout << "Do you want to print ALL accounts history or specific ones? (type 'a' to print all) ";
    std::cin >> selection;

    //se si desidera stampare tutti, allora percorro la List di accounts e le loro transazioni e le stampo
    if (caseInsensitiveCompare(selection, "a")) {
        clearConsole();
        for (const auto& account : accounts) {
            if (account.getName() != "admin") {
                std::cout << "Account name: " << account.getName() << std::endl;
                for (const auto &transaction: account.getTransactions()) {
                    std::cout << transaction.toString() << std::endl;
                }
                if (account.getTransactions().empty()) std::cout << "\033[33mNo transactions made.\033[0m" << std::endl;
                else {
                    std::cout << "Account balance: ";
                    if (account.getBalance() <= 0) std::cout << "\033[31m" << account.getBalance() << " Euro\033[0m" << std::endl;
                    else std::cout << "\033[32m" << account.getBalance() << " Euro\033[0m" << std::endl;
                }
                std::cout << std::endl;
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Press Enter to continue... ";
        std::getline(std::cin, selection);
        clearConsole();
    //sennò...
    } else {
        std::string name;
        bool cycle = true;

        //chiedo all'utente di inserire il nome dell'account
        do {
            std::cout << "Enter account name: ";
            std::cin >> name;

            //verifico non sia stato inserito il nome admin
            if (!caseInsensitiveCompare(name, "admin")) {
                bool found = false;
                for (const auto &account: accounts) {
                    //cerco l'account...
                    if (caseInsensitiveCompare(account.getName(), name)) {
                        found = true;
                        //...e se lo trovo allora procedo con la stampa di tutte le transazioni
                        for (const auto &transaction: account.getTransactions()) {
                            std::cout << transaction.toString() << std::endl;
                        }
                        if (account.getTransactions().empty()) std::cout << "\033[33mNo transactions made.\033[0m" << std::endl;
                        else {
                            std::cout << "Account balance: ";
                            if (account.getBalance() <= 0) std::cout << "\033[31m" << account.getBalance() << " Euro\033[0m" << std::endl;
                            else std::cout << "\033[32m" << account.getBalance() << " Euro\033[0m" << std::endl;
                        }
                        std::cout << std::endl;
                        break;
                    }
                }
                //se non trovo l'account, avviso l'utente
                if (!found) std::cout << "\033[31mNo account found with that name!\033[0m ";

                //chiedo all'utente se vuole fare altre stampe di altri utenti
                std::string choice;
                std::cout << "Would you like to check other accounts? (type 'y' and Enter to continue) ";
                std::cin >> choice;
                if (!caseInsensitiveCompare(choice, "y")) cycle = false;
                clearConsole();

            //se si tenta di inserire il nome "admin", blocco l'operazione e gli chiedo se vuole ritentare
            } else {
                std::string abort;
                clearConsole();
                std::cout << "\033[31mYou cannot see transactions to admin account!\033[0m Would you like to try again? (type 'n' to abort) ";
                std::cin >> abort;
                if (caseInsensitiveCompare(abort, "n")) {
                    clearConsole();
                    return;
                }
            }
        } while (cycle);
    }
}

//cambia la password dell'account amministratore
void changeAdminPw() {
    int pwd, newPwd;
    bool ok = true;
    clearConsole();

    //l'utente immette la vecchia password
    do {
        std::cout << "Enter current password: ";
        std::cin >> pwd;

        //se vengono inseriti caratteri diversi da numeri, resetto il buffer input e chiedo all'utente se vuole ritentare
        if (std::cin.fail()) {
            ok = false;
            std::string abort;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            clearConsole();
            std::cout << "\033[31mUsername not found or wrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
            std::cin >> abort;
            if (caseInsensitiveCompare(abort, "n")) {
                clearConsole();
                return;
            }
        } else {
            for (auto& account : accounts) {
                //verifico se la password è corretta
                if (account.getName() == "admin" && account.getPassword() == pwd) {
                    //se corretta, chiedo la nuova password
                    do {
                        ok = false;

                        std::cout << "Enter new password \033[31m(numbers only)\033[0m: ";
                        std::cin >> newPwd;

                        //se vengono inseriti caratteri diversi da numeri, resetto il buffer input e avviso l'utente
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            clearConsole();
                            std::cout << "\033[31mPassword must contain numbers only!\033[0m" << std::endl;
                        } else {
                            //salvo la nuova password sia nella List che in accounts.txt
                            account.setPassword(newPwd);
                            Account::saveAccounts(accounts);
                            ok = true;

                            clearConsole();
                            std::cout << "\033[32mPassword changed! New password: " << newPwd << "\033[0m" << std::endl;
                        }
                    } while (!ok);
                    break;

                //se non corretta, allora propongo all'utente se vuole ritentare
                } else if (account.getName() == "admin" && account.getPassword() != pwd) {
                    ok = false;
                    std::string abort;
                    clearConsole();
                    std::cout << "\033[31mWrong password!\033[0m Would you like to try again? (type 'n' to abort) ";
                    std::cin >> abort;
                    if (caseInsensitiveCompare(abort, "n")) {
                        clearConsole();
                        return;
                    }
                    break;
                }
            }
        }
    } while (!ok);
}

//menu amministratore
bool adminMenu() {
    int selection;

    clearConsole();

    //stampa menu
    do {
        std::cout << "Welcome \033[93madmin\033[0m" << std::endl << std::endl;
        std::cout << "\033[36m----- Main menu -----\033[0m" << std::endl;
        std::cout << "\033[32m1 -\033[0m Add account" << std::endl;
        std::cout << "\033[32m2 -\033[0m Remove account" << std::endl;
        std::cout << "\033[32m3 -\033[0m View all accounts" << std::endl;
        std::cout << "\033[32m4 -\033[0m Change an account's password" << std::endl;
        std::cout << "\033[32m5 -\033[0m Add a transaction on an account" << std::endl;
        std::cout << "\033[32m6 -\033[0m View transaction history on one or all accounts" << std::endl;
        std::cout << "\033[32m7 -\033[0m Change your password" << std::endl;
        std::cout << "\033[32m8 -\033[0m Log out" << std::endl;
        std::cout << "\033[32m9 -\033[0m Log out and return to OS" << std::endl;
        std::cout << "Enter your selection: ";
        std::cin >> selection;

        //verifico la selezione scelta dall'utente e chiamo la funzione rispettiva
        switch (selection) {
            case 1:
                addAccount();
                break;
            case 2:
                removeAccount();
                break;
            case 3:
                viewAccounts();
                break;
            case 4:
                changeAccPw();
                break;
            case 5:
                addAccTransaction();
                break;
            case 6:
                viewTransactions();
                break;
            case 7:
                changeAdminPw();
                break;
            case 8:
                return true;
            case 9:
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

#endif //NEAMTU_FINANCES_ADMINMENU_H