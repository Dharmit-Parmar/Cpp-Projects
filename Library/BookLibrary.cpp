#include <iostream>
#include <string>
#include <vector>
using namespace std;

 
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"

class Book
{
private:
    string title, author;

public:
    void setDetails(const string t, const string a)
    {
        this->title = t;
        this->author = a;
    }

    void readDetails() const
    {
        cout << CYAN << "Book Title  : " << RESET << this->title << endl;
        cout << CYAN << "Book Author : " << RESET << this->author << endl;
    }
    string getTitle() const
    {
        return title;
    }
    string getAuthor() const
    {
        return author;
    }
    Book(string t, string a) : title(t), author(a) {}
};

class Lib
{
private:
    vector<Book> books;

public:
    void addBook()
    {
        string t, a;
        cin.ignore();
        cout << GREEN << "\n=== Add a New Book ===\n" << RESET;
        cout << "Enter Book Title: ";
        getline(cin, t);
        cout << "Enter Author Name: ";
        getline(cin, a);
        books.push_back(Book(t, a));
        cout << GREEN << "Book added successfully!\n" << RESET;
    }

    void listOfBook()
    {
        cout << BLUE << "\n=== List of Books ===\n" << RESET;
        if (books.empty())
        {
            cout << YELLOW << "No books in the library.\n" << RESET;
            return;
        }
        for (int i = 0; i < books.size(); i++)
        {
            cout << MAGENTA << "\nBook #" << i + 1 << ":\n" << RESET;
            books.at(i).readDetails();
        }
        cout << BLUE << "\nEnd of list.\n" << RESET;
    }
    void deleteBook();
    void replaceBook();
};

void Lib::deleteBook()
{
    string t, a;
    cin.ignore();
    cout << RED << "\n=== Delete a Book ===\n" << RESET;
    cout << "Enter Book Title to delete: ";
    getline(cin, t);
    cout << "Enter Author Name to delete: ";
    getline(cin, a);

    for (int i = 0; i < books.size(); i++)
    {
        if (books.at(i).getTitle() == t && books.at(i).getAuthor() == a)
        {
            books.erase(books.begin() + i);
            cout << RED << "Book deleted successfully.\n" << RESET;
            --i;
            break;
        }
    }
}

void Lib::replaceBook()
{
    string t, a;

    cout << YELLOW << "\n=== Replace Book Details ===\n" << RESET;
    cout << "Choose the book from the list below:\n";
    this->listOfBook();
    cout << "\n-----------------------------\n";
    cin.ignore();
    cout << "Enter Book Title to replace: ";
    getline(cin, t);
    cout << "Enter Author Name to replace: ";
    getline(cin, a);

    for (int i = 0; i < books.size(); i++)
    {
        if (books.at(i).getTitle() == t && books.at(i).getAuthor() == a)
        {
            cout << "\n-----------------------------\n";
            cin.ignore();
            cout << "Enter Book New Title : ";
            getline(cin, t);
            cout << "Enter Author New Name : ";
            getline(cin, a);
            books.at(i).setDetails(t, a);
            cout << GREEN << "Book details updated successfully.\n" << RESET;
        }
    }
}

int main()
{
    Lib lib;
    int choice;

    cout << BOLD << CYAN << "==============================\n";
    cout << "  Welcome to Book Library  \n";
    cout << "==============================" << RESET << endl;

    do
    {
        cout << BOLD << "\nMenu:\n" << RESET;
        cout << " 1. Add Book\n";
        cout << " 2. List Books\n";
        cout << " 3. Delete Book\n";
        cout << " 4. Replace Book\n";
        cout << " 0. Exit\n";
        cout << "------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            lib.addBook();
            break;
        case 2:
            lib.listOfBook();
            break;
        case 3:
            lib.deleteBook();
            break;
        case 4:
            lib.replaceBook();
            break;
        case 0:
            cout << CYAN << "\nExiting the library. Goodbye!\n" << RESET;
            break;
        default:
            cout << RED << "Invalid choice. Please try again.\n" << RESET;
        }

    } while (choice != 0);
    return 0;
}


