#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";
const std::string MAGENTA = "\033[35m";
const std::string BOLD = "\033[1m";

// small helper to keep consistent left margin and coloring
inline void pm(const std::string &msg, const std::string &color = RESET)
{
    std::cout << "   " << color << msg << RESET << "\n";
}

struct Book
{
    std::string title;
    std::string author;
    std::string isbn;
    bool isAvailable;
    std::string borrowerId;
};

struct Member
{
    std::string name;
    std::string memberId;
    std::string pin;
    std::string contactInfo;
    std::vector<std::string> borrowBooksId;
};

struct LibraryManager
{
    std::string memberid;
    std::string isbn;
};

class Library
{
private:
    std::vector<Book> books;
    std::vector<Member> members;

    // UI helpers (only formatting, no logic changes)
    void printBoxTop(const std::string &title)
    {
        std::cout << CYAN << BOLD << "#===========================================================" << RESET << "\n";
        std::cout << CYAN << BOLD << "| " << std::setw(58) << std::left << title << " |" << RESET << "\n";
        std::cout << CYAN << BOLD << "#===========================================================" << RESET << "\n";
    }

    void printBoxBottom()
    {
        std::cout << CYAN << BOLD << "#===========================================================" << RESET << "\n";
    }

    void printLine(const std::string &msg, const std::string &color = RESET)
    {
        std::cout << "   " << color << msg << RESET << "\n";
    }

    void printSectionTitle(const std::string &title)
    {
        std::cout << "\n";
        std::cout << MAGENTA << BOLD << "  >> " << title << RESET << "\n";
        std::cout << "   " << CYAN << "------------------------------------------------------------" << RESET << "\n";
    }

public:
    void addBook()
    {
        printBoxTop(" ADD NEW BOOK ");
        Book newBook;
        std::string title, author;
        printLine("Enter book title: ", YELLOW);
        std::cout << "     " << BOLD;
        std::getline(std::cin, title);
        std::cout << RESET;
        printLine("Enter book author: ", YELLOW);
        std::cout << "     " << BOLD;
        std::getline(std::cin, author);
        std::cout << RESET;
        newBook.title = title;
        newBook.author = author;
        newBook.isbn = genrateIsbn();
        newBook.isAvailable = true;
        books.push_back(newBook);
        printLine("");
        printLine("Book added successfully!", GREEN);
        printLine(std::string("ISBN: ") + BLUE + newBook.isbn + RESET);
        printBoxBottom();
    }
    bool checkContact(std::string contact);
    void addMember();
    int LoginMember();
    std::string genrateIsbn();
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
    std::string genrateMemberId();
    std::vector<int> searchBook();
    std::vector<int> searchBook(std::string mid);
    int borrowBook(std::string mid, int index);
    int returnBook(std::string mid, int index);
    bool checkPina(std::string mid, std::string pin)
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
    int viewBorrowedBooks(std::string mid);
    bool writeDataToFile();
    bool readDataFromFile();
};
std::string Library::genrateMemberId()
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
        first13[0] = rand() % 9 + '1'; // ensure first digit is not zero
    }

    return first13;
}

