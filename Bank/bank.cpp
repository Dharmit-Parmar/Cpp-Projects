#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD "\033[1m"

struct accountDetails {
    string accountNumber;
    int pin;
    float balance;
};

accountDetails setDetail(string a, int p, float b) {
    accountDetails acc;
    acc.accountNumber = a;
    acc.pin = p;
    acc.balance = b;
    return acc;
}

int findAccountIndex(const vector<string>& accounts, const string& accountNumber) {
    int s = 0;
    int e = accounts.size() - 1;
    while (s <= e) {
        int mid = s + (e - s) / 2;
        if (accounts[mid] == accountNumber) {
            return mid;
        }
        if (accounts[mid] < accountNumber) {
            s = mid + 1;
        } else {
            e = mid - 1;
        }
    }
    return -1;
}

class Account {
private:
    string userName;
    string mobileNumber;
    vector<accountDetails> acc;

public:
    Account(string name, string mobilenumber, string accountnumber, float initialBalance, int pin)
        : userName(name), mobileNumber(mobilenumber) {
        if (initialBalance < 0) {
            cout << RED << "Initial balance cannot be negative." << RESET << endl;
            return;
        }
        if (pin < 1000 || pin > 9999) {
            cout << RED << "Enter a valid pin" << RESET << endl;
            return;
        }
        acc.push_back(setDetail(accountnumber, pin, initialBalance));
        sortAccounts();
    }

    void sortAccounts() {
        sort(acc.begin(), acc.end(), [](const accountDetails& a, const accountDetails& b) {
            return a.accountNumber < b.accountNumber;
        });
    }

    bool setPin(int pin) {
        if (pin < 1000 || pin > 9999) {
            cout << RED << "Enter a valid pin" << RESET << endl;
            return false;
        }
        if (!acc.empty())
            acc.back().pin = pin;
        return true;
    }

    string getUserName() const {
        return userName;
    }

    vector<int> getPin() const {
        vector<int> pins;
        for (const auto& a : acc) pins.push_back(a.pin);
        return pins;
    }

    string getMobileNumber() const {
        return mobileNumber;
    }

    vector<string> getAccountNumbers() const {
        vector<string> numbers;
        for (const auto& a : acc) numbers.push_back(a.accountNumber);
        return numbers;
    }

    vector<float> getBalances() const {
        vector<float> balances;
        for (const auto& a : acc) balances.push_back(a.balance);
        return balances;
    }

    int changePin(int oldPin, int newPin) {
        if (oldPin < 1000 || oldPin > 9999 || newPin < 1000 || newPin > 9999) {
            cout << RED << "Enter a valid pin" << RESET << endl;
            return -1;
        }
        for (auto& a : acc) {
            if (a.pin == oldPin) {
                a.pin = newPin;
                cout << GREEN << "Pin changed successfully!" << RESET << endl;
                return 0;
            }
        }
        cout << RED << "Old pin does not match." << RESET << endl;
        return -1;
    }

    void display() const {
        cout << CYAN << "Username: " << RESET << userName << endl;
        cout << CYAN << "Mobile Number: " << RESET << mobileNumber << endl;
        cout << CYAN << "Accounts: " << RESET;
        for (const auto& a : acc) {
            cout << a.accountNumber << " ";
            cout << CYAN << "Balance: " << RESET << a.balance << " " << endl;
        }
        cout << endl;
    }

    void addAccount(string accountnumber, float balance, int pin) {
        acc.push_back(setDetail(accountnumber, pin, balance));
        sortAccounts();
    }

    int findAccountIdx(const string& accountNumber) const {
        vector<string> accNums = getAccountNumbers();
        return findAccountIndex(accNums, accountNumber);
    }

    float& getBalanceByIndex(int idx) {
        return acc[idx].balance;
    }
};

class Bank {
private:
    vector<Account> accounts;
    int index;
    int choice;
    vector<string> universalAccounts;

public:
    bool check(string num) {
        for (const auto& a : accounts) {
            if (a.getMobileNumber() == num) return true;
        }
        return false;
    }

    void addAccount();
    int login();
    int withdraw();
    int transfer();
    void displayAccounts() const {
        cout << BOLD << CYAN << "\n=== All Accounts ===" << RESET << endl;
        for (const auto& account : accounts) {
            account.display();
            cout << YELLOW << "-----------------------------" << RESET << endl;
        }
    }
    void makeAnotherAccount(string& name, string& mobileNumber);
    string genrateAccountNumber();
};

