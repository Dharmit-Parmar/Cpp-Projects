#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
using namespace std;
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";
const string BOLD = "\033[1m";

// small helper to keep consistent left margin and coloring
inline void pm(const string &msg, const string &color = RESET)
{
    cout << "   " << color << msg << RESET << "\n";
}

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
    string pin;
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

    // UI helpers (only formatting, no logic changes)
    void printBoxTop(const string &title)
    {
        cout << CYAN << BOLD << "╔══════════════════════════════════════════════════════════════╗" << RESET << "\n";
        cout << CYAN << BOLD << "║ " << setw(58) << left << title << " ║" << RESET << "\n";
        cout << CYAN << BOLD << "╠══════════════════════════════════════════════════════════════╣" << RESET << "\n";
    }

    void printBoxBottom()
    {
        cout << CYAN << BOLD << "╚══════════════════════════════════════════════════════════════╝" << RESET << "\n";
    }

    void printLine(const string &msg, const string &color = RESET)
    {
        cout << "   " << color << msg << RESET << "\n";
    }

    void printSectionTitle(const string &title)
    {
        cout << "\n";
        cout << MAGENTA << BOLD << "  >> " << title << RESET << "\n";
        cout << "   " << CYAN << "------------------------------------------------------------" << RESET << "\n";
    }

public:
    void addBook()
    {
        printBoxTop(" ADD NEW BOOK ");
        Book newBook;
        string title, author;
        printLine("Enter book title: ", YELLOW);
        cout << "     " << BOLD;
        getline(cin, title);
        cout << RESET;
        printLine("Enter book author: ", YELLOW);
        cout << "     " << BOLD;
        getline(cin, author);
        cout << RESET;
        newBook.title = title;
        newBook.author = author;
        newBook.isbn = genrateIsbn();
        newBook.isAvailable = true;
        books.push_back(newBook);
        printLine("");
        printLine("Book added successfully!", GREEN);
        printLine(string("ISBN: ") + BLUE + newBook.isbn + RESET);
        printBoxBottom();
    }
    bool checkContact(string contact);
    void addMember();
    int LoginMember();
    string genrateIsbn();
    void menu()
    {
        // use the helper for consistent margin
        printLine("1. Add Book   2. Add Member   3. Login Member   4. Exit", BLUE);
    }
    void sortMembersById()
    {

        std::sort(members.begin(), members.end(), [](const Member &a, const Member &b)
                  { return a.memberId < b.memberId; });
    }
    string genrateMemberId();
    vector<int> searchBook();
    vector<int> searchBook(string mid);
    int borrowBook(string mid, int index);
    int returnBook(string mid, int index);
    bool checkPina(string mid, string pin)
    {
        int s = 0;
        int e = members.size() - 1;
        while (s <= e)
        {
            int m = s + (e - s) / 2;
            if (members[m].memberId == mid)
            {
                if (members[m].pin == pin)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (members[m].memberId < mid)
            {
                s = m + 1;
            }
            else
            {
                e = m - 1;
            }
        }
        return false;
    }
    int viewBorrowedBooks(string mid);
};
string Library::genrateMemberId()
{

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string first13 = "";
    for (int i = 0; i < 14; ++i)
    {
        first13 += std::to_string(dis(gen));
    }

    // now we suffel all of that to make sure about true random

    std::uniform_int_distribution<> ds(0, 9);

    for (int i = 13; i != 0; i--)
    {
        int rad = ds(gen);

        char temp = first13[i];
        first13[i] = first13[rad];
        first13[rad] = temp;
    }
    if (first13[0] == '0')
    {
        first13[0] = random() % 9 + '1'; // ensure first digit is not zero
    }

    return first13;
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
    // Using the UI helper formatting while preserving logic
    cout << "\n";
    cout << "   " << CYAN << BOLD << "╔════════════════════════════════════════════════╗" << RESET << "\n";
    cout << "   " << CYAN << BOLD << "║               REGISTER MEMBER                  ║" << RESET << "\n";
    cout << "   " << CYAN << BOLD << "╠════════════════════════════════════════════════╣" << RESET << "\n";

    Member newMember;
    string name, contactInfo, PIn;
    cout << "   " << YELLOW << "Enter member name: " << RESET;
    cin.ignore();
    getline(cin, name);
    cout << "   " << YELLOW << "Enter contact info (must be 10 number): " << RESET;

    getline(cin, contactInfo);
    if ((contactInfo.size() != 10) || (!checkContact(contactInfo)))
    {
        cout << "   " << RED << "Contact info must be 10 digits long and should new" << RESET << endl;
        cout << "   " << CYAN << "╚════════════════════════════════════════════════╝" << RESET << "\n";
        return;
    }
    cout << "   " << YELLOW << "Set a 4-digit PIN for your account: " << RESET;
    cin >> PIn;
    if (PIn.size() != 4)
    {
        cout << "   " << RED << "PIN must be 4 digits long." << RESET << endl;
        cout << "   " << CYAN << "╚════════════════════════════════════════════════╝" << RESET << "\n";
        return;
    }
    newMember.pin = PIn;
    newMember.name = name;
    newMember.contactInfo = contactInfo;

    newMember.memberId = genrateMemberId();

    members.push_back(newMember);
    cout << "   " << GREEN << "Member added successfully!" << RESET << endl;
    cout << "   " << BLUE << "Member ID: " << MAGENTA << newMember.memberId << RESET << endl;
    cout << "   " << CYAN << "╚════════════════════════════════════════════════╝" << RESET << "\n\n";
}
void menuForLogin()
{
    pm("1. Borrow Book   2. Return Book   3. View Borrowed Books   4. Logout", BLUE);
}

