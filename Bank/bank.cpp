#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

// Color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD "\033[1m"

class Account
{
private:
    string userName;
    int totalAccount = 0;
    string mobileNumber;
    vector<string> accountNumber;
    vector<int> Pin;
    vector<float> balance;

public:
    void setDetails(string &name, float &mobilenumber, string &accountnumber, int &pin)
    {
        this->userName = name;
        this->mobileNumber = mobilenumber;
        this->accountNumber.push_back(accountnumber);
        this->Pin.push_back(pin);
        this->totalAccount++;
    }

    Account(string name, string mobilenumber, string accountnumber, float initialBalance, int pin)
        : userName(name), mobileNumber(mobilenumber), totalAccount(1)
    {
        if (initialBalance < 0 || initialBalance == 0)
        {
            cout << RED << "Initial balance cannot be negative." << RESET << endl;
            return;
        }
        if (pin < 0 || pin > 9999 || pin < 1000)
        {
            cout << RED << "Enter the valid pin" << RESET << endl;
            return;
        }
        this->accountNumber.push_back(accountnumber);
        this->balance.push_back(initialBalance) ; 
        this->Pin.push_back(pin);
    }

    bool setPin(int pin)
    {
        if (pin < 0 || pin > 9999 || pin < 1000)
        {
            cout << RED << "Enter the valid pin" << RESET << endl;
            return false;
        }
        this->Pin.push_back(pin);
        return true ;
    }

    string getUserName() const
    {
        return this->userName;
    }
    vector<int> getPin() const
    {
        return this->Pin;
    }
    string getMobileNumber() const
    {
        return this->mobileNumber;
    }

    vector<string> &getAccountNumbers()
    {
        return this->accountNumber;
    }

    vector<float> &getBalance()
    {
        return this->balance;
    }
    int changePin(int oldPin, int newPin)
    {
        if (oldPin < 0 || oldPin > 9999 || oldPin < 1000 || newPin < 0 || newPin > 9999 || newPin < 1000)
        {
            cout << RED << "Enter the valid pin" << RESET << endl;
            return -1;
        }
        for (int i = 0; i < this->Pin.size(); i++)
        {
            if (this->Pin[i] == oldPin)
            {
                this->Pin[i] = newPin;
                cout << GREEN << "Pin changed successfully!" << RESET << endl;
                return 0;
            }
        }
        cout << RED << "Old pin does not match." << RESET << endl;
        return -1;
    }

    void display() const
    {
        cout << CYAN << "Username: " << RESET << this->getUserName() << endl;
        cout << CYAN << "Mobile Number: " << RESET << this->getMobileNumber() << endl;
        cout << CYAN << "Accounts: " << RESET;
        for (int i = 0; i < this->accountNumber.size(); i++)
        {
            cout << this->accountNumber[i] << " ";
            cout << CYAN << "Balance: " << RESET << this->balance[i] << " " << endl;
        }
        cout << endl;
    }
};

class Bank
{
private:
    vector<Account> accounts;
    int index;
    int choice;
    vector<string> universalAccounts;

public:
    bool check(string num)
    {
        for (int i = 0; i < this->accounts.size(); i++)
        {
            if (this->accounts[i].getMobileNumber() == num)
            {
                return true;
            }
        }
        return false;
    }

    void addAccount();
    int createAccountNO();
    bool checkAccountNumber(string &accountNumber);
    int login();

    int withdraw();
    int transfer();

    void displayAccounts() const
    {
        cout << BOLD << CYAN << "\n=== All Accounts ===" << RESET << endl;
        for (const auto &account : this->accounts)
        {
            account.display();
            cout << YELLOW << "-----------------------------" << RESET << endl;
        }
    }

    void makeAnotherAccount(string &name, string &mobileNumber);
    string genrateAccountNumber();
};