void Bank::addAccount() {
    string name, mobileNumber, accountNumber;
    int pin;
    cin.ignore();
    cout << BLUE << "Enter Mobile Number: " << RESET;
    cin >> mobileNumber;
    if (check(mobileNumber)) {
        cout << YELLOW << "You already have an account." << RESET << endl;
        cout << GREEN << "You are making another account with the same mobile number." << RESET << endl;
        int choice;
        cout << "Do you want to continue? (1 for Yes, 0 for No): ";
        cin >> choice;
        if (choice == 0) {
            cout << RED << "Account creation cancelled." << RESET << endl;
            return;
        }
        cout << GREEN << "Continuing with account creation..." << RESET << endl;
        cin.ignore();
        name = accounts[0].getUserName();
        makeAnotherAccount(name, mobileNumber);
        cout << GREEN << "Account created successfully!" << RESET << endl;
        return;
    }
    accountNumber = genrateAccountNumber();
    cout << BLUE << "Your Account Number is: " << accountNumber << "\n" << RESET;
    cout << BLUE << "Enter Username: " << RESET;
    cin >> name;
    cout << BLUE << "Enter Pin: " << RESET;
    cin >> pin;
    cout << "Enter Initial Balance: ";
    float initialBalance;
    cin >> initialBalance;
    if (initialBalance < 0) {
        cout << RED << "Initial balance cannot be negative." << RESET << endl;
        return;
    }
    accounts.push_back(Account(name, mobileNumber, accountNumber, initialBalance, pin));
    universalAccounts.push_back(accountNumber);
    cout << GREEN << "Account added successfully!\n" << RESET;
}

int Bank::login() {
    string mobileNumber;
    cout << BLUE << "Enter Mobile Number: " << RESET;
    cin >> mobileNumber;
    if (!check(mobileNumber)) {
        cout << RED << "You don't have an account. Create an account." << RESET << endl;
        return -1;
    }
    if (mobileNumber.size() > 10) {
        cout << RED << "Mobile number cannot be longer than 10 digits." << RESET << endl;
        return -1;
    }
    int foundIndex = -1;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts.at(i).getMobileNumber() == mobileNumber) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex == -1) {
        cout << RED << "Account not found." << RESET << endl;
        return -1;
    }
    index = foundIndex;
    cout << CYAN << "Choose the account number you want to login:" << RESET << endl;
    auto accNums = accounts[index].getAccountNumbers();
    for (int i = 0; i < accNums.size(); i++) {
        cout << MAGENTA << (i + 1) << ". " << accNums.at(i) << RESET << endl;
    }
    string selectedAccountNumber;
    cin >> selectedAccountNumber;
    int accIdx = findAccountIndex(accNums, selectedAccountNumber);
    if (accIdx == -1) {
        cout << RED << "Account number not found." << RESET << endl;
        return -1;
    }
    choice = accIdx;
    auto balances = accounts.at(index).getBalances();
    cout << GREEN << "Your current balance is " << balances.at(choice) << RESET << endl;
    cout << CYAN << "You are logged in successfully!" << RESET << endl;
    cout << "Choose an option:\n";
    cout << "1. Withdraw\n";
    cout << "2. Display Account Details\n";
    cout << "3. Change Pin\n";
    cout << "4. Transfer Money\n";
    cout << "0. Exit\n";
    int option;
    do {
        cout << YELLOW << "Enter your option: " << RESET;
        cin >> option;
        if (option == 1) {
            withdraw();
            return 0;
        } else if (option == 2) {
            accounts[index].display();
        } else if (option == 3) {
            int oldPin, newPin;
            cout << BLUE << "Enter Old Pin: " << RESET;
            cin >> oldPin;
            cout << BLUE << "Enter New Pin: " << RESET;
            cin >> newPin;
            if (accounts[index].changePin(oldPin, newPin) == -1) {
                cout << RED << "Failed to change pin." << RESET << endl;
                return -1;
            }
            cout << GREEN << "Pin changed successfully!" << RESET << endl;
        } else if (option == 4) {
            if (transfer() == -1) {
                cout << RED << "Transfer failed. Please try again." << RESET << endl;
            }
        } else if (option == 0) {
            cout << GREEN << "Exiting..." << RESET << endl;
            return 0;
        } else {
            cout << RED << "Invalid option selected." << RESET << endl;
            return -1;
        }
    } while (option != 0);
    cout << GREEN << "Thank you for using our service!" << RESET << endl;
    return 0;
}

int Bank::withdraw() {
    int amount, pin;
    cout << BLUE << "Enter Pin: " << RESET;
    cin >> pin;
    auto pins = accounts[index].getPin();
    if (pin != pins[choice]) {
        cout << RED << "Invalid Pin. Please try again." << RESET << endl;
        return -1;
    }
    cout << BLUE << "How much money do you want to withdraw?" << RESET << endl;
    cin >> amount;
    cout << "You are withdrawing " << amount << " from your account." << endl;
    float& balance = accounts[index].getBalanceByIndex(choice);
    if (amount > balance || amount < 0) {
        cout << RED << "You entered an insufficient amount." << RESET << endl;
        return -1;
    }
    balance -= amount;
    cout << GREEN << "You have successfully withdrawn " << amount << " from your account." << RESET << endl;
    cout << CYAN << "Your new balance is: " << balance << RESET << endl;
    cout << GREEN << "Thank you for using our service!" << RESET << endl;
    return 0;
}

