#include <iostream> // Required for input (cin) and output (cout) operations
#include <string>   // Required for using std::string class (text manipulation)
#include <vector>   // Required for using std::vector (dynamic arrays)
#include <random>   // Required for random number generation (std::random_device, std::mt19937, std::uniform_int_distribution)
#include <algorithm> // Required for std::shuffle (although you implemented a manual shuffle, std::shuffle is in here)

// Using the 'std' namespace to avoid writing 'std::' before every standard library element.
// This can be convenient for smaller projects.
using namespace std;

/*
 * =========================================================================
 * ANSI Escape Codes for Console Colors
 * =========================================================================
 * These are preprocessor macros to define color constants. They allow you to
 * print colored text to the console, making the output more readable and
 * user-friendly. The compiler will replace these names with their string
 * values before compilation.
 */
#define RESET "\033[0m"   // Resets text color and formatting to default
#define RED "\033[31m"     // Sets text color to red
#define GREEN "\033[32m"   // Sets text color to green
#define YELLOW "\033[33m"  // Sets text color to yellow
#define BLUE "\033[34m"    // Sets text color to blue
#define CYAN "\033[36m"    // Sets text color to cyan
#define MAGENTA "\033[35m" // Sets text color to magenta
#define BOLD "\033[1m"     // Sets text to bold

/*
 * =========================================================================
 * Struct: Book
 * =========================================================================
 * Purpose: Represents a single book in the library catalog.
 * It holds all essential information about a book.
 * Members are public by default in a struct.
 */
struct Book
{
    string title;       // The title of the book (e.g., "The Hobbit")
    string author;      // The author of the book (e.g., "J.R.R. Tolkien")
    string isbn;        // The International Standard Book Number (unique identifier for the book)
    bool isAvailable;   // A flag indicating if the book is currently in the library (true) or checked out (false)
    string borrowerId;  // The Member ID of the person who currently has the book, empty if isAvailable is true
    // TODO: Add std::chrono::time_point for checkoutDate and dueDate for proper tracking
};

/*
 * =========================================================================
 * Struct: Member
 * =========================================================================
 * Purpose: Represents a single registered member of the library.
 * It stores personal details and a list of books they have borrowed.
 * Members are public by default in a struct.
 */
struct Member
{
    string name;            // The full name of the library member
    string memberId;        // A unique identifier for the member
    string contactInfo;     // Contact details like email or phone number
    vector<string> borrowBooksId; // A list of ISBNs of books currently borrowed by this member
};

/*
 * =========================================================================
 * Class: Library
 * =========================================================================
 * Purpose: Manages the entire library system, including all books and members.
 * It provides functionalities to generate IDs and will eventually handle
 * adding/removing books/members, checking out/in, etc.
 */
class Library
{
private:
    // --- Data Storage ---
    // These vectors will hold all Book and Member objects currently in the system.
    // TODO: Consider changing these to std::map<string, Book> and std::map<string, Member>
    // for faster lookup by ISBN/MemberID, as discussed.
    vector<Book> books;
    vector<Member> members;

public:
    // --- ID Generation Functions ---
    // These functions generate unique identifiers for books and members.

    /**
     * @brief Generates a pseudo-random 12-digit ISBN-like string.
     * This function creates a string starting with "978" and appends 9 random digits.
     * It then shuffles a portion of these digits to enhance randomness.
     * Note: This currently does NOT calculate the ISBN-13 check digit, making it
     * a 12-digit ID rather than a full 13-digit ISBN. It's also not guaranteed
     * to be unique within the library without external checking.
     *
     * @return A std::string representing a pseudo-random 12-digit ISBN-like identifier.
     */
    string genrateIsbn();

    /**
     * @brief Generates a pseudo-random 10-digit numeric Member ID string.
     * This function creates a 10-digit string using random numbers, shuffles
     * all its digits, and ensures the first digit is not '0' to avoid leading zeros.
     * Note: This ID is not guaranteed to be unique within the library without
     * external checking (e.g., checking against a std::map of existing members).
     *
     * @return A std::string representing a pseudo-random 10-digit Member ID.
     */
    string genrateMemberId();
};

/*
 * =========================================================================
 * Method: Library::genrateMemberId() Implementation
 * =========================================================================
 * Generates a pseudo-random 10-digit Member ID.
 */
