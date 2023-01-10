#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <windows.h>
#include <algorithm>

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
    cin.sync();
    return entryData;
}

string changeFirstLetterToCapitalLetter (string word) {
    if (!word.empty() ) {
        transform (word.begin(), word.end(), word.begin(), ::tolower);
        word[0] = toupper (word[0]);
    }
    return word;
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

int readTheLastUserId (vector <User> user) {
    int lastId = 0;
    if (!user.size() == 0) {
        int endOfVector = user.size()-1;
        lastId = user[endOfVector].id;
    }
    return lastId;
}

int readTheLastRecipientId () {

    int lastId = 0, dataRow = 0;
    fstream myFile;
    string dataLine = "", data = "";

    myFile.open ("Recipients.txt",ios::in);

    while (getline (myFile, dataLine) ) {
        for (int i = 0; i < (int) dataLine.size(); i++) {
            if (dataLine[i] != '|') {
                data.push_back (dataLine[i]);
            } else if (dataLine[i] == '|') {
                if (dataRow == 0) {
                    lastId = makeNubersNotWords (data);
                }
                data.erase();
                dataRow++;
            }
        }
        dataRow = 0;
    }
    myFile.close();
    return lastId;
}

int readTheId (string lineOfRecipientData) {
    string recipientId;

    for (int i = 0; i < (int) lineOfRecipientData.size(); i++) {
        if (lineOfRecipientData[i] != '|') {
            recipientId.push_back (lineOfRecipientData[i]);
        } else {
            cout << "recipient Id: " << recipientId << endl;
            system("pause");
            return makeNubersNotWords (recipientId);
        }
    }
    return 0;
}

void splitUserData (string lineOfData, vector <User> &user) {

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

vector <User> openUserDataFile (vector <User> &user) {

    string lineOfData = "";
    fstream myFile;

    myFile.open ("Users.txt",ios::app);
    myFile.close();

    myFile.open ("Users.txt",ios::in);

    while (getline (myFile,lineOfData) ) {
        splitUserData (lineOfData, user);
    }
    myFile.close();
    return user;
}

int login (vector <User> &user) {

    string login, pass;
    int amountOfUsers = 0;

    cout << "Podaj login: ";
    cin >> login;

    amountOfUsers = user.size();

    int i = 0;

    while (i < amountOfUsers) {
        if (user[i].login == login) {
            for (int j=0; j<3 ; j++) {
                cout << "Podaj haslo, pozostalo prob " << 3-j << ": ";
                cin >> pass;
                if (user[i].password == pass) {
                    cout << "Zalogowales sie." << endl;
                    system ("pause");
                    return user[i].id;
                }
            }
            cout << "poczekaj 5 sekund, niepoprawne haslo zostalo wpisane 3 razy." << endl;
            system ("pause");
            return 0;
        }
        i++;
    }
    cout << "Nie ma takiego uzytkownika." << endl;
    system ("pause");
    return 0;

}

void saveUserData (User newUser) {
    fstream userDataFile;
    userDataFile.open ("Users.txt",ios::out | ios::app);
    if (userDataFile.good() == true) {
        userDataFile << newUser.id << '|';
        userDataFile << newUser.login << '|';
        userDataFile << newUser.password << '|' << endl;
        userDataFile.close();

        cout << endl << "Dane zapisane pomyslnie" << endl;
        system ("pause");
    } else {
        cout << "Nie udlao sie zapisac danych!!" << endl;
        system ("pause");
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
    saveUserData (newUser);
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

vector <Recipient> openRecipientDataFile (vector <Recipient> recipient, int loggedInUserId) {

    string lineOfData = "";
    fstream myFile;

    myFile.open ("Recipients.txt",ios::app);
    myFile.close();

    myFile.open ("Recipients.txt",ios::in);

    while (getline (myFile,lineOfData) ) {
        splitRecipientData (lineOfData, recipient, loggedInUserId);
    }
    myFile.close();
    return recipient;
}

void addNewRecipientToFile ( Recipient newPerson) {
    fstream recipientDataFile;
    recipientDataFile.open ("Recipients.txt", ios::out |ios::app);

    if (recipientDataFile.good() == true) {
        recipientDataFile << newPerson.id << "|";
        recipientDataFile << newPerson.userId << "|";
        recipientDataFile << newPerson.name << "|";
        recipientDataFile << newPerson.surname << "|";
        recipientDataFile << newPerson.phoneNumber << "|";
        recipientDataFile << newPerson.email << "|";
        recipientDataFile << newPerson.address << "|" << endl;
        recipientDataFile.close();

        cout << left << setw (10) << "Dodano nowa osobe!!" << endl;
    } else {
        cout << "Nie udalo sie otworzyc pliku i dopisac nowego adresata." << endl;
        system ("pause");
    }
}

vector <Recipient> getDataFromUser (vector <Recipient> &recipient, int loggedInUserId) {

    string name="", surname="", phoneNumber="", email="", address="";
    Recipient newPerson;
    int lastId = readTheLastRecipientId ();

    system ( "cls" );
    cout << "Podaj imie: ";
    name = changeFirstLetterToCapitalLetter (readTheLine() );

    cout << "Podaj nazwisko: ";
    surname = changeFirstLetterToCapitalLetter (readTheLine() );

    cout << "Podaj numer telefonu: ";
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
    addNewRecipientToFile (newPerson);
    return recipient;
}

void addNewRecipient (vector <Recipient> &recipient, int loggedInUserId) {

    system ("cls");
    int index = recipient.size();
    recipient = getDataFromUser (recipient, loggedInUserId);

    cout << left << setw (10) << "ID osoby: "       << recipient[index].id << endl;
    cout << left << setw (10) << "ID uzytkownika: " << recipient[index].userId << endl;
    cout << left << setw (10) << "Imie: "           << recipient[index].name << endl;
    cout << left << setw (10) << "Nazwisko: "       << recipient[index].surname<< endl;
    cout << left << setw (10) << "Nr. Tel: "        << recipient[index].phoneNumber << endl;
    cout << left << setw (10) << "Email: "          << recipient[index].email << endl;
    cout << left << setw (10) << "Adres: "          << recipient[index].address << endl << endl;
    system ("pause");
}

void findName (vector <Recipient> recipient) {
    string searchedName = "";
    bool foundTheName = false;
    int amountOfRecipients = recipient.size();
    cout << "Podaj imie do wyszukania : ";
    searchedName = changeFirstLetterToCapitalLetter (readTheLine() );

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
    searchedSurname = changeFirstLetterToCapitalLetter (readTheLine() );

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

void writeAllRecipientsOut (vector <Recipient> recipient) {
    system ( "cls" );
    int amountOfRecipients = recipient.size();
    cout << "Ilosc adresatow : " << amountOfRecipients << endl << endl;
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

bool findTheGivenId (int idToKill, vector <Recipient> recipient) {
    for (int i = 0; i < (int) recipient.size(); i++) {
        if (recipient[i].id == idToKill) {
            return true;
        }
    }
    return false;
}

void saveDataAfterDeletion (vector <Recipient> &recipient, int idToKill) {
    string data = "";
    fstream tempFile, orginalFile;

    orginalFile.open ("Recipients.txt", ios::in);
    tempFile.open ("temp.txt",ios::out);

    while (getline (orginalFile, data) ) {
        if (readTheId (data) != idToKill) {
            tempFile << data << endl;
        }
    }

    orginalFile.close();
    tempFile.close();

    remove ("Recipients.txt");
    rename ("temp.txt","Recipients.txt");
}

void deleteRecipient (vector <Recipient> &recipient, int loggedInUserId) {

    int idToKill = 0;
    char confirmingChar;
    cout << "Wypisac wszystkie osoby? [T/N] : ";
    cin >> confirmingChar;
    if (confirmingChar == 'T' || confirmingChar == 't') {
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
        if (confirmingChar == 'T' || confirmingChar == 't') {
            for (int i = 0; i < (int) recipient.size(); i++) {
                if (recipient[i].id == idToKill) {
                    recipient.erase (recipient.begin()+i);
                }
            }
            saveDataAfterDeletion (recipient, idToKill);
        }
    }
    cin.clear();
    system ("pause");
}

string convertVectorDataToString (vector <Recipient> &recipient, int givenId, int loggedInUserId) {
    string personsData = "";
    int indexOfData = 0;
    const int AMOUNT_OF_LOOPS = 7;

    for (int i = 0; i < AMOUNT_OF_LOOPS; i++) {
        if (indexOfData == 0) {
            string id = makeWordsNotNumbers (recipient[givenId].id);
            personsData.append (id);
        } else if (indexOfData == 1) {
            string userId = makeWordsNotNumbers (loggedInUserId);
            personsData.append (userId);
        } else if (indexOfData == 2) {
            personsData.append (recipient[givenId].name);
        } else if (indexOfData == 3) {
            personsData.append (recipient[givenId].surname);
        } else if (indexOfData == 4) {
            personsData.append (recipient[givenId].phoneNumber);
        } else if (indexOfData == 5) {
            personsData.append (recipient[givenId].email);
        } else if (indexOfData == 6) {
            personsData.append (recipient[givenId].address);
        }
        personsData.push_back ('|');
        indexOfData++;
    }

    return personsData;
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

void saveRecipientDataToFile (vector <Recipient> &recipient, int idToEdit, int loggedInUserId) {
    fstream tempFile, recipientDataFile;
    string data = "", editedRecipientdata = "";
    int positionOfPerson = positionOfPersonInFile(recipient, idToEdit);

    recipientDataFile.open ("Recipients.txt",ios::in);
    tempFile.open ("temp.txt", ios::out);

    editedRecipientdata = convertVectorDataToString (recipient, positionOfPerson, loggedInUserId);

    while (getline (recipientDataFile,data) ) {
        if(readTheId (data) != idToEdit){
            tempFile << data << endl;
        }else{
            cout << editedRecipientdata << endl;
            tempFile << editedRecipientdata << endl;
        }
    }

    recipientDataFile.close();
    tempFile.close();

    remove ("Recipients.txt");
    rename ("temp.txt","Recipients.txt");
}

void changeTheName (vector <Recipient> &recipient, int positionOfPerson) {
    string newName = "";
    cout << "Podaj nowe imie: ";
    cin >> newName;

    recipient[positionOfPerson].name = changeFirstLetterToCapitalLetter (newName);
    cout << "Pomylsnie zmieniono imie." << endl;
    system ("pause");
}

void changeTheSurname (vector <Recipient> &recipient, int positionOfPerson) {
    string newSurname = "";
    cout << "Podaj nowe nazwisko: ";
    cin >> newSurname;

    recipient[positionOfPerson].surname = changeFirstLetterToCapitalLetter (newSurname);
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
            cout << "1. Edytuj imie" << endl;
            cout << "2. Edytuj nazwisko" << endl;
            cout << "3. Edytuj numer telefonu" << endl;
            cout << "4. Edytuj email" << endl;
            cout << "5. Edytuj adres" << endl;
            cout << "0. Powrot do menu" << endl;
            cout << "Podaj wybor : ";
            menuOptions = getchar();

            switch (menuOptions) {
            case '1':
                changeTheName (recipient, positionOfPerson);
                saveRecipientDataToFile (recipient, idToEdit, loggedInUserId);
                break;
            case '2':
                changeTheSurname (recipient, positionOfPerson);
                saveRecipientDataToFile (recipient, idToEdit, loggedInUserId);
                break;
            case '3':
                changeThePhoneNumber (recipient, positionOfPerson);
                saveRecipientDataToFile (recipient, idToEdit, loggedInUserId);
                break;
            case '4':
                changeTheEmail (recipient, positionOfPerson);
                saveRecipientDataToFile (recipient, idToEdit, loggedInUserId);
                break;
            case '5':
                changeTheAddress (recipient, positionOfPerson);
                saveRecipientDataToFile (recipient, idToEdit, loggedInUserId);
                break;
            case '0':
                return;
            }
        }
    }
    cin.clear();
    system ("pause");
}

void saveUpdatedUserData (vector <User> &user) {
    fstream userDataFile;
    string data = "";

    userDataFile.open ("Users.txt",ios::out);

    if (userDataFile.good() == true) {
        for (vector<User>::iterator itr = user.begin(); itr != user.end(); itr++) {
            data += makeWordsNotNumbers (itr->id) + '|';
            data += itr->login + '|';
            data += itr->password + '|';

            userDataFile << data << endl;
            data = "";
        }
        userDataFile.close();

        cout << endl << "Dane zapisane pomyslnie" << endl;
        system ("pause");
    } else {

        cout << "Nie udlao sie zapisac danych!!" << endl;
        system ("pause");
    }
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
            if (confirmation == 'T' || confirmation == 't') {
                user[i].password = password;
                saveUpdatedUserData (user);
                cout << "Haslo zostalo zmienione" << endl;
                system ("pause");
            } else if (confirmation == 'N' || confirmation == 'n') {
                cout << "Haslo nie zostalo zmienione" << endl;
                system ("pause");
            } else {
                cout << "Nieprawidlowy znak " << endl;
                break;
            }
        }
    }
}

int main() {

    int loggedInUserId = 0;
    int tag = 0;
    char menuOptions;

    vector <User> user;
    vector <Recipient> recipient;

    while (1) {
        if (loggedInUserId == 0) {
            user = openUserDataFile (user);
            system ( "cls" );
            cout << "1. Logowanie" << endl;
            cout << "2. Rejestracja" << endl;
            cout << "0. Wyjdz z programu" << endl;
            cout << "Podaj wybor : ";
            menuOptions = getchar();
            cin.sync();

            switch (menuOptions) {
            case '1':
                if (user.size() == 0) {
                    cout << "Nie ma zarejestrowanych uzytkownikow!!" << endl;
                    cout << "Prosze o zarejestrowanie uzytkownika!!" << endl;
                    system ("pause");
                    menuOptions = '2';
                } else if (user.size() >0) {
                    loggedInUserId = login (user);
                    break;
                } else {
                    break;
                }
            case '2':
                user = registerNewUser (user);
                break;
            case '0':
                exit (0);
                break;
            }
        } else {
            if (tag == 0) {
                recipient = openRecipientDataFile (recipient, loggedInUserId);
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
            cin.sync();

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
