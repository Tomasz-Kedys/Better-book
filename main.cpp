#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <windows.h>

using namespace std;

struct User {
    int id = 0;
    string login = "", password = "";
};

struct Recipient {
    int id = 0, userId = 0;
    string name = "", surname = "", phoneNumber = "", email = "", address = "";
};

string readTheLine() {
    string entryData;
    getline (cin,entryData);
    return entryData;
}

int makeNubersNotWords (string lineOfDataSub) {
    int numberIsId = 0;
    int numberLength = lineOfDataSub.size();
    int positionOfDidgit = 0;
    if (numberLength == 1) {
        numberIsId = lineOfDataSub[positionOfDidgit] - '0';
    } else {
        while (positionOfDidgit < numberLength) {
            numberIsId += lineOfDataSub[positionOfDidgit] - '0';
            numberIsId *= 10;
            positionOfDidgit++;
        }
        numberIsId /= 10;
    }
    return numberIsId;
}

string makeWordsNotNumbers (int number) {
    string stringNumber = "";
    stringNumber = to_string (number);
    return stringNumber;
}

void splitUserData (string lineOfData, vector <User> &user, string login) {

    string lineOfDataSub = "";
    int rowOfData = 0;
    User person;
    for (int i = 0; i < (int) lineOfData.size(); i++) {
        if (lineOfData[i] != '|') {
            lineOfDataSub.push_back (lineOfData[i]);
        } else if (lineOfData[i] == '|') {
            if (rowOfData == 0) {
                int idOfAPerson = makeNubersNotWords (lineOfDataSub);
                person.id = idOfAPerson;
                lineOfDataSub.erase();
            } else if (rowOfData == 1) {
                person.login = lineOfDataSub;
                lineOfDataSub.erase();
            } else if (rowOfData == 2) {
                person.password = lineOfDataSub;
                lineOfDataSub.erase();
            }
            rowOfData++;
        }
    }

    user.push_back (person);
    rowOfData = 0;
}

void openUserDataFile (vector <User> &user, string login) {

    string lineOfData = "";
    fstream myFile;

    myFile.open ("Users.txt",ios::app);
    myFile.close();

    myFile.open ("Users.txt",ios::in);

    while (getline (myFile,lineOfData) ) {
        splitUserData (lineOfData, user, login);
    }
    myFile.close();
}

int login (vector <User> user) {

    string login, pass;
    int amountOfUsers = 0;

    cout << "Podaj login: ";
    cin >> login;

    openUserDataFile (user, login);
    amountOfUsers = user.size();

    int i = 0;
    while (i < amountOfUsers) {
        if (user[i].login == login) {
            for (int j=0; j<3 ; j++) {
                cout << "Podaj haslo, pozostalo prob " << 3-j << ": ";
                cin >> pass;
                if (user[i].password == pass) {
                    cout << "Zalogowales sie." << endl;
                    Sleep (1000);
                    return user[i].id;
                }
            }
            cout << "poczekaj 5 sekund, niepoprawne haslo zostalo wpisane 3 razy." << endl;
            Sleep (5000);
            return 0;
        }
        i++;
    }
    cout << "Nie ma takiego uzytkownika." << endl;
    Sleep (1500);
    return 0;

}

int readTheLastUserId (vector <User> user) {
    int lastId = 0;
    if (!user.size() == 0) {
        int endOfVector = user.size()-1;
        lastId = user[endOfVector].id;
    }
    return lastId;
}

int readTheLastRecipientId (vector <Recipient> recipient) {
    int lastId = 0;
    if (!recipient.size() == 0) {
        int endOfVector = recipient.size()-1;
        lastId = recipient[endOfVector].id;
    }
    return lastId;
}

void saveUserData (vector <User> &user, int indexOfPerson, int caseOfExecution) {

    const int AMOUNT_OF_LOOPS = 3;
    fstream myFile;
    string personsData = "";
    int indexOfData = 0;


    for (int i = 0; i < AMOUNT_OF_LOOPS; i++) {
        if (indexOfData == 0) {
            string id = makeWordsNotNumbers (user[indexOfPerson].id);
            personsData.append (id);
        } else if (indexOfData == 1) {
            personsData.append (user[indexOfPerson].login);
        } else if (indexOfData == 2) {
            personsData.append (user[indexOfPerson].password);
        }
        personsData.push_back ('|');
        indexOfData++;
    }

    if (caseOfExecution == 1) {
        myFile.open ("Users.txt",ios::app);
        myFile << personsData << endl;
        myFile.close();
    } else if (caseOfExecution == 2) {
        if (indexOfPerson == 0) {
            myFile.open ("Users.txt",ios::out);
            myFile.close();
        }
        myFile.open ("Users.txt",ios::app);
        myFile << personsData << endl;
        myFile.close();
    } else {
        cout << "Nic sie nie stalo, zly kod zapisu do funkcji" << endl;
    }
}