std::string Library::genrateIsbn()
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
    std::cout << "\n";
    std::cout << "   " << CYAN << BOLD << "#================================================#" << RESET << "\n";
    std::cout << "   " << CYAN << BOLD << "|               REGISTER MEMBER                  |" << RESET << "\n";
    std::cout << "   " << CYAN << BOLD << "#================================================#" << RESET << "\n";

    Member newMember;
    std::string name, contactInfo, PIn;
    std::cout << "   " << YELLOW << "Enter member name: " << RESET;
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "   " << YELLOW << "Enter contact info (must be 10 number): " << RESET;

    std::getline(std::cin, contactInfo);
    if ((contactInfo.size() != 10) || (!checkContact(contactInfo)))
    {
        std::cout << "   " << RED << "Contact info must be 10 digits long and should new" << RESET << std::endl;
        std::cout << "   " << CYAN << "#================================================#" << RESET << "\n";
        return;
    }
    std::cout << "   " << YELLOW << "Set a 4-digit PIN for your account: " << RESET;
    std::cin >> PIn;
    if (PIn.size() != 4)
    {
        std::cout << "   " << RED << "PIN must be 4 digits long." << RESET << std::endl;
        std::cout << "   " << CYAN << "#================================================#" << RESET << "\n";
        return;
    }
    newMember.pin = PIn;
    newMember.name = name;
    newMember.contactInfo = contactInfo;

    newMember.memberId = genrateMemberId();

    members.push_back(newMember);
    std::cout << "   " << GREEN << "Member added successfully!" << RESET << std::endl;
    std::cout << "   " << BLUE << "Member ID: " << MAGENTA << newMember.memberId << RESET << std::endl;
    std::cout << "   " << CYAN << "#================================================#" << RESET << "\n\n";
}
void menuForLogin()
{
    pm("1. Borrow Book   2. Return Book   3. View Borrowed Books   4. Logout", BLUE);
}

