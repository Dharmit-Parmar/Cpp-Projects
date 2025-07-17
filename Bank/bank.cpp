#include <iostream>
#include <string>
#include <vector>
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
    float mobileNumber;
    vector<float> accountNumber;
    vector<float> balance = {0}; // Initialize balance with 0

public:
    void setDetails(string &name, float &mobilenumber, float &accountnumber)
    {
        this->userName = name;
        this->mobileNumber = mobilenumber;
        this->accountNumber.push_back(accountnumber);
        this->balance.push_back(0);
        this->totalAccount++;
    }

    Account(string name, float mobilenumber, float accountnumber, float initialBalance)
        : userName(name), mobileNumber(mobilenumber), totalAccount(1)
    {

        this->accountNumber.push_back(accountnumber);
        this->balance.clear();
        this->balance.push_back(initialBalance);
    }

   

    string getUserName() const
    {
        return this->userName;
    }

    float getMobileNumber() const
    {
        return this->mobileNumber;
    }

    vector<float> &getAccountNumbers()  
    {
        return this->accountNumber;
    }

    vector<float> &getBalance()
    {
        return this->balance;
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

public:
    bool check(float num)
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

    void addAccount()
    {
        string name;
        float mobileNumber, accountNumber;
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
        cout << BLUE << "Enter Account Number: " << RESET;
        cin >> accountNumber;
        cin.ignore();
        cout << BLUE << "Enter Username: " << RESET;
        getline(cin, name);
        cout << "Enter Initial Balance: ";
        float initialBalance;
        cin >> initialBalance;
        if (initialBalance < 0)
        {
            cout << RED << "Initial balance cannot be negative." << RESET << endl;
            return;
        }

        this->accounts.push_back(Account(name, mobileNumber, accountNumber, initialBalance));
        cout << GREEN << "Account added successfully!\n"
             << RESET;
    }

    int login()
    {
        float mobileNumber;
        cin.ignore();
        cout << BLUE << "Enter Mobile Number: " << RESET;
        cin >> mobileNumber;
        if (!this->check(mobileNumber))
        {
            cout << RED << "You don't have an account. Create an account." << RESET << endl;
            return -1;
        }
        if (mobileNumber < 0)
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
        cout << GREEN << "Your current balance is " << this->accounts[this->index].getBalance()[this->choice] << RESET << endl;
        cout << CYAN << "You are logged in successfully!" << RESET << endl;
        cout << "Choose an option:\n";
        cout << "1. Withdraw\n";
        cout << "2. Display Account Details\n";
        int option;
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
        else
        {
            cout << RED << "Invalid option selected." << RESET << endl;
            return -1;
        }
        cout << GREEN << "Thank you for using our service!" << RESET << endl;
        return 0;
    }

    int withdraw()
    {
        int amount;

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

    void displayAccounts() const
    {
        cout << BOLD << CYAN << "\n=== All Accounts ===" << RESET << endl;
        for (const auto &account : this->accounts)
        {
            account.display();
            cout << YELLOW << "-----------------------------" << RESET << endl;
        }
    }

    void makeAnotherAccount(string &name, float &mobileNumber)
    {
         for (int i = 0; i < this->accounts.size(); i++)
        {
            if (this->accounts.at(i).getMobileNumber() == mobileNumber)
            {
                this->index = i;
                break;
            }
        }
         
     
         
       

        

        float accountnumber;
        float balance;
        cout << "Making another account username: " << name << endl;
        cout << BLUE << "Enter Account Number: " << RESET;
        cin >> accountnumber;
        for (int i = 0; i < this->accounts[index].getAccountNumbers().size(); i++)
        {
            if ( this->accounts[index].getAccountNumbers()[i] == accountnumber)
            {
                cout << RED << "Account number already exists. Please try again." << RESET << endl;
                return;
            }
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
    }
};

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

// here bugs are

// 5. The program does not handle the case where the user tries to withdraw more than their balance.
// 6. The program does not handle the case where the user tries to create an account with an existing mobile number.
// 7. The program does not handle the case where the user tries to create an account with an existing account number.
// 8. The program does not handle the case where the user tries to login with an invalid mobile number.
// 9. The program does not handle the case where the user tries to login with an invalid account number.
// 10. The program does not handle the case where the user tries to withdraw an invalid amount (negative or zero).
// 11. The program does not handle the case where the user tries to withdraw more than their balance.
// 12. The program does not handle the case where the user tries to withdraw from an account that does not exist.
// 13. The program does not handle the case where the user tries to display accounts when there are no accounts.
// 14. The program does not handle the case where the user tries to login when there are no accounts.
// 15. The program does not handle the case where the user tries to add an account with an empty name.
// 16. The program does not handle the case where the user tries to add an account with an empty mobile number.
// 17. The program does not handle the case where the user tries to add an account with an empty account number.
// 18. The program does not handle the case where the user tries to add an account with a negative initial balance.
// 19. The program does not handle the case where the user tries to add an account with a zero initial balance.
// 20. The program does not handle the case where the user tries to add an account with a negative mobile number.
// 21. The program does not handle the case where the user tries to add an account with a negative account number.