vector <User> registerNewUser (vector <User> &user) {

    string login = "", password = "";
    int amountOfUsers = user.size();
    int lastId = readTheLastUserId (user);
    User newUser;

    cout << "Podaj nazwe uzytkownika: ";
    cin >> login;
    int i = 0;
    while (i < amountOfUsers) {
        if (user[i].login == login) {
            cout << "Taki uzytkownik istnieje, wpisz inna nazwe uzytkownika: ";
            cin >> login;
            i = 0;
        } else {
            i++;
        }
    }
    cout << "Podaj haslo: ";
    cin >> password;
    newUser.login = login;
    newUser.password = password;
    newUser.id = lastId+1;
    user.push_back (newUser);
    saveUserData (user, user.size()-1, 1);
    cout << "Konto zalozone pomyslnie" << endl;
    Sleep (1000);

    return user;
}

void splitRecipientData (string lineOfData,vector <Recipient> &recipient, int loggedInUserId) {

    string lineOfDataSub = "";
    int rowOfData = 0;
    Recipient person;
    for (int i = 0; i < (int) lineOfData.size(); i++) {
        if (lineOfData[i] != '|') {
            lineOfDataSub.push_back (lineOfData[i]);
        } else if (lineOfData[i] == '|') {
            if (rowOfData == 0) {
                int idOfAPerson = makeNubersNotWords (lineOfDataSub);
                person.id = idOfAPerson;
                lineOfDataSub.erase();
            } else if (rowOfData == 1) {
                int userId = makeNubersNotWords (lineOfDataSub);
                person.userId = userId;
                lineOfDataSub.erase();
            } else if (rowOfData == 2) {
                person.name = lineOfDataSub;
                lineOfDataSub.erase();
            } else if (rowOfData == 3) {
                person.surname = lineOfDataSub;
                lineOfDataSub.erase();
            } else if (rowOfData == 4) {
                person.phoneNumber = lineOfDataSub;
                lineOfDataSub.erase();
            } else if (rowOfData == 5) {
                person.email = lineOfDataSub;
                lineOfDataSub.erase();
            } else if (rowOfData == 6) {
                person.address = lineOfDataSub;
                lineOfDataSub.erase();
            }
            rowOfData++;
        }
    }

    if (person.userId == loggedInUserId) {
        recipient.push_back (person);
    }
    rowOfData = 0;
}

void openRecipientDataFile (vector <Recipient> &recipient, int loggedInUserId) {

    string lineOfData = "";
    fstream myFile;

    myFile.open ("Recipients.txt",ios::app);
    myFile.close();

    myFile.open ("Recipients.txt",ios::in);

    while (getline (myFile,lineOfData) ) {
        splitRecipientData (lineOfData, recipient, loggedInUserId);
    }
    myFile.close();
}

void writeAllRecipientsOut (vector <Recipient> recipient) {
    system ( "cls" );
    int amountOfRecipients = recipient.size();
    cout << "Adresaci : " << amountOfRecipients << endl << endl;
    for (int i = 0; i < amountOfRecipients; i++) {
        cout << left << setw (10) << "ID uzytkownika: " << recipient[i].userId << endl;
        cout << left << setw (10) << "ID osoby: "       << recipient[i].id << endl;
        cout << left << setw (10) << "Imie: "           << recipient[i].name << endl;
        cout << left << setw (10) << "Nazwisko: "       << recipient[i].surname<< endl;
        cout << left << setw (10) << "Nr. Tel: "        << recipient[i].phoneNumber << endl;
        cout << left << setw (10) << "Email: "          << recipient[i].email << endl;
        cout << left << setw (10) << "Adres: "          << recipient[i].address << endl << endl;
    }
    system ("pause");
}

void findName (vector <Recipient> recipient) {
    string searchedName = "";
    bool foundTheName = false;
    int amountOfRecipients = recipient.size();
    cout << "Podaj imie do wyszukania : ";
    getchar();
    searchedName = readTheLine();

    system ( "cls" );
    for (int i = 0; i < amountOfRecipients; i++) {
        if (searchedName == recipient[i].name) {
            foundTheName = true;
            cout << left << setw (10) << "ID osoby: " << recipient[i].id << endl;
            cout << left << setw (10) << "Imie: "     << recipient[i].name << endl;
            cout << left << setw (10) << "Nazwisko: " << recipient[i].surname<< endl;
            cout << left << setw (10) << "Nr. Tel: "  << recipient[i].phoneNumber << endl;
            cout << left << setw (10) << "Email: "    << recipient[i].email << endl;
            cout << left << setw (10) << "Adres: "    << recipient[i].address << endl << endl;
        }
    }
    if (foundTheName == false) {
        cout << "Nie znaleziono podanego imienia!!" << endl;
    }
    system ("pause");
}