int Library::LoginMember()
{
    std::string idd;
    std::string memberId, pin;
    int choice;
    std::cout << "\n";
    std::cout << "   " << CYAN << BOLD << "#===================================================#" << RESET << "\n";
    std::cout << "   " << CYAN << BOLD << "|                   MEMBER LOGIN                     |" << RESET << "\n";
    std::cout << "   " << CYAN << BOLD << "#===================================================#" << RESET << "\n\n";

    std::cout << "   " << YELLOW << "Enter member ID : OR Contact No." << RESET;
    std::cin.clear();
    std::cin >> idd;

    int index = -1;
    if (idd.length() == 10)
    {
        std::string contact = idd;
        memberId = "";
        {
            for (int i = 0; i < members.size(); i++)
            {
                if (members[i].contactInfo == contact)
                {
                    std::cout << "   " << YELLOW << "Enter your 4-digit PIN: " << RESET;
                    std::cin >> pin;
                    if (checkPina(members[i].memberId, pin) == false)
                    {
                        std::cout << "   " << RED << BOLD << "✖ Incorrect PIN." << RESET << "\n\n";
                        return -1;
                    }
                    index = i;
                    memberId = members[i].memberId;
                    std::cout << "   " << GREEN << BOLD << "✔ Member logged in successfully." << RESET << "\n";
                    std::cout << "   " << BLUE << "Welcome, " << RESET << MAGENTA << members[i].name << " (" << memberId << ")" << RESET << "\n\n";
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
                    std::cout << "   " << YELLOW << "Enter your 4-digit PIN: " << RESET;
                    std::cin >> pin;
                    if (checkPina(members[m].memberId, pin) == false)
                    {
                        std::cout << "   " << RED << BOLD << "✖ Incorrect PIN." << RESET << "\n\n";
                        return -1;
                    }
                    std::cout << "   " << GREEN << BOLD << "✔ Member logged in successfully." << RESET << "\n";
                    std::cout << "   " << BLUE << "Welcome, " << RESET << MAGENTA << members[m].name << RESET << "\n\n";
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
        std::cout << "   " << RED << BOLD << "✖ Member ID not found." << RESET << "\n\n";
        return -1;
    }
    // show the index in a cleaner way
    pm(std::string("Member index: ") + std::to_string(index), CYAN);
    std::cout << "   " << CYAN << "----------------------------------------------------" << RESET << "\n";
    menuForLogin();
    std::cout << "   " << CYAN << "----------------------------------------------------" << RESET << "\n";

    while (true)
    {
        std::cout << "\n";
        std::cout << BOLD << CYAN << "   Choose an action:" << RESET << "\n";
        menuForLogin();
        std::cout << "   " << YELLOW << "Enter your choice: " << RESET;
        std::cin >> choice;
        std::cin.ignore(); // to ignore the newline character after choice input

        switch (choice)
        {
        case 1:
            std::cout << "\n";
            std::cout << MAGENTA << BOLD << "   >>> Borrow Book <<<" << RESET << "\n\n";
            borrowBook(memberId, index);
            break;
        case 2:
            std::cout << "\n";
            std::cout << MAGENTA << BOLD << "   >>> Return Book <<<" << RESET << "\n\n";
            returnBook(memberId, index);
            break;
        case 3:
            std::cout << "\n";
            std::cout << MAGENTA << BOLD << "   >>> Your Borrowed Books <<<" << RESET << "\n\n";
            std::cout << "   " << CYAN << "(Listing borrowed books...)" << RESET << "\n\n";
            if (viewBorrowedBooks(memberId) == -1)
            {
                std::cout << "\n";
            }
            break;
        case 4:
            std::cout << "\n";
            std::cout << GREEN << BOLD << "   Logging out..." << RESET << "\n\n";
            return 0;
        default:
            std::cout << "   " << RED << "Invalid choice. Please try again." << RESET << "\n";
            break;
        }
    }
}

int Library::borrowBook(std::string mid, int index)
{
    int choice = -1;
    std::vector<int> foundBooks = searchBook();
    std::cout << "\n";
    std::cout << "   " << CYAN << BOLD << "=== Available Books ===" << RESET << std::endl;

    if (foundBooks.empty())
    {
        std::cout << "   " << RED << "No books available to borrow." << RESET << std::endl;
        return -1;
    }

    for (int i = 0; i < foundBooks.size(); i++)
    {
        std::cout << "   " << YELLOW << std::setw(3) << (i + 1) << RESET;
        std::cout << " " << BLUE << "Title: " << RESET << books[foundBooks[i]].title;
        std::cout << " " << MAGENTA << "| Author: " << RESET << books[foundBooks[i]].author << std::endl;
    }

    std::cout << std::endl
              << BOLD << "   Enter the book number you want to borrow: " << RESET;
    std::cin >> choice;
    choice--;

    if (choice < 0 || choice >= foundBooks.size())
    {
        std::cout << "   " << RED << "Invalid book selection." << RESET << std::endl;
        return -1;
    }
    int bookIndex = foundBooks[choice];
    if (!books[bookIndex].isAvailable)
    {
        std::cout << "   " << RED << "Sorry, this book is already borrowed." << RESET << std::endl;
        return -1;
    }
    books[bookIndex].isAvailable = false;
    books[bookIndex].borrowerId = mid;

    members[index].borrowBooksId.push_back(books[bookIndex].isbn);

    std::cout << "\n";
    std::cout << "   " << GREEN << "You have successfully borrowed: " << RESET << BLUE << "'" << books[bookIndex].title << "'" << RESET << std::endl;
    std::cout << "   " << MAGENTA << "Borrower ID: " << RESET << mid << "   " << MAGENTA << "ISBN: " << RESET << books[bookIndex].isbn << "\n";
    return 0;
}
int Library::returnBook(std::string mid, int index)
{
    std::vector<int> indexs = searchBook(mid);
    if (indexs.empty())
    {
        std::cout << "   " << RED << "You have no borrowed books to return." << RESET << std::endl;
        return -1;
    }

    int choice = -1;
    std::cout << "\n";
    std::cout << "   " << CYAN << BOLD << "Your Borrowed Books:" << RESET << "\n";
    for (int i = 0; i < indexs.size(); i++)
    {
        int j = indexs[i];
        std::cout << "     " << YELLOW << (i + 1) << ". " << RESET << books[j].title << " " << MAGENTA << "-" << RESET << " " << books[j].author << std::endl;
    }

    std::cout << "\n";
    std::cout << "   " << BOLD << "Which book would you like to return (enter number): " << RESET;
    std::cin >> choice;
    if (std::cin.fail() || choice < 1 || choice > (int)indexs.size())
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "   " << RED << "Invalid selection." << RESET << std::endl;
        return -1;
    }

    int bookIdx = indexs[choice - 1];

    // remove the ISBN from the member's borrowed list
    std::string isbn = books[bookIdx].isbn;
    std::cout << "   " << CYAN << "Returning ISBN: " << RESET << isbn << "\n";
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
    std::cout << "   " << GREEN << "Returned " << books[bookIdx].title << " successfully." << RESET << std::endl;
    return 0;
}
bool Library::checkContact(std::string contact)
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
std::vector<int> Library::searchBook()
{
    std::string searchTerm;
    std::cout << "\n";
    std::cout << BOLD << YELLOW << "   Enter book title or author to search: " << RESET;
    std::getline(std::cin, searchTerm);
    std::vector<int> index;

    for (int i = 0; i < books.size(); i++)
    {
        if ((books[i].title.find(searchTerm) != std::string::npos ||
             books[i].author.find(searchTerm) != std::string::npos) &&
            books[i].isAvailable)
        {
            index.push_back(i);
        }
    }
    if (index.empty())
    {
        std::cout << "   " << RED << "No books found matching the criteria." << RESET << std::endl;
    }
    return index;
}
int Library::viewBorrowedBooks(std::string mid)
{
    std::vector<int> indexs = searchBook(mid);
    if (indexs.empty())
    {
        std::cout << "   " << RED << "You have no borrowed books." << RESET << std::endl;
        return -1;
    }

    // Use existing box helpers for a cleaner look
    printBoxTop(" YOUR BORROWED BOOKS ");

    // Header row (widths chosen to fit the box)
    std::cout << "   " << MAGENTA << BOLD
              << std::setw(4) << std::left << "No."
              << std::setw(30) << std::left << "Title"
              << std::setw(16) << std::left << "Author"
              << std::setw(12) << std::left << "ISBN"
              << RESET << "\n";

    std::cout << "   " << CYAN << std::string(62 - 8, '-') << RESET << "\n";

    for (int i = 0; i < (int)indexs.size(); i++)
    {
        int j = indexs[i];
        std::string title = books[j].title;
        std::string author = books[j].author;

        // Truncate nicely to avoid breaking columns
        if (title.length() > 28)
            title = title.substr(0, 25) + "...";
        if (author.length() > 14)
            author = author.substr(0, 11) + "...";

        std::cout << "   " << YELLOW << std::setw(4) << std::left << (i + 1) << RESET;
        std::cout << " " << BLUE << std::setw(30) << std::left << title << RESET;
        std::cout << " " << MAGENTA << std::setw(16) << std::left << author << RESET;
        std::cout << " " << CYAN << std::setw(12) << std::left << books[j].isbn << RESET << "\n";
    }

    printBoxBottom();
    return 0;
}
std::vector<int> Library::searchBook(std::string mid)
{
    std::vector<int> index;
    for (int i = 0; i < books.size(); i++)
    {
        if (books[i].borrowerId == mid)
        {
            index.push_back(i);
        }
    }

    return index;
}

bool Library::writeDataToFile()
{
    std::ofstream outFileForBooks("Books_data.txt");
    std::ofstream outFileForMembers("Members_data.txt");

    if (!outFileForMembers || !outFileForBooks)
    {
        std::cout << "   " << RED << "Error opening one or more files for writing." << RESET << std::endl;

        return false;
    }

    outFileForBooks << "Books:\n";
    for (const auto &book : books)
    {

        outFileForBooks << book.title << "," << book.author << "," << book.isbn << "," << book.isAvailable << "," << book.borrowerId << "\n";
    }

    outFileForMembers << "Members:\n";
    for (const auto &member : members)
    {

        outFileForMembers << member.name << "," << member.memberId << "," << member.pin << "," << member.contactInfo;
        for (const auto &borrowedIsbn : member.borrowBooksId)
        {
            outFileForMembers << "," << borrowedIsbn;
        }
        outFileForMembers << "\n";
    }

    std::cout << "   " << GREEN << "Library data saved to files successfully." << RESET << std::endl;
    return true;
}

bool Library::readDataFromFile()
{
    std::ifstream inFileForBooks("Books_data.txt");
    std::ifstream inFileForMembers("Members_data.txt");

    if (!inFileForMembers || !inFileForBooks)
    {
        std::cout << "   " << RED << "Error opening data files for reading. Starting with empty state." << RESET << std::endl;
        return false;
    }

    books.clear();
    members.clear();

    std::string line;
    std::string segment;

    std::getline(inFileForBooks, line);

    while (std::getline(inFileForBooks, line))
    {

        std::stringstream ss(line);
        Book newBook;

        std::getline(ss, newBook.title, ',');

        std::getline(ss, newBook.author, ',');
        std::getline(ss, newBook.isbn, ',');

        std::string isAvailableStr;
        std::getline(ss, isAvailableStr, ',');

        try
        {
            newBook.isAvailable = std::stoi(isAvailableStr);
        }
        catch (...)
        {

            newBook.isAvailable = false;
        }

        std::getline(ss, newBook.borrowerId);

        books.push_back(newBook);
    }

    std::getline(inFileForMembers, line);

    while (std::getline(inFileForMembers, line))
    {

        std::stringstream ss(line);
        Member newMember;

        std::getline(ss, newMember.name, ',');
        std::getline(ss, newMember.memberId, ',');
        std::getline(ss, newMember.pin, ',');
        std::getline(ss, newMember.contactInfo, ',');

        std::string borrowedIsbn;
        while (std::getline(ss, borrowedIsbn, ','))
        {

            if (!borrowedIsbn.empty())
            {
                newMember.borrowBooksId.push_back(borrowedIsbn);
            }
        }

        members.push_back(newMember);
    }

    std::cout << "   " << GREEN << "Library data loaded successfully." << RESET << std::endl;
    return true;
}
int main()
{
    Library lib;
    lib.readDataFromFile();
    int choice = -1;
    std::cout << "\n";
    std::cout << "   " << CYAN << BOLD << "##############################################################" << RESET << "\n";
    std::cout << "   " << CYAN << BOLD << "#              Welcome to Book Library                       #" << RESET << "\n";
    std::cout << "   " << CYAN << BOLD << "##############################################################" << RESET << "\n\n";
    while (true)
    {
        std::cout << BOLD << "   Main Menu" << RESET << "\n";
        std::cout << "   " << BLUE << "1." << RESET << " Add Book\n";
        std::cout << "   " << BLUE << "2." << RESET << " Add Member\n";
        std::cout << "   " << BLUE << "3." << RESET << " Login Member\n";
        std::cout << "   " << BLUE << "0." << RESET << " Exit\n";
        std::cout << "   " << YELLOW << "Enter your choice: " << RESET;
        std::cin >> choice;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "   " << RED << "Please enter a number!" << RESET << "\n\n";
            continue;
        }
        std::cin.ignore();
        if (choice == 0)
        {
            std::cout << "\n";
            std::cout << "   " << GREEN << "Goodbye. Thank you for using Book Library." << RESET << "\n";
            lib.writeDataToFile();
            break;
        }
        switch (choice)
        {
        case 1:
            std::cout << "\n";
            std::cout << "   " << CYAN << "Add a new book" << RESET << "\n";
            lib.addBook();
            std::cout << "\n";
            break;
        case 2:
            std::cout << "\n";
            std::cout << "   " << CYAN << "Register a new member" << RESET << "\n";
            lib.addMember();
            lib.sortMembersById();
            break;
        case 3:
            std::cout << "\n";
            std::cout << "   " << CYAN << "Member login" << RESET << "\n";
            if (lib.LoginMember() == -1)
            {
                std::cout << "\n";
                std::cout << "   " << RED << "Login failed or member not found." << RESET << "\n\n";
            }
            else
            {
                std::cout << "\n";
                std::cout << "   " << GREEN << "Logged out." << RESET << "\n\n";
            }
            break;
        default:
            std::cout << "   " << RED << "Invalid choice. Please try again." << RESET << "\n\n";
            break;
        }
    }
    return 0;
}