int Library::LoginMember()
{
    string idd;
    string memberId, pin;
    int choice;
    cout << "\n";
    cout << "   " << CYAN << BOLD << "╔════════════════════════════════════════════════════╗" << RESET << "\n";
    cout << "   " << CYAN << BOLD << "║                   MEMBER LOGIN                     ║" << RESET << "\n";
    cout << "   " << CYAN << BOLD << "╠════════════════════════════════════════════════════╣" << RESET << "\n\n";

    cout << "   " << YELLOW << "Enter member ID : OR Contact No." << RESET;
    cin.ignore();
    cin >> idd;

    int index = -1;
    if (idd.length() == 10)
    {
        string contact = idd;
        string memberId = "";
        {
            for (int i = 0; i < members.size(); i++)
            {
                if (members[i].contactInfo == contact)
                {
                    cout << "   " << YELLOW << "Enter your 4-digit PIN: " << RESET;
                    cin >> pin;
                    if (checkPina(members[i].memberId, pin) == false)
                    {
                        cout << "   " << RED << BOLD << "✖ Incorrect PIN." << RESET << "\n\n";
                        return -1;
                    }
                    index = i;
                    memberId = members[i].memberId;
                    cout << "   " << GREEN << BOLD << "✔ Member logged in successfully." << RESET << "\n";
                    cout << "   " << BLUE << "Welcome, " << RESET << MAGENTA << members[i].name << RESET << "\n\n";
                    break;
                }
            }
        }
    }
    else
    {
        int s = 0;
        int e = members.size() - 1;
        while (s <= e)
        {
            int m = s + (e - s) / 2;
            if (members[m].memberId == idd)
            {
                index = m;
                memberId = idd;
                if (members[m].memberId == memberId)
                {
                    index = m;
                    cout << "   " << YELLOW << "Enter your 4-digit PIN: " << RESET;
                    cin >> pin;
                    if (checkPina(members[m].memberId, pin) == false)
                    {
                        cout << "   " << RED << BOLD << "✖ Incorrect PIN." << RESET << "\n\n";
                        return -1;
                    }
                    cout << "   " << GREEN << BOLD << "✔ Member logged in successfully." << RESET << "\n";
                    cout << "   " << BLUE << "Welcome, " << RESET << MAGENTA << members[m].name << RESET << "\n\n";
                    break;
                }
            }
            else if (members[m].memberId < idd)
            {
                s = m + 1;
            }
            else
            {
                e = m - 1;
            }
        }
    }

    if (index == -1)
    {
        cout << "   " << RED << BOLD << "✖ Member ID not found." << RESET << "\n\n";
        return -1;
    }
    // show the index in a cleaner way
    pm(string("Member index: ") + to_string(index), CYAN);
    cout << "   " << CYAN << "----------------------------------------------------" << RESET << "\n";
    menuForLogin();
    cout << "   " << CYAN << "----------------------------------------------------" << RESET << "\n";

    while (true)
    {
        cout << "\n";
        cout << BOLD << CYAN << "   Choose an action:" << RESET << "\n";
        menuForLogin();
        cout << "   " << YELLOW << "Enter your choice: " << RESET;
        cin >> choice;
        cin.ignore(); // to ignore the newline character after choice input

        switch (choice)
        {
        case 1:
            cout << "\n";
            cout << MAGENTA << BOLD << "   >>> Borrow Book <<<" << RESET << "\n\n";
            borrowBook(memberId, index);
            break;
        case 2:
            cout << "\n";
            cout << MAGENTA << BOLD << "   >>> Return Book <<<" << RESET << "\n\n";
            returnBook(memberId, index);
            break;
        case 3:
            cout << "\n";
            cout << MAGENTA << BOLD << "   >>> Your Borrowed Books <<<" << RESET << "\n\n";
            cout << "   " << CYAN << "(Listing borrowed books...)" << RESET << "\n\n";
            if (viewBorrowedBooks(memberId) == -1)
            {
                cout << "\n";
            }
            break;
        case 4:
            cout << "\n";
            cout << GREEN << BOLD << "   Logging out..." << RESET << "\n\n";
            return 0;
        default:
            cout << "   " << RED << "Invalid choice. Please try again." << RESET << "\n";
            break;
        }
    }
}