void findSurname (vector <Recipient> recipient) {
    string searchedSurname = "";
    bool foundTheSurname = false;
    int amountOfRecipients = recipient.size();
    cout << "Podaj nazwisko do wyszukania : ";
    getchar();
    searchedSurname = readTheLine();

    system ( "cls" );
    for (int i = 0; i < amountOfRecipients; i++) {
        if (searchedSurname == recipient[i].surname) {
            foundTheSurname = true;
            cout << left << setw (10) << "ID osoby: " << recipient[i].id << endl;
            cout << left << setw (10) << "Imie: "     << recipient[i].name << endl;
            cout << left << setw (10) << "Nazwisko: " << recipient[i].surname<< endl;
            cout << left << setw (10) << "Nr. Tel: "  << recipient[i].phoneNumber << endl;
            cout << left << setw (10) << "Email: "    << recipient[i].email << endl;
            cout << left << setw (10) << "Adres: "    << recipient[i].address << endl << endl;
        }
    }
    if (foundTheSurname == false) {
        cout << "Nie znaleziono podanego nazwiska!!" << endl;
    }
    system ("pause");
}

void passwordChange (vector <User> &user, int loggedInUserId) {
    string password;
    char confirmation;
    int amountOfUsers = user.size();
    cout << "Podaj nowe haslo: ";
    cin >> password;
    for (int i = 0; i < amountOfUsers; i++) {
        if (user[i].id == loggedInUserId) {
            cout << "Czy na pewno chcesz zmienic haslo [T/N] : ";
            cin >> confirmation;
            if (confirmation == 'T') {
                user[i].password = password;
                cout << "Haslo zostalo zmienione" << endl;
                Sleep (1500);
            } else if (confirmation == 'N') {
                cout << "Haslo nie zostalo zmienione" << endl;
                Sleep (1500);
            } else {
                cout << "Nieprawidlowy znak " << endl;
                break;
            }
        }
    }

    for (int i = 0; i < (int) user.size(); i++) {
        saveUserData (user, i, 2);
    }
}

vector <Recipient> getDataFromUser (vector <Recipient> &recipient, int loggedInUserId) {

    string name="", surname="", phoneNumber="", email="", address="";
    Recipient newPerson;
    int lastId = readTheLastRecipientId (recipient);

    system ( "cls" );
    cout << "Podaj imie: ";
    cin >> name;

    cout << "Podaj nazwisko: ";
    cin >> surname;

    cout << "Podaj numer telefonu: ";
    cin.sync();
    phoneNumber = readTheLine();

    cout << "Podaj email: ";
    email = readTheLine();

    cout << "Podaj adres: ";
    address = readTheLine();

    newPerson.id = lastId+1;
    newPerson.userId = loggedInUserId;
    newPerson.name = name;
    newPerson.surname = surname;
    newPerson.phoneNumber = phoneNumber;
    newPerson.email = email;
    newPerson.address = address;

    recipient.push_back (newPerson);

    return recipient;
}

void saveRecipientData (vector <Recipient> recipient, int indexOfPerson, int loggedInUserId) {

    const int AMOUNT_OF_LOOPS = 7;
    fstream myFile;
    string personsData = "";
    int indexOfData = 0;

    for (int i = 0; i < AMOUNT_OF_LOOPS; i++) {
        if (indexOfData == 0) {
            string id = makeWordsNotNumbers (recipient[indexOfPerson].id);
            personsData.append (id);
        } else if (indexOfData == 1) {
            string userId = makeWordsNotNumbers (loggedInUserId);
            personsData.append (userId);
        } else if (indexOfData == 2) {
            personsData.append (recipient[indexOfPerson].name);
        } else if (indexOfData == 3) {
            personsData.append (recipient[indexOfPerson].surname);
        } else if (indexOfData == 4) {
            personsData.append (recipient[indexOfPerson].phoneNumber);
        } else if (indexOfData == 5) {
            personsData.append (recipient[indexOfPerson].email);
        } else if (indexOfData == 6) {
            personsData.append (recipient[indexOfPerson].address);
        }
        personsData.push_back ('|');
        indexOfData++;
    }

    myFile.open ("Recipients_temp.txt",ios::app);
    myFile << personsData << endl;
    myFile.close();
}