void Bank::makeAnotherAccount(string& name, string& mobileNumber) {
    int pin;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts.at(i).getMobileNumber() == mobileNumber) {
            index = i;
            break;
        }
    }
    string accountnumber;
    float balance;
    cout << "Making another account username: " << name << endl;
    accountnumber = genrateAccountNumber();
    cout << BLUE << "Your Account Number is: " << accountnumber << "\n" << RESET;
    auto accNums = accounts[index].getAccountNumbers();
    int foundIdx = findAccountIndex(accNums, accountnumber);
    if (foundIdx != -1) {
        cout << RED << "Account number already exists. Please try again." << RESET << endl;
        return;
    }
    cout << BLUE << "Enter Pin: " << RESET;
    cin >> pin;
    if (pin < 1000 || pin > 9999) {
        cout << RED << "Enter a valid pin" << RESET << endl;
        return;
    }
    cout << "Enter Initial Balance: ";
    cin >> balance;
    if (balance < 0) {
        cout << RED << "Initial balance cannot be negative." << RESET << endl;
        return;
    }
    accounts[index].addAccount(accountnumber, balance, pin);
    universalAccounts.push_back(accountnumber);
    cout << GREEN << "Account created successfully!" << RESET << endl;
}

string Bank::genrateAccountNumber() {
    char rowAccount[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    string acc = "1234567890";
    std::random_device rd;
    std::mt19937 rng(rd());
back_to_genrateAgain:
    for (int i = 0; i < 10; i++) {
        std::uniform_int_distribution<int> dist(0, 9);
        for (int i = 0; i < 10; i++) {
            std::uniform_int_distribution<int> dist(0, 9);
            acc[i] = rowAccount[dist(rng)];
        }
    }
    for (int i = acc.length() - 1; i > 0; i--) {
        std::uniform_int_distribution<int> dist(0, i);
        int rand = dist(rng);
        char temp;
        temp = acc[i];
        acc[i] = acc[rand];
        acc[rand] = temp;
    }
    std::shuffle(acc.begin(), acc.end(), rng);
    for (int i = 0; i < universalAccounts.size(); i++) {
        if (universalAccounts.at(i) == acc) {
            goto back_to_genrateAgain;
        }
    }
    return acc;
}

int Bank::transfer() {
    string receiverAccountNumber;
    float amount;
    int pin;
    int Accountindex = -1;
    int accindex = -1;
    cout << BLUE << "Enter the receiver's account number: " << RESET;
    cin >> receiverAccountNumber;
    if (receiverAccountNumber.size() != 10) {
        cout << RED << "Invalid account number. Please try again." << RESET << endl;
        return -1;
    }
    for (int i = 0; i < accounts.size(); i++) {
        auto accNums = accounts[i].getAccountNumbers();
        int foundIdx = findAccountIndex(accNums, receiverAccountNumber);
        if (foundIdx != -1) {
            Accountindex = i;
            accindex = foundIdx;
            cout << GREEN << "Receiver's account found!" << RESET << endl;
            break;
        }
    }
    if (Accountindex == -1 || accindex == -1) {
        cout << RED << "Receiver account not found." << RESET << endl;
        return -1;
    }
    cout << BLUE << "Enter the amount to transfer: " << RESET;
    cin >> amount;
    cout << BLUE << "Enter your Pin: " << RESET;
    cin >> pin;
    auto pins = accounts[index].getPin();
    if (pin != pins[choice]) {
        cout << RED << "Invalid Pin. Please try again." << RESET << endl;
        return -1;
    }
    if (amount <= 0) {
        cout << RED << "Amount must be greater than zero." << RESET << endl;
        return -1;
    }
    float& senderBalance = accounts[index].getBalanceByIndex(choice);
    if (amount > senderBalance) {
        cout << RED << "Insufficient balance for transfer." << RESET << endl;
        return -1;
    }
    senderBalance -= amount;
    float& receiverBalance = accounts[Accountindex].getBalanceByIndex(accindex);
    receiverBalance += amount;
    cout << GREEN << "Transfer successful! " << amount << " has been transferred to " << receiverAccountNumber << "." << RESET << endl;
    cout << CYAN << "Your new balance is: " << senderBalance << RESET << endl;
    return 0;
}

int main() {
    Bank bank;
    int choice;
    cout << BOLD << CYAN << "==============================\n";
    cout << "      Welcome to Bank App     \n";
    cout << "==============================" << RESET << endl;
    do {
        cout << BOLD << "\nMenu:\n" << RESET;
        cout << BLUE << "1. Add Account\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";
        cout << YELLOW << "Enter your choice: " << RESET;
        cin >> choice;
        switch (choice) {
        case 1:
            bank.addAccount();
            break;
        case 2:
            if (bank.login() == -1) {
                cout << RED << "Login failed. Please try again." << RESET << endl;
            }
            break;
        case 0:
            cout << GREEN << "Thank you for using our service!" << RESET << endl;
            break;
        default:
            cout << RED << "Invalid choice. Please try again." << RESET << endl;
        }
    } while (choice != 0);
    return 0;
}



