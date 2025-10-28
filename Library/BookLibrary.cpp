#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

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

int Library::LoginMember()
{
    string memberId;
    cout << "Enter member ID: ";
    getline(cin, memberId);
    int index = -1;
   for(int i=0;i<members.size();i++)
   {
    if(members[i].memberId==memberId)
    {
        index=i;
        cout<<"Member logged in successfully."<<endl;
        break;
    }
   }
   if(index==-1)
   {
    cout<<"Member ID not found."<<endl;
   }

}

int main()
{
    Library l;
    cout << l.genrateMemberId() << "\t" << l.genrateMemberId().length();

    return 0;
}