void Bank::addAccount()
{
    string name;
    string mobileNumber;
    string accountNumber;
    int pin;
    cin.ignore();
    cout << BLUE << "Enter Mobile Number: " << RESET;
    cin >> mobileNumber;
    if (this->check(mobileNumber))
    {
        cout << YELLOW << "You already have an account." << RESET << endl;
        cout << GREEN << " You are making another account with the same mobile number." << RESET << endl;
        int choice;
        cout << "Do you want to continue? (1 for Yes, 0 for No): ";
        cin >> choice;
        if (choice == 0)
        {
            cout << RED << "Account creation cancelled." << RESET << endl;
            return;
        }
        cout << GREEN << "Continuing with account creation..." << RESET << endl;
        cin.ignore();
        name = this->accounts[0].getUserName();
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
    if (initialBalance < 0)
    {
        cout << RED << "Initial balance cannot be negative." << RESET << endl;
        return;
    }

    this->accounts.push_back(Account(name, mobileNumber, accountNumber, initialBalance, pin));
    this->universalAccounts.push_back(accountNumber);
    cout << GREEN << "Account added successfully!\n"
         << RESET;
}

int Bank::login()
{
    string mobileNumber;
    cout << BLUE << "Enter Mobile Number: " << RESET;
    cin >> mobileNumber;
    if (!this->check(mobileNumber))
    {
        cout << RED << "You don't have an account. Create an account." << RESET << endl;
        return -1;
    }
    if (mobileNumber.size() < 0 || mobileNumber.size() > 10)
    {
        cout << RED << "Mobile number cannot be negative." << RESET << endl;
        return -1;
    }

    // finding what is the index of the account
    for (int i = 0; i < this->accounts.size(); i++)
    {
        if (this->accounts.at(i).getMobileNumber() == mobileNumber)
        {
            this->index = i;
            break;
        }
    }

    cout << CYAN << "Choose the account number you want to login:" << RESET << endl;
    for (int i = 0; i < this->accounts[this->index].getAccountNumbers().size(); i++)
    {
        cout << MAGENTA << (i + 1) << ". " << this->accounts[this->index].getAccountNumbers().at(i) << RESET << endl;
    }

    cin >> this->choice;
    --this->choice;
    cout << GREEN << "Your current balance is " << this->accounts.at(this->index).getBalance().at(this->choice) << RESET << endl;
    cout << CYAN << "You are logged in successfully!" << RESET << endl;
    cout << "Choose an option:\n";
    cout << "1. Withdraw\n";
    cout << "2. Display Account Details\n";
    cout << "3. Change Pin\n";
    cout << "4. Transfer Money\n";
    cout << "0. Exit\n";
    int option;
    do
    {
    cout << YELLOW << "Enter your option: " << RESET;
        cin >> option;
    
    if (option == 1)
    {
        this->withdraw();
        return 0;
    }
    else if (option == 2)
    {
        this->accounts[this->index].display();
    }
    else if (option == 3)
    {
        int oldPin, newPin;
        cout << BLUE << "Enter Old Pin: " << RESET;
        cin >> oldPin;
        cout << BLUE << "Enter New Pin: " << RESET;
        cin >> newPin;
        if (this->accounts[this->index].changePin(oldPin, newPin) == -1)
        {
            cout << RED << "Failed to change pin." << RESET << endl;
            return -1;
        }
        cout << GREEN << "Pin changed successfully!" << RESET << endl;
    }
    else if (option == 4)
    {
        if (this->transfer() == -1)
        {
            cout << RED << "Transfer failed. Please try again." << RESET << endl;
        }
    }
    else if (option == 0)
    {
        cout << GREEN << "Exiting..." << RESET << endl;
        return 0;
    }
    else
    {
        cout << RED << "Invalid option selected." << RESET << endl;
        return -1;
    }
} while (option != 0 );
cout << GREEN << "Thank you for using our service!" << RESET << endl; 

    return 0;
}

int Bank::withdraw()
{
    int amount;
    int pin;

    cout << BLUE << "Enter Pin: " << RESET;
    cin >> pin;
    if (pin != this->accounts[this->index].getPin()[choice])
    {
        cout << RED << "Invalid Pin. Please try again." << RESET << endl;
        return -1;
    }
    cout << BLUE << "How much money do you want to withdraw?" << RESET << endl;
    cin >> amount;
    cout << "You are withdrawing " << amount << " from your account." << endl;

    if (amount > this->accounts[this->index].getBalance()[this->choice] || amount < 0)
    {
        cout << RED << "You entered an insufficient amount." << RESET << endl;
        return -1;
    }
    this->accounts[this->index].getBalance()[this->choice] -= amount;
    cout << GREEN << "You have successfully withdrawn " << amount << " from your account." << RESET << endl;
    cout << CYAN << "Your new balance is: " << this->accounts[this->index].getBalance()[this->choice] << RESET << endl;
    cout << GREEN << "Thank you for using our service!" << RESET << endl;
    return 0;
}

void Bank::makeAnotherAccount(string &name, string &mobileNumber)
{
    int pin;
    for (int i = 0; i < this->accounts.size(); i++)
    {
        if (this->accounts.at(i).getMobileNumber() == mobileNumber)
        {
            this->index = i;
            break;
        }
    }

    string accountnumber;
    float balance;
    cout << "Making another account username: " << name << endl;
    accountnumber = genrateAccountNumber();
    cout << BLUE << "Your Account Number is: " << accountnumber << "\n" << RESET;
    
    for (int i = 0; i < this->accounts[index].getAccountNumbers().size(); i++)
    {
        if (this->accounts[index].getAccountNumbers()[i] == accountnumber)
        {
            cout << RED << "Account number already exists. Please try again." << RESET << endl;
            return;
        }
    }
    cout << BLUE << "Enter Pin: " << RESET;
    cin >> pin;
    if (pin < 0 || pin > 9999 || pin < 1000)
    {
        cout << RED << "Enter the valid pin" << RESET << endl;
        return;
    }

    cout << "Enter Initial Balance: ";
    cin >> balance;
    if (balance < 0)
    {
        cout << RED << "Initial balance cannot be negative." << RESET << endl;
        return;
    }

    this->accounts[this->index].getAccountNumbers().push_back(accountnumber);
    this->accounts[this->index].getBalance().push_back(balance);
    this->accounts[this->index].setPin(pin);
    this->universalAccounts.push_back(accountnumber);
    cout << GREEN << "Account created successfully!" << RESET << endl;
}

string Bank::genrateAccountNumber(){
  char rowAccount[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

    string acc = "1234567890";
    std::random_device rd;
    std::mt19937 rng(rd());
back_to_genrateAgain : 
    for(int i = 0 ; i<10;i++){
        std::uniform_int_distribution<int> dist(0 , 9);
        for (int i = 0; i < 10; i++) {
        std::uniform_int_distribution<int> dist(0, 9);
        acc[i] = rowAccount[dist(rng)];
    }

    }
    for(int i = acc.length() -1; i>0 ; i--){
        std::uniform_int_distribution<int> dist(0 , i);
        int rand = dist(rng);
        char temp;
        temp = acc[i];
        acc[i] = acc[rand];
        acc[rand] = temp;
        
    }
      std::shuffle(acc.begin(), acc.end(), rng);
    for(int i = 0 ; i<this->universalAccounts.size() ; i++){
        if(this->universalAccounts.at(i) == acc){
            goto back_to_genrateAgain ;
        }
    }
     
    return acc;
     
}

int Bank::transfer()
{
    string receiverAccountNumber;
    float amount;
    int pin;
    int Accountindex;
    int accindex ; 
    cout<< BLUE << "Enter the receiver's account number: " << RESET;
    cin >> receiverAccountNumber;
    if(receiverAccountNumber.size() < 10 || receiverAccountNumber.size() > 10)
    {
        cout << RED << "Invalid account number. Please try again." << RESET << endl;
        return -1;
    }
    for(int i = 0 ; i< this->accounts.size() ; i++)
    {
        for(int j= 0 ; j< this->accounts[i].getAccountNumbers().size() ; j++)
        {
            if(this->accounts[i].getAccountNumbers()[j] == receiverAccountNumber)
            {
                Accountindex = i;
                accindex = j ;
                cout << GREEN << "Receiver's account found!" << RESET << endl;
                break;
            }
            
        }
    }


    cout << BLUE << "Enter the amount to transfer: " << RESET;
    cin >> amount;
    cout << BLUE << "Enter your Pin: " << RESET;
    cin >> pin;
    if (pin != this->accounts[this->index].getPin()[this->choice])
    {
        cout << RED << "Invalid Pin. Please try again." << RESET << endl;
        return -1;
    }

    if (amount <= 0)
    {
        cout << RED << "Amount must be greater than zero." << RESET << endl;
        return -1;
    }
    if (amount > this->accounts[this->index].getBalance()[this->choice])
    {
        cout << RED << "Insufficient balance for transfer." << RESET << endl;
        return -1;
    }
    this->accounts[this->index].getBalance()[this->choice] -= amount;
    this->accounts[Accountindex].getBalance()[accindex] += amount;
    cout << GREEN << "Transfer successful! " << amount << " has been transferred to " << receiverAccountNumber << "." << RESET << endl;
    cout << CYAN << "Your new balance is: " << this->accounts[this->index].getBalance()[this->choice] << RESET << endl;
    return 0;


}

int main()
{
    Bank bank;
    int choice;
    cout << BOLD << CYAN << "==============================\n";
    cout << "      Welcome to Bank App     \n";
    cout << "==============================" << RESET << endl;
    do
    {

        cout << BOLD << "\nMenu:\n"
             << RESET;
        cout << BLUE << "1. Add Account\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";
        cout << YELLOW << "Enter your choice: " << RESET;
        cin >> choice;

        switch (choice)
        {
        case 1:
            bank.addAccount();
            break;
        case 2:
            if (bank.login() == -1)
            {
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
 
