#include <iostream>
#include <string>
#include<string.h>
#include <vector>
#include <random>
using namespace std;
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";
const string BOLD = "\033[1m";

struct Book
{
    string title;
    string author;
    string isbn;
    bool isAvailable;
    string borrowerId;
};

struct Member
{
    string name;
    string memberId;
    string contactInfo;
    vector<string> borrowBooksId;
};

struct LibraryManager
{
    string memberid;
    string isbn;
};

class Library
{
private:
    vector<Book> books;
    vector<Member> members;

public:
    void addBook()
    {
        Book newBook;
        string title, author;
        cout << "Enter book title: ";
        getline(cin, title);
        cout << "Enter book author: ";
        getline(cin, author);
        newBook.title = title;
        newBook.author = author;
        newBook.isbn = genrateIsbn();
        newBook.isAvailable = true;
        books.push_back(newBook);
    }

    void addMember();
    int LoginMember();
    string genrateIsbn();
    void menu()
    {
        cout << "1. Add Book\n2. Add Member\n3. Login Member\n4. Exit\n";
    }

    string genrateMemberId();
    vector<int> searchBook() ; 
    int borrowBook(string mid , int index) ;
};

string Library::genrateMemberId()
{

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string first12 = "";
    for (int i = 0; i < 10; ++i)
    {
        first12 += std::to_string(dis(gen));
    }

    // now we suffel all of that to make sure about true random

    std::uniform_int_distribution<> ds(0, 9);

    for (int i = 9; i != 0; i--)
    {
        int rad = ds(gen);

        char temp = first12[i];
        first12[i] = first12[rad];
        first12[rad] = temp;
    }
    if (first12[0] == '0')
    {
        first12[0] = random() % 9 + '1'; // ensure first digit is not zero
    }
    return first12;
}

string Library::genrateIsbn()
{

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string first12 = "978";
    for (int i = 0; i < 9; ++i)
    {
        first12 += std::to_string(dis(gen));
    }
    // now we suffel all of that to make sure about true random

    std::uniform_int_distribution<> ds(3, 11);

    for (int i = 11; i > 2; i--)
    {
        int rad = ds(gen);

        char temp = first12[i];
        first12[i] = first12[rad];
        first12[rad] = temp;
    }
    return first12;
}

void Library::addMember()
{
    Member newMember;
    string name, contactInfo;
    cout << "Enter member name: ";
    getline(cin, name);
    cout << "Enter contact info: ";
    getline(cin, contactInfo);
    newMember.name = name;
    newMember.contactInfo = contactInfo;
    newMember.memberId = genrateMemberId();
    members.push_back(newMember);
}
void menuForLogin()
{
    cout << "1. Borrow Book\n2. Return Book\n3. View Borrowed Books\n4. Logout\n";
}

int Library::LoginMember()
{
    string memberId;
    int choice;
    cout << "Enter member ID: ";
    getline(cin, memberId);
    int index = -1;
    for (int i = 0; i < members.size(); i++)
    {
        if (members[i].memberId == memberId)
        {
            index = i;
            cout << "Member logged in successfully." << endl;
            break;
        }
    }
    if (index == -1)
    {
        cout << "Member ID not found." << endl;
        return -1;
    }

    menuForLogin();
    while (true)
    {
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // to ignore the newline character after choice input
        switch (choice) 
        {
        case 1:
            // Borrow Book logic here
            cout << "Borrow Book functionality is not implemented yet.\n";
            borrowBook(memberId , index);
            break;
        case 2:
            // Return Book logic here
            cout << "Return Book functionality is not implemented yet.\n";
            break;
        case 3:
            // View Borrowed Books logic here
            cout << "View Borrowed Books functionality is not implemented yet.\n";
            break;
        case 4:
            cout << "Logging out...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}


int Library::borrowBook(string mid , int index)
{
    int choice = -1; 
     vector<int>foundBooks = searchBook();
     cout << "The name of the books are :" << endl ; 
     for(int i = 0 ; i < foundBooks.size() ; i++)
     {
        cout<<(i+1)<<" " << books[foundBooks[i]].title <<" "<< books[foundBooks[i]].author << endl;
     }
     if(!books[choice].isAvailable)
     {
        cout <<"the book is already taken" << endl;
     }

     cout << endl;
     cout << "enter the book you want" << endl;
     cin >> --choice ;
     books[choice].isAvailable = false;
     books[choice].borrowerId = mid;
     members[index].borrowBooksId.push_back(books[choice].borrowerId);

return 0;

     
}

vector<int> Library::searchBook()
{
    string title, author;
    cout << "Enter book title or author to search: ";
    getline(cin, title);
    getline(cin, author);
    vector<int> index;

    for(int i = 0 ; i < books.size() ; i++)
    {
        if(books[i].title == title || books[i].author == author)
        {
            cout << "Book Found: " << books[i].title << " by " << books[i].author << endl;
            index.push_back(i);
        }
    }
    if(index.size() == 0)
    {
        cout << "No books found matching the criteria." << endl;
    }
    return index;
}




int main()
{
    Library lib;
    int choice = -1;
    cout << BOLD << CYAN << "=== Welcome to Book Library ===" << RESET << "\n\n";
    while (true)
    {
        cout << BOLD << "Menu" << RESET << "\n";
        cout << BLUE << "1. Add Book" << RESET << "\n";
        cout << BLUE << "2. Add Member" << RESET << "\n";
        cout << BLUE << "3. Login Member" << RESET << "\n";
        cout << BLUE << "0. Exit" << RESET << "\n";
        cout << YELLOW << "Enter your choice: " << RESET;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input. Please enter a number." << RESET << "\n\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 0) {
            cout << GREEN << "Goodbye. Thank you for using Book Library." << RESET << "\n";
            break;
        }
        switch (choice)
        {
        case 1:
            cout << CYAN << "Add a new book" << RESET << "\n";
            lib.addBook();
            cout << GREEN << "Book added." << RESET << "\n\n";
            break;
        case 2:
            cout << CYAN << "Register a new member" << RESET << "\n";
            lib.addMember();
            cout << GREEN << "Member added." << RESET << "\n\n";
            break;
        case 3:
            cout << CYAN << "Member login" << RESET << "\n";
            if (lib.LoginMember() == -1) {
                cout << RED << "Login failed or member not found." << RESET << "\n\n";
            } else {
                cout << GREEN << "Logged out." << RESET << "\n\n";
            }
            break;
        default:
            cout << RED << "Invalid choice. Please try again." << RESET << "\n\n";
            break;
        }
    }
    return 0;
}