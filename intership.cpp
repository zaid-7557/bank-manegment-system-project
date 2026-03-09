// ======================================================
// BANK MANAGEMENT SYSTEM (C++)
// ======================================================
// Features:
// ✔ Create Account
// ✔ Deposit Money
// ✔ Withdraw Money
// ✔ Check Balance
// ✔ Display Account Details
// ✔ Delete Account
// ✔ Dashboard View
// ✔ Save to File
// ✔ Load from File
// ======================================================
#include <iostream>     // input/output
#include <fstream>      // file read/write
#include <vector>       // store list of accounts
#include <string>       // string handling
#include <regex>        // phone/email validation
#include <sstream>      // used for reading file with delimiter 

using namespace std;

// ======================================================
// BANK ACCOUNT CLASS
// ======================================================
class BankAccount {
private:
    int accountNumber;  // unique account number
    int customerID;     // customer's ID
    string name;        // customer name
    string phone;       // 10-digit phone
    string email;       // valid email
    double balance;     // account balance

public:
    // Constructor: sets default balance
    BankAccount() {
        balance = 10000;
    }

    // ------------------------------------------------------
    // Create a new account
    // ------------------------------------------------------
    void createAccount(vector<BankAccount>& accounts) {

        // get account number & ensure unique
        while (true) {
            cout << "Enter Account Number: ";
            cin >> accountNumber;

            if (cin.fail()) {              // if input invalid
                cin.clear();               // reset error state
                cin.ignore(10000, '\n');   // clear input buffer
                cout << "Invalid number!\n";
                continue;
            }

            if (accountExists(accounts, accountNumber))
                cout << "Account number already exists!\n";
            else break;
        }

        cout << "Enter Customer ID: ";
        cin >> customerID;
        cin.ignore(); // clear leftover newline

        cout << "Enter Name: ";
        getline(cin, name);

        // Phone validation (must be 10 digits)
        while (true) {
            cout << "Enter Phone Number: ";
            getline(cin, phone);
            if (!regex_match(phone, regex("\\d{10}")))
                cout << "Phone must be 10 digits!\n";
            else break;
        }

        // Email validation
        while (true) {
            cout << "Enter Email: ";
            getline(cin, email);
            if (!regex_match(email,
                regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})")))
                cout << "Invalid email!\n";
            else break;
        }

        cout << "\nAccount Created! Initial Balance: ₹" << balance << endl;
    }

    // ------------------------------------------------------
    // Deposit
    // ------------------------------------------------------
    void depositMoney(double amount) {
        if (amount <= 0) {                  // prevent negative
            cout << "Invalid amount!\n";
            return;
        }
        balance = balance + amount;
        cout << "₹" << amount << " deposited. New Balance: ₹" << balance << endl;
    }

    // ------------------------------------------------------
    // Withdraw
    // ------------------------------------------------------
    void withdrawMoney(double amount) {
        if (amount <= 0) {
            cout << "Invalid amount!\n";
            return;
        }
        if (amount > balance)
            cout << "Insufficient balance!\n";
        else {
            balance = balance - amount;
            cout << "₹" << amount << " withdrawn. New Balance: ₹" << balance << endl;
        }
    }

    // ------------------------------------------------------
    // Display balance
    // ------------------------------------------------------
    void checkBalance() const {
        cout << "Current Balance: ₹" << balance << endl;
    }

    // ------------------------------------------------------
    // Display full account details
    // ------------------------------------------------------
    void displayAccount() const {
        cout << "\n--- Account Details ---\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Customer ID: " << customerID << endl;
        cout << "Name: " << name << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "Balance: ₹" << balance << endl;
        cout << "Finance Offers: Credit cards, Loans available.\n";
    }

    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    // ------------------------------------------------------
    // Check if account number already exists
    // ------------------------------------------------------
    static bool accountExists(vector<BankAccount>& accounts, int accNo) {
        for (auto& acc : accounts)
            if (acc.accountNumber == accNo)
                return true;
        return false;
    }

    // ------------------------------------------------------
    // Save all accounts to file using "|" as separator
    // ------------------------------------------------------
    static void saveAllToFile(vector<BankAccount>& accounts) {
        ofstream outFile("accounts.txt");

        for (auto& acc : accounts) {
            outFile << acc.accountNumber << "|"
                    << acc.customerID << "|"
                    << acc.name << "|"
                    << acc.phone << "|"
                    << acc.email << "|"
                    << acc.balance << "\n";
        }

        outFile.close();
    }