string Library::genrateMemberId()
{
    // --- Random Number Generator Setup ---
    // static std::random_device rd: An object that tries to get true random numbers from the system.
    //   'static' ensures it's initialized only once across all calls to genrateMemberId().
    static std::random_device rd;
    // static std::mt19937 gen(rd()): A Mersenne Twister engine, a high-quality pseudo-random number generator,
    //   seeded by 'rd' for varied results each program run. 'static' means it's initialized once.
    static std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(0, 9): Defines a distribution for generating
    //   integers uniformly between 0 and 9 (inclusive).
    std::uniform_int_distribution<> dis(0, 9);

    // --- Generate Initial Digits ---
    std::string memberID_str = ""; // Initialize an empty string to build the ID
    for (int i = 0; i < 10; ++i) // Loop 10 times to get 10 digits
    {
        // Generate a random digit (0-9) using 'dis' and 'gen', convert to string, and append.
        memberID_str += std::to_string(dis(gen));
    }

    // --- Shuffle Digits (Fisher-Yates style) ---
    // This loop shuffles the digits of the generated ID string to ensure better randomness
    // and less predictability of digit sequence.
    // 'ds' distribution will pick a random index from 0 up to the current 'i'.
    // Note: The loop condition `i!=0` and distribution `ds(0,9)` can be improved for strict Fisher-Yates.
    // For a 10-digit string (indices 0-9), the loop should be `for (int i = 9; i > 0; --i)`
    // and `ds` should be `ds(0, i)`. The current implementation has a slight bias.
    std::uniform_int_distribution<> ds(0, 9); // Distribution for shuffle index (0-9)

    for (int i = 9; i != 0; i--) // Loop from the last index (9) down to 1
    {
        int rad = ds(gen); // Generate a random index 'rad' within the full 0-9 range

        // Swap the character at current index 'i' with the character at 'rad'
        char temp = memberID_str[i];     // Store char at 'i' temporarily
        memberID_str[i] = memberID_str[rad]; // Replace char at 'i' with char from 'rad'
        memberID_str[rad] = temp;        // Place the temp char (original 'i') into 'rad'
    }

    // --- Ensure First Digit is Not Zero ---
    // This check prevents Member IDs from having leading zeros, which can sometimes be
    // confusing or problematic if they were ever treated as numbers.
    if (memberID_str[0] == '0')
    {
        // Replaces the first digit with a random digit from 1-9.
        // `random() % 9 + '1'` uses the non-standard `random()` function.
        // It's recommended to use `std::uniform_int_distribution(1,9)(gen)` instead.
        memberID_str[0] = random() % 9 + '1'; // Ensure first digit is not zero
    }
    return memberID_str; // Return the 10-digit shuffled Member ID
}

/*
 * =========================================================================
 * Method: Library::genrateIsbn() Implementation
 * =========================================================================
 * Generates a pseudo-random 12-digit ISBN-like string.
 */
string Library::genrateIsbn()
{
    // --- Random Number Generator Setup ---
    // (Same setup as in genrateMemberId for efficiency and consistent randomness)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9); // Distribution for single digits (0-9)

    // --- Generate Initial Digits ---
    std::string isbn_str = "978"; // ISBN-13s typically start with "978" (Bookland prefix)
    for (int i = 0; i < 9; ++i)   // Generate 9 more random digits to make it 3 + 9 = 12 digits total
    {
        // Append a random digit (0-9) to the ISBN string
        isbn_str += std::to_string(dis(gen));
    }

    // --- Shuffle Digits (Fisher-Yates style) ---
    // This shuffle is applied to the digits *after* the "978" prefix.
    // The loop iterates over indices from 11 down to 3 (inclusive), ensuring
    // that the '978' prefix at indices 0, 1, 2 remains untouched.
    // 'ds' distribution will pick a random index from 3 up to the current 'i'.
    // Note: The original code's `ds(3,11)` and `i > 2` with `ds` picking from 3 to 11
    // has a slight bias compared to a perfect Fisher-Yates shuffle where `ds` would be `ds(3,i)`.
    std::uniform_int_distribution<> ds(3, 11); // Distribution for shuffle index (3-11)

    for (int i = 11; i > 2; i--) // Loop from index 11 down to 3
    {
        int rad = ds(gen); // Generate a random index 'rad' within the shuffle range (3-11)

        // Swap the character at current index 'i' with the character at 'rad'
        char temp = isbn_str[i];     // Store char at 'i' temporarily
        isbn_str[i] = isbn_str[rad]; // Replace char at 'i' with char from 'rad'
        isbn_str[rad] = temp;        // Place the temp char (original 'i') into 'rad'
    }
    // TODO: Implement the ISBN-13 checksum algorithm here to calculate and append the 13th check digit.
    // This will make it a truly valid ISBN-13.

    return isbn_str; // Return the 12-digit (currently) shuffled ISBN-like string
}

/*
 * =========================================================================
 * Function: main() - Program Entry Point
 * =========================================================================
 * This is where the program execution begins.
 * Currently, it serves as a test bed for the ID generation functions.
 */
int main()
{
    Library l; // Create an instance of the Library class

    // Test the Member ID generation:
    // Calls genrateMemberId() twice and prints the generated ID and its length.
    cout << "Generated Member ID: " << l.genrateMemberId() << "\tLength: " << l.genrateMemberId().length() << endl;
    cout << "Generated Member ID: " << l.genrateMemberId() << "\tLength: " << l.genrateMemberId().length() << endl;

    // Test the ISBN generation (uncomment to test):
    // cout << "Generated ISBN: " << l.genrateIsbn() << "\tLength: " << l.genrateIsbn().length() << endl;
    // cout << "Generated ISBN: " << l.genrateIsbn() << "\tLength: " << l.genrateIsbn().length() << endl;

    return 0; // Indicates successful program execution
}