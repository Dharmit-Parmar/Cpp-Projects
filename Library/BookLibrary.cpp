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

class Library
{
private:
    vector<Book> books;
    vector<Member> members;

public:
    string genrateIsbn();

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

        for (int i = 9; i!=0; i--)
        {
            int rad = ds(gen);

            char temp = first12[i];
            first12[i] = first12[rad];
            first12[rad] = temp;
        }
        if(first12[0] == '0') {
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

int main()
{
    Library l;
    cout << l.genrateMemberId() << "\t"<< l.genrateMemberId().length();
    

    return 0;
}