    // ------------------------------------------------------
    // Load all accounts from file
    // ------------------------------------------------------
    static vector<BankAccount> loadFromFile() {
        vector<BankAccount> accounts;
        ifstream inFile("accounts.txt");

        string line;
        while (getline(inFile, line)) {     // read each line
            stringstream ss(line);
            BankAccount acc;

            string a, b, bal;

            getline(ss, a, '|');
            getline(ss, b, '|');
            getline(ss, acc.name, '|');
            getline(ss, acc.phone, '|');
            getline(ss, acc.email, '|');
            getline(ss, bal, '|');

            if (a.empty()) continue;        // skip empty lines

            acc.accountNumber = stoi(a);
            acc.customerID = stoi(b);
            acc.balance = stod(bal);

            accounts.push_back(acc);        // add to list
        }

        inFile.close();
        return accounts;
    }
};

// ======================================================
// UTILITY FUNCTIONS
// ======================================================

// find account by account number
BankAccount* findAccount(vector<BankAccount>& accounts, int accNo) {
    for (auto& acc : accounts)
        if (acc.getAccountNumber() == accNo)
            return &acc;
    return nullptr;
}

// delete account
void deleteAccount(vector<BankAccount>& accounts, int accNo) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->getAccountNumber() == accNo) {
            accounts.erase(it);                 // remove from vector
            cout << "Account deleted!\n";
            BankAccount::saveAllToFile(accounts);
            return;
        }
    }
    cout << "Account not found!\n";
}

// dashboard: show all accounts
void displayDashboard(vector<BankAccount>& accounts) {
    if (accounts.empty()) {
        cout << "No accounts available.\n";
        return;
    }

    cout << "\n--- Dashboard ---\n";
    cout << "Acc No\tName\t\tBalance\n";
    cout << "---------------------------------\n";

    for (auto& acc : accounts)
        cout << acc.getAccountNumber() << "\t" << acc.getName()
             << "\t\t₹" << acc.getBalance() << endl;
}

// show menu
void showMenu() {
    cout << "\n--- Bank Management System ---\n";
    cout << "1. Create New Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Check Balance\n";
    cout << "5. Display Account Details\n";
    cout << "6. Delete Account\n";
    cout << "7. Display Dashboard\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

// ======================================================
// MAIN FUNCTION
// ======================================================
int main() {

    vector<BankAccount> accounts = BankAccount::loadFromFile(); // load saved accounts

    int choice, accNo;
    double amount;

    do {
        showMenu();
        cin >> choice;

        // validate integer input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice!\n";
            continue;
        }

        BankAccount* accPtr;

        switch (choice) {

        case 1: {   // create account
            BankAccount acc;
            acc.createAccount(accounts);
            accounts.push_back(acc);
            BankAccount::saveAllToFile(accounts);
            break;
        }

        case 2:     // deposit
            cout << "Enter Account Number: ";
            cin >> accNo;

            accPtr = findAccount(accounts, accNo);
            if (accPtr) {
                cout << "Enter amount to deposit: ₹";
                cin >> amount;
                accPtr->depositMoney(amount);
                BankAccount::saveAllToFile(accounts);
            } else cout << "Account not found!\n";
            break;

        case 3:     // withdraw
            cout << "Enter Account Number: ";
            cin >> accNo;

            accPtr = findAccount(accounts, accNo);
            if (accPtr) {
                cout << "Enter amount to withdraw: ₹";
                cin >> amount;
                accPtr->withdrawMoney(amount);
                BankAccount::saveAllToFile(accounts);
            } else cout << "Account not found!\n";
            break;

        case 4:     // check balance
            cout << "Enter Account Number: ";
            cin >> accNo;

            accPtr = findAccount(accounts, accNo);
            if (accPtr) accPtr->checkBalance();
            else cout << "Account not found!\n";
            break;

        case 5:     // show account details
            cout << "Enter Account Number: ";
            cin >> accNo;

            accPtr = findAccount(accounts, accNo);
            if (accPtr) accPtr->displayAccount();
            else cout << "Account not found!\n";
            break;

        case 6:     // delete
            cout << "Enter Account Number to delete: ";
            cin >> accNo;
            deleteAccount(accounts, accNo);
            break;

        case 7:     // dashboard
            displayDashboard(accounts);
            break;

        case 8:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 8);

    return 0;
}