void saveTheFile (vector <Recipient> recipient, int loggedInUserId) {

    fstream orgFile, newFile;
    string orgData = "", newData = "", lineOfDataSub = "";
    int orgDataUserId = 0, j = 0;

    orgFile.open ("Recipients.txt",ios::in);
    newFile.open ("Recipients_temp.txt",ios::app);

    while (getline (orgFile, orgData) ) {

        int rowOfData = 0;
        for (int i = 0; i < (int) orgData.size(); i++) {
            if (orgData[i] != '|') {
                lineOfDataSub.push_back (orgData[i]);
            } else if (orgData[i] == '|') {
                if (rowOfData == 0) {
                    lineOfDataSub.erase();
                } else if (rowOfData == 1) {
                    orgDataUserId = makeNubersNotWords (lineOfDataSub);
                    lineOfDataSub.erase();
                }
                rowOfData++;
            }
        }
        if (orgDataUserId == loggedInUserId && (int) recipient.size() > j) {
            saveRecipientData (recipient, j, loggedInUserId);
            j++;
        } else if (orgDataUserId != loggedInUserId) {
            newFile << orgData << endl;
        }
    }
    orgFile.close();
    newFile.close();
    remove ("Recipients.txt");
    rename ("Recipients_temp.txt","Recipients.txt");
}

void addNewRecipient (vector <Recipient> &recipient, int loggedInUserId) {

    int index = recipient.size();
    recipient = getDataFromUser (recipient, loggedInUserId);
    saveTheFile (recipient, loggedInUserId);

    system ("cls");
    cout << left << setw (10) << "Dodano nowa osobe!!" << endl;
    cout << left << setw (10) << "ID osoby: "       << recipient[index].id << endl;
    cout << left << setw (10) << "ID uzytkownika: " << recipient[index].userId << endl;
    cout << left << setw (10) << "Imie: "           << recipient[index].name << endl;
    cout << left << setw (10) << "Nazwisko: "       << recipient[index].surname<< endl;
    cout << left << setw (10) << "Nr. Tel: "        << recipient[index].phoneNumber << endl;
    cout << left << setw (10) << "Email: "          << recipient[index].email << endl;
    cout << left << setw (10) << "Adres: "          << recipient[index].address << endl << endl;
    system ("pause");
}

bool findTheGivenId (int idToKill, vector <Recipient> recipient) {
    for (int i = 0; i < (int) recipient.size(); i++) {
        if (recipient[i].id == idToKill) {
            return true;
        }
    }
    return false;
}

void deleteRecipient (vector <Recipient> &recipient, int loggedInUserId) {

    int idToKill = 0;
    char confirmingChar;
    cout << "Wypisac wszystkie osoby? [T/N] : ";
    cin >> confirmingChar;
    if (confirmingChar == 'T') {
        writeAllRecipientsOut (recipient);
    }
    cout << "Podaj ID osoby ktora chesz usunac z ksiazki adresowej : ";
    cin >> idToKill;

    if (!findTheGivenId (idToKill, recipient) ) {
        cout << "Nie znaleziono podanego ID: " << idToKill << endl;
    } else {
        cout << "Napewno chcesz usunac adresata?" << endl;
        cout << "dla potwierdzenia wpisz 'T': ";
        cin >> confirmingChar;
        if (confirmingChar == 'T') {
            for (int i = 0; i < (int) recipient.size(); i++) {
                if (recipient[i].id == idToKill) {
                    recipient.erase (recipient.begin()+i);
                }
            }
            saveTheFile (recipient, loggedInUserId);
        }
    }
    cin.clear();
    system ("pause");
}

int positionOfPersonInFile (vector <Recipient> recipient, int idToEdit) {
    int i;
    for (i = 0; i < (int) recipient.size(); i++) {
        if (recipient[i].id == idToEdit) {
            break;
        }
    }
    return i;
}

void changeTheName (vector <Recipient> &recipient, int positionOfPerson) {
    string newName = "";
    cout << "Podaj nowe imie: ";
    cin >> newName;

    recipient[positionOfPerson].name = newName;
    cout << "Pomylsnie zmieniono imie." << endl;
    system ("pause");
}

void changeTheSurname (vector <Recipient> &recipient, int positionOfPerson) {
    string newSurname = "";
    cout << "Podaj nowe nazwisko: ";
    cin >> newSurname;

    recipient[positionOfPerson].surname = newSurname;
    cout << "Pomylsnie zmieniono nazwisko." << endl;
    system ("pause");
}