int Library::borrowBook(string mid, int index)
{
    int choice = -1;
    vector<int> foundBooks = searchBook();
    cout << "\n";
    cout << "   " << CYAN << BOLD << "=== Available Books ===" << RESET << endl;

    if (foundBooks.empty())
    {
        cout << "   " << RED << "No books available to borrow." << RESET << endl;
        return -1;
    }

    for (int i = 0; i < foundBooks.size(); i++)
    {
        cout << "   " << YELLOW << setw(3) << (i + 1) << RESET;
        cout << " " << BLUE << "Title: " << RESET << books[foundBooks[i]].title;
        cout << " " << MAGENTA << "| Author: " << RESET << books[foundBooks[i]].author << endl;
    }

    cout << endl
         << BOLD << "   Enter the book number you want to borrow: " << RESET;
    cin >> choice;
    choice--;

    if (choice < 0 || choice >= foundBooks.size())
    {
        cout << "   " << RED << "Invalid book selection." << RESET << endl;
        return -1;
    }
    int bookIndex = foundBooks[choice];
    if (!books[bookIndex].isAvailable)
    {
        cout << "   " << RED << "Sorry, this book is already borrowed." << RESET << endl;
        return -1;
    }
    books[bookIndex].isAvailable = false;
    books[bookIndex].borrowerId = mid;
    members[index].borrowBooksId.push_back(books[bookIndex].isbn);

    cout << "\n";
    cout << "   " << GREEN << "You have successfully borrowed: " << RESET << BLUE << "'" << books[bookIndex].title << "'" << RESET << endl;
    cout << "   " << MAGENTA << "Borrower ID: " << RESET << mid << "   " << MAGENTA << "ISBN: " << RESET << books[bookIndex].isbn << "\n";
    return 0;
}
int Library::returnBook(string mid, int index)
{
    vector<int> indexs = searchBook(mid);
    if (indexs.empty())
    {
        cout << "   " << RED << "You have no borrowed books to return." << RESET << endl;
        return -1;
    }

    int choice = -1;
    cout << "\n";
    cout << "   " << CYAN << BOLD << "Your Borrowed Books:" << RESET << "\n";
    for (int i = 0; i < indexs.size(); i++)
    {
        int j = indexs[i];
        cout << "     " << YELLOW << (i + 1) << ". " << RESET << books[j].title << " " << MAGENTA << "-" << RESET << " " << books[j].author << endl;
    }

    cout << "\n";
    cout << "   " << BOLD << "Which book would you like to return (enter number): " << RESET;
    cin >> choice;
    if (cin.fail() || choice < 1 || choice > (int)indexs.size())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "   " << RED << "Invalid selection." << RESET << endl;
        return -1;
    }

    int bookIdx = indexs[choice - 1];

    // remove the ISBN from the member's borrowed list
    string isbn = books[bookIdx].isbn;
    cout << "   " << CYAN << "Returning ISBN: " << RESET << isbn << "\n";
    for (int k = 0; k < members[index].borrowBooksId.size(); k++)
    {
        if (members[index].borrowBooksId[k] == isbn)
        {
            members[index].borrowBooksId.erase(members[index].borrowBooksId.begin() + k);
            break;
        }
    }

    books[bookIdx].isAvailable = true;
    books[bookIdx].borrowerId = "0";
    cout << "   " << GREEN << "Returned " << books[bookIdx].title << " successfully." << RESET << endl;
    return 0;
}
bool Library::checkContact(string contact)
{
    for (const auto &member : members)
    {
        if (member.contactInfo == contact)
        {
            return false;
        }
    }
    return true;
}
vector<int> Library::searchBook()
{
    string searchTerm;
    cout << "\n";
    cout << BOLD << YELLOW << "   Enter book title or author to search: " << RESET;
    getline(cin, searchTerm);
    vector<int> index;

    for (int i = 0; i < books.size(); i++)
    {
        if ((books[i].title.find(searchTerm) != string::npos ||
             books[i].author.find(searchTerm) != string::npos) &&
            books[i].isAvailable)
        {
            index.push_back(i);
        }
    }
    if (index.empty())
    {
        cout << "   " << RED << "No books found matching the criteria." << RESET << endl;
    }
    return index;
}
int Library::viewBorrowedBooks(string mid)
{
    vector<int> indexs = searchBook(mid);
    if (indexs.empty())
    {
        cout << "   " << RED << "You have no borrowed books." << RESET << endl;
        return -1;
    }

    // Use existing box helpers for a cleaner look
    printBoxTop(" YOUR BORROWED BOOKS ");

    // Header row (widths chosen to fit the box)
    cout << "   " << MAGENTA << BOLD
         << setw(4) << left << "No."
         << setw(30) << left << "Title"
         << setw(16) << left << "Author"
         << setw(12) << left << "ISBN"
         << RESET << "\n";

    cout << "   " << CYAN << string(62 - 8, '-') << RESET << "\n";

    for (int i = 0; i < (int)indexs.size(); i++)
    {
        int j = indexs[i];
        string title = books[j].title;
        string author = books[j].author;

        // Truncate nicely to avoid breaking columns
        if (title.length() > 28)
            title = title.substr(0, 25) + "...";
        if (author.length() > 14)
            author = author.substr(0, 11) + "...";

        cout << "   " << YELLOW << setw(4) << left << (i + 1) << RESET;
        cout << " " << BLUE << setw(30) << left << title << RESET;
        cout << " " << MAGENTA << setw(16) << left << author << RESET;
        cout << " " << CYAN << setw(12) << left << books[j].isbn << RESET << "\n";
    }

    printBoxBottom();
    return 0;
}
vector<int> Library::searchBook(string mid)
{
    vector<int> index;
    for (int i = 0; i < books.size(); i++)
    {
        if (books[i].borrowerId == mid)
        {
            index.push_back(i);
        }
    }

    return index;
}
int main()
{
    Library lib;
    int choice = -1;
    cout << "\n";
    cout << "   " << CYAN << BOLD << "##############################################################" << RESET << "\n";
    cout << "   " << CYAN << BOLD << "#              Welcome to Book Library                       #" << RESET << "\n";
    cout << "   " << CYAN << BOLD << "##############################################################" << RESET << "\n\n";
    while (true)
    {
        cout << BOLD << "   Main Menu" << RESET << "\n";
        cout << "   " << BLUE << "1." << RESET << " Add Book\n";
        cout << "   " << BLUE << "2." << RESET << " Add Member\n";
        cout << "   " << BLUE << "3." << RESET << " Login Member\n";
        cout << "   " << BLUE << "0." << RESET << " Exit\n";
        cout << "   " << YELLOW << "Enter your choice: " << RESET;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "   " << RED << "Please enter a number!" << RESET << "\n\n";
            continue;
        }
        if (choice == 0)
        {
            cout << "\n";
            cout << "   " << GREEN << "Goodbye. Thank you for using Book Library." << RESET << "\n";
            break;
        }
        switch (choice)
        {
        case 1:
            cout << "\n";
            cout << "   " << CYAN << "Add a new book" << RESET << "\n";
            lib.addBook();
            cout << "\n";
            break;
        case 2:
            cout << "\n";
            cout << "   " << CYAN << "Register a new member" << RESET << "\n";
            lib.addMember();
            lib.sortMembersById();
            break;
        case 3:
            cout << "\n";
            cout << "   " << CYAN << "Member login" << RESET << "\n";
            if (lib.LoginMember() == -1)
            {
                cout << "\n";
                cout << "   " << RED << "Login failed or member not found." << RESET << "\n\n";
            }
            else
            {
                cout << "\n";
                cout << "   " << GREEN << "Logged out." << RESET << "\n\n";
            }
            break;
        default:
            cout << "   " << RED << "Invalid choice. Please try again." << RESET << "\n\n";
            break;
        }
    }
    return 0;
}