void changeThePhoneNumber (vector <Recipient> &recipient, int positionOfPerson) {
    string newPhoneNumber = "";
    cout << "Podaj nowy numer telefonu: ";
    getchar();
    newPhoneNumber = readTheLine();

    recipient[positionOfPerson].phoneNumber = newPhoneNumber;
    cout << "Pomylsnie zmieniono nr. telefonu." << endl;
    system ("pause");
}

void changeTheEmail (vector <Recipient> &recipient, int positionOfPerson) {
    string newEmail = "";
    cout << "Podaj nowy e-mail: ";
    cin >> newEmail;

    recipient[positionOfPerson].email = newEmail;
    cout << "Pomylsnie zmieniono email." << endl;
    system ("pause");
}

void changeTheAddress (vector <Recipient> &recipient, int positionOfPerson) {
    string newAddress = "";
    cout << "Podaj nowy adres: ";
    getchar();
    newAddress = readTheLine();

    recipient[positionOfPerson].address = newAddress;
    cout << "Pomylsnie zmieniono adres." << endl;
    system ("pause");
}

void editRecipient (vector <Recipient> &recipient, int loggedInUserId) {

    char menuOptions;
    int idToEdit = 0;
    int positionOfPerson = 0;
    cout << "Podaj ID osoby ktorej informacje chesz edytowac: ";
    cin >> idToEdit;

    if (!findTheGivenId (idToEdit, recipient) ) {
        cout << "Nie znaleziono podanego ID: " << idToEdit << endl;
    } else {
        positionOfPerson = positionOfPersonInFile (recipient, idToEdit);
        while (1) {
            system ( "cls" );
            cout << "1. imie" << endl;
            cout << "2. nazwisko" << endl;
            cout << "3. numer telefonu" << endl;
            cout << "4. email" << endl;
            cout << "5. adres" << endl;
            cout << "0. powrut do menu" << endl;
            cout << "Podaj wybor : ";
            menuOptions = getchar();

            switch (menuOptions) {
            case '1':
                changeTheName (recipient, positionOfPerson);
                break;
            case '2':
                changeTheSurname (recipient, positionOfPerson);
                break;
            case '3':
                changeThePhoneNumber (recipient, positionOfPerson);
                break;
            case '4':
                changeTheEmail (recipient, positionOfPerson);
                break;
            case '5':
                changeTheAddress (recipient, positionOfPerson);
                break;
            case '0':
                saveTheFile (recipient, loggedInUserId);
                return;
            }
        }
    }
    cin.clear();
    system ("pause");
}

int main() {

    int loggedInUserId = 0;
    int tag = 0;
    char menuOptions;

    vector <Recipient> recipient;
    vector <User> user;

    while (1) {
        if (loggedInUserId == 0) {

            system ( "cls" );
            cout << "1. Logowanie" << endl;
            cout << "2. Rejestracja" << endl;
            cout << "0. Wyjdz z programu" << endl;
            cout << "Podaj wybor : ";
            cin >> menuOptions;

            switch (menuOptions) {
            case '1':
                loggedInUserId = login (user);
                break;
            case '2':
                user = registerNewUser (user);
                break;
            case '0':
                exit (0);
                break;
            }
        } else {

            if (tag == 0) {
                openRecipientDataFile (recipient, loggedInUserId);
                tag++;
            }
            system ( "cls" );
            cout << "1. Dodaj adresata" << endl;
            cout << "2. Wyszukanie po imieniu" << endl;
            cout << "3. Wyszukanie po nazwisku" << endl;
            cout << "4. Wypisanie wszystkich adresatow" << endl;
            cout << "5. Usun adresata" << endl;
            cout << "6. Edytuj adresata" << endl;
            cout << "7. Zmien haslo" << endl;
            cout << "0. Wyloguj" << endl;
            cout << "Podaj wybor : ";
            menuOptions = getchar();

            switch (menuOptions) {
            case '1':
                addNewRecipient (recipient, loggedInUserId);
                break;
            case '2':
                findName (recipient);
                break;
            case '3':
                findSurname (recipient);
                break;
            case '4':
                writeAllRecipientsOut (recipient);
                break;
            case '5':
                deleteRecipient (recipient, loggedInUserId);
                break;
            case '6':
                editRecipient (recipient, loggedInUserId);
                break;
            case '7':
                passwordChange (user, loggedInUserId);
                break;
            case '0':
                loggedInUserId = 0;
                tag = 0;
                recipient.clear();
                break;
            }
        }
    }
    return 0;
}
