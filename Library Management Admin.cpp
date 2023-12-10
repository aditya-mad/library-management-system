#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <time.h>
#include <conio.h>
using namespace std;

class Books;
class User;
class Librarian;

bool Validate_Login(unordered_map<string, string>, string);
unordered_map<string, string> Store_In_Map(string);
int Calculate_Time(int, int, int, int, int, int);

const string BOOKS_FILE_NAME = "all_books.txt";
const string USER_LOGIN_FILE_NAME = "user_login_details.txt";
const string ADMIN_LOGIN_FILE_NAME = "admin_login_details.txt";
const string USER_ALL_FILE_NAME = "user_all_details.txt";
const string FINES_DATES = "issue_dates.txt";
const string PAID_FINES = "paid_fines.txt";

const int QUIT_KEY = 27;
const int GO_BACK_KEY = 8;

string rollNumber;
unordered_map<string, string> adminDetails;
unordered_map<string, string> userDetails;

class Books
{
public:
    unordered_map<string, int> AllBooks;
    bool is_book_available(string);
    void all_books_currently_available();
};

class User : public Books
{
protected:
    unordered_map<string, vector<string>> AllUsersDetails;
    unordered_map<string, int> AllUsersFines;

public:
    string current_roll_no;
    void borrow_book(Librarian, string, string);
    void renew_book(Librarian, string, string);
    void return_book(Librarian, string, string);
    void pay_fines(Librarian, string);
};

class Librarian : public User
{
public:
    unordered_map<string, vector<pair<int, pair<int, int>>>> AllBookIssueDates;
    unordered_map<string, int> AllUsersPaidFinesRecord;

    void store_books_to_map();
    void update_books_to_file();
    void store_users_to_map();
    void update_users_to_file();
    void show_all_borrowed_books();
    void edit_book_name(string, string);
    void add_new_book(string, int);
    void store_book_issue_dates_to_map();
    void update_book_issue_dates_to_file();
    bool borrowerd_any_book(string);
    void show_all_current_fines_record();
    void librarian_options();
    void store_paid_fines_to_map();
    void update_paid_fines_to_file();
    void show_all_previous_fines_record();
    void user_options();
    void add_user(string, string);
    void borrow_book(string, string);
    void renew_book(string, string, int);
    void return_book(string, string, int);
    void pay_fines(string);
};

int main()
{
    adminDetails = Store_In_Map(ADMIN_LOGIN_FILE_NAME);
    userDetails = Store_In_Map(USER_LOGIN_FILE_NAME);

    system("CLS");

    while (true)
    {
        if (Validate_Login(adminDetails, "Librarian"))
        {
            break;
        }
    }

    Librarian one;
    one.store_books_to_map();
    one.store_users_to_map();
    one.store_book_issue_dates_to_map();
    one.store_paid_fines_to_map();

    system("color 3F");

    while (true)
    {
        system("CLS");
        cout << "---------- Library Management System ----------" << endl;
        cout << "1. Librarian options" << endl;
        cout << "2. User options" << endl;
        cout << "Press Escape key to exit" << endl;
        cout << "Your Choice - " << endl;
        char start = getch();

        if (start == QUIT_KEY)
        {
            one.update_users_to_file();
            one.update_books_to_file();
            one.update_paid_fines_to_file();
            one.update_book_issue_dates_to_file();
            return 0;
        }
        else if (start == '1')
        {
            system("CLS");
            if (Validate_Login(adminDetails, "Librarian"))
            {
                one.librarian_options();
            }
        }
        else if (start == '2')
        {
            system("CLS");
            cout << "--USER LOGIN OPTIONS--" << endl;
            cout << "1. New User" << endl;
            cout << "2. Existing User" << endl;
            cout << "Press Escape to go back " << endl;
            int choice = getch();

            if (choice == QUIT_KEY)
            {
                continue;
            }
            else if (choice == '1')
            {
                system("CLS");

                bool flag = true;
                string rollNo, passWord;
                cout << "--NEW USER LOGIN--" << endl;
                cout << "Enter your roll no - ";
                cin >> rollNo;
                cout << "Enter password - ";
                cin >> passWord;
                for (auto x : userDetails)
                {
                    if (x.first == rollNo)
                    {
                        flag = false;
                        break;
                    }
                }

                if (flag)
                {
                    one.current_roll_no = rollNumber;
                    one.add_user(rollNo, passWord);
                    one.user_options();
                }
                else
                {
                    cout << "User name already exist" << endl;
                    cout << "Press any key to continoue" << endl;
                    getch();
                }
            }
            else if (choice == '2')
            {
                if (Validate_Login(userDetails, "EXISTING USER"))
                {
                    one.current_roll_no = rollNumber;
                    one.user_options();
                }
            }
        }
    }

    return 0;
}

unordered_map<string, string> Store_In_Map(string file_name)
{
    unordered_map<string, string> details;

    ifstream data(file_name);
    string userName, passWord;

    while (data >> userName)
    {
        data >> passWord;
        details[userName] = passWord;
    }

    return details;
}

bool Validate_Login(unordered_map<string, string> details, string user_type)
{

    for (int i = 0; i < 3; i++)
    {
        string userName, passWord;

        system("CLS");
        cout << "--" << user_type << " LOGIN--" << endl;
        cout << "Enter username - ";
        cin >> userName;
        cout << "Enter password - ";
        cin >> passWord;

        if (details[userName] == passWord)
        {
            rollNumber = userName;
            return true;
        }
    }
    return false;
}

int Calculate_Time(int issue_date, int issue_month, int issue_year, int return_date, int return_month, int return_year)
{
    int totalDays;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (issue_month == return_month)
    {
        totalDays = return_date - issue_date + 1;
    }
    else
    {
        for (int i = issue_month; i < return_month; i++)
        {
            totalDays += daysInMonth[i];
        }
        totalDays += return_date - issue_date + 1;
    }

    return totalDays;
}

bool Books::is_book_available(string book_name)
{
    for (auto search : AllBooks)
    {
        if (search.first == book_name && search.second > 0)
        {
            return true;
        }
    }
    return false;
}

void Books::all_books_currently_available()
{
    for (auto book : AllBooks)
    {
        if (book.second > 0)
        {
            cout << book.first << "\t-\t" << book.second << endl;
        }
    }
}

void Librarian::user_options()
{
    while (true)
    {
        string bookName;
        bool flag = false;
        int index = 0;
        char temp;
        system("CLS");

        cout << "-----------USER Options----------" << endl;
        cout << "1. Borrow Book" << endl;
        cout << "2. Return Book" << endl;
        cout << "3. Renew Book" << endl;
        cout << "4. Pay Fines" << endl;
        cout << "5. See All Available Books" << endl;
        cout << "6. Check Book Availability" << endl;
        cout << "Press Escape key to go back" << endl;
        cout << "Your Choice - ";
        char choice = getch();

        if (choice == QUIT_KEY)
        {
            break;
        }

        switch (choice)
        {

        case '1':
            system("CLS");
            cout << "--BORROW BOOK--" << endl;

            if (AllUsersFines[current_roll_no] > 0)
            {
                cout << "You have some unpaid fines. Pay them first to borrow a new book." << endl;
                cout << "Press any key to go back" << endl;
                getch();
            }
            else
            {
                cout << "Enter Book name - ";
                cin >> bookName;

                if (is_book_available(bookName))
                {
                    borrow_book(current_roll_no, bookName);
                    cout << "Book issue successful." << endl;
                    cout << "Press any key to go back" << endl;
                    getch();
                }
                else
                {
                    cout << "Book issue failed. Book is currently unavalilable." << endl;
                    cout << "Press any key to go back" << endl;
                    getch();
                }
            }

            break;

        case '2':

            system("CLS");
            cout << "--RETURN BOOK--" << endl;
            cout << "Enter Book name - ";
            cin >> bookName;

            for (auto x : AllUsersDetails[current_roll_no])
            {
                if (x == bookName)
                {
                    flag = true;
                    break;
                }
                index++;
            }

            if (flag)
            {
                return_book(current_roll_no, bookName, index);
                cout << "Book returned successfully" << endl;
                cout << "Press any key to go back" << endl;
                getch();
            }
            else
            {
                cout << "You didnot borrow any book with name " << bookName << endl;
                cout << "Press any key to go back" << endl;
                getch();
            }
            break;

        case '3':
            system("CLS");
            cout << "--RENEW BOOK--" << endl;
            cout << "Enter Book name - ";
            cin >> bookName;

            for (auto x : AllUsersDetails[current_roll_no])
            {
                if (x == bookName)
                {
                    flag = true;
                    break;
                }
                index++;
            }

            if (flag)
            {
                renew_book(current_roll_no, bookName, index);
                cout << "Book renewed successfully" << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }
            else
            {
                cout << "You didnot borrow any book with name " << bookName << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }
            break;

        case '4':

            system("CLS");
            cout << "--PAY FINES--" << endl;

            if (AllUsersFines[current_roll_no] > 0)
            {
                cout << "You have a fine of Rs." << AllUsersFines[current_roll_no] << endl;
                cout << "Press escape to cancel payment and any other key to pay" << endl;
                temp = getch();

                if (temp == QUIT_KEY)
                {
                    break;
                }
                cout << "Fine of Rs." << AllUsersFines[current_roll_no] << "collected" << endl;
                pay_fines(current_roll_no);
                cout << "Press any key to go back" << endl;
                getch();
            }
            else
            {
                cout << "This user don't have any fines to pay" << endl;
                cout << "Press any key to go back" << endl;
                getch();
            }

            break;
        case '5':
            system("CLS");
            cout << "--ALL BOOKS--" << endl;
            all_books_currently_available();
            cout << "Press any key to go back" << endl;
            getch();

            break;

        case '6':

            system("CLS");
            cout << "--BOOK AVALILABILITY--" << endl;
            cout << "Enter Book name - ";
            cin >> bookName;

            if (is_book_available(bookName))
            {
                cout << "Book is currently available" << endl;
                cout << "Press any key to go back" << endl;
                getch();
            }
            else
            {
                cout << "Book is currently unavailable" << endl;
                cout << "Press any key to go back" << endl;
                getch();
            }

            break;
        default:
            break;
        }
    }
}

void Librarian::librarian_options()
{
    while (true)
    {
        system("CLS");

        string rollNo, bookName, bookName2, password;
        bool flag = false;
        int index = 0, quantity;
        char choice1, choice2, choice, temp;

        cout << "-----------Librarian Options----------" << endl;
        cout << "1. Issue Book" << endl;
        cout << "2. Collect Book" << endl;
        cout << "3. Renew Book" << endl;
        cout << "4. Fines Options" << endl;
        cout << "5. Books Options" << endl;
        cout << "6. Add User" << endl;
        cout << "Press Escape to go back" << endl;
        cout << "Your Choice - ";
        choice = getch();

        if (choice == QUIT_KEY)
        {
            break;
        }

        switch (choice)
        {
        case '1':
            system("CLS");
            cout << "--BOOK ISSUE--" << endl;
            cout << "Enter Roll No - ";
            cin >> rollNo;
            cout << "Enter Book name - ";
            cin >> bookName;

            if (is_book_available(bookName))
            {
                borrow_book(rollNo, bookName);
                cout << "Book issue successful." << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }
            else
            {
                cout << "Book issue failed. Book is currently unavalilable." << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }

            break;

        case '2':
            system("CLS");
            cout << "--COLLECT BOOK--" << endl;
            cout << "Enter Roll No - ";
            cin >> rollNo;
            cout << "Enter Book name - ";
            cin >> bookName;

            for (auto x : AllUsersDetails[rollNo])
            {
                if (x == bookName)
                {
                    flag = true;
                    break;
                }
                index++;
            }

            if (flag)
            {
                return_book(rollNo, bookName, index);
                cout << "Book collected successfully" << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }
            else
            {
                cout << "The user didnot borrow any book with name " << bookName << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }

            break;
        case '3':
            system("CLS");
            cout << "--RENEW BOOK--" << endl;
            cout << "Enter Roll No - ";
            cin >> rollNo;
            cout << "Enter Book name - ";
            cin >> bookName;

            for (auto x : AllUsersDetails[rollNo])
            {
                if (x == bookName)
                {
                    flag = true;
                    break;
                }
                index++;
            }

            if (flag)
            {
                renew_book(rollNo, bookName, index);
                cout << "Book renewed successfully" << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }
            else
            {
                cout << "The user didnot borrow any book with name " << bookName << endl;
                cout << "Press any key to go back - " << endl;
                getch();
            }
            break;

        case '4':
            system("CLS");

            cout << "---FINES OPTIONS---" << endl;
            cout << "1. Collect Fines" << endl;
            cout << "2. View Paid Fines Record" << endl;
            cout << "3. View Current Fines Record " << endl;
            cout << "4. Total Fine Amount Collected" << endl;
            cout << "Press Escape key to go back - " << endl;
            cout << "Your Choice - ";
            choice1 = getch();
            if (choice1 == QUIT_KEY)
            {
                break;
            }
            switch (choice1)
            {
            case '1':
                system("CLS");
                cout << "--COLLECT FINES--" << endl;
                cout << "Enter Roll No - ";
                cin >> rollNo;

                if (AllUsersFines[rollNo] > 0)
                {
                    cout << "User have a fine of Rs." << AllUsersFines[rollNo] << endl;
                    cout << "Press escape to cancel and any key to collect it" << endl;
                    temp = getch();

                    if (temp == QUIT_KEY)
                    {
                        break;
                    }

                    pay_fines(rollNo);
                    cout << "Fine collected successfully" << endl;
                    cout << "Press any key to go back - " << endl;
                    getch();
                }
                else
                {
                    cout << "User is clear of all Due. No fine to collect" << endl;
                    cout << "Press any key to go back - " << endl;
                    getch();
                }

                break;

            case '2':
                system("CLS");
                cout << "-- ALL PAID FINES DETAILS--" << endl;
                show_all_previous_fines_record();
                cout << "Press any key to go back - " << endl;
                getch();

                break;

            case '3':
                system("CLS");
                cout << "-- ALL CURRENT FINES DETAILS--" << endl;
                show_all_current_fines_record();
                cout << "Press any key to go back - " << endl;
                getch();

                break;

            case '4':
                system("CLS");
                int amount = 0;
                for (auto x : AllUsersPaidFinesRecord)
                {
                    amount += x.second;
                }
                cout << "Total fine ammount is - Rs. " << amount << endl;
                cout << "Press any key to go back - " << endl;
                getch();

                break;
            }
            break;

        case '5':
            system("CLS");

            cout << "---BOOK OPTIONS---" << endl;
            cout << "1. Check Book Availability" << endl;
            cout << "2. View All Available Books" << endl;
            cout << "3. Show all borrowed Books" << endl;
            cout << "4. Add New Books" << endl;
            cout << "5. Edit Book Name" << endl;
            cout << "Press Escape key to go back - " << endl;
            cout << "Your Choice - ";
            choice2 = getch();

            if (choice1 == QUIT_KEY)
            {
                break;
            }

            switch (choice2)
            {
            case '1':
                system("CLS");
                cout << "--BOOK AVAILABILITY--" << endl;
                cout << "Enter Book name - ";
                cin >> bookName;

                if (is_book_available(bookName))
                {
                    cout << "Book is currently available" << endl;
                    cout << "Press any key to go back" << endl;
                    getch();
                }
                else
                {
                    cout << "Book is currently unavailable" << endl;
                    cout << "Press any key to go back" << endl;
                    getch();
                }

                break;

            case '2':
                system("CLS");
                cout << "--ALL BOOKS--" << endl;
                all_books_currently_available();
                cout << "Press any key to go back" << endl;
                getch();

                break;

            case '3':
                system("CLS");
                cout << "--ALL BORROWED BOOKS" << endl;
                show_all_borrowed_books();
                cout << "Press any key to go back" << endl;
                getch();

                break;

            case '4':
                system("CLS");
                cout << "-- ADD NEW BOOK--" << endl;
                cout << "Enter Book name - ";
                cin >> bookName;
                cout << "Enter Quantity - ";
                cin >> quantity;
                add_new_book(bookName, quantity);
                cout << "New book successfully added" << endl;
                cout << "Press any key to go back" << endl;
                getch();

                break;

            case '5':
                system("CLS");
                cout << "Enter Original Book name - ";
                cin >> bookName;
                cout << "Enter New Book name - ";
                cin >> bookName2;

                edit_book_name(bookName, bookName2);
                cout << "Book name changed successfully" << endl;
                cout << "Press any key to go back" << endl;
                getch();
                break;

            default:
                break;
            }

            break;

        case '6':
            system("CLS");
            cout << "--ADD USER--" << endl;
            cout << "Enter Roll No - ";
            cin >> rollNo;
            cout << "Enter date of birth - ";
            cin >> password;

            add_user(rollNo, password);
            break;

        default:
            break;
        }
    }
}

void Librarian::store_books_to_map()
{
    ifstream data(BOOKS_FILE_NAME);
    string bookName;

    while (data >> bookName)
    {
        int bookCount;
        data >> bookCount;
        AllBooks[bookName] = bookCount;
    }
}

void Librarian::update_books_to_file()
{
    ofstream data(BOOKS_FILE_NAME, ios::out);

    for (auto book : AllBooks)
    {
        data << book.first << " " << book.second << endl;
    }
}

void Librarian::store_users_to_map()
{
    ifstream data(USER_ALL_FILE_NAME);
    string userName;

    while (data >> userName)
    {
        int bookCount;
        data >> bookCount;
        vector<string> bookNames;
        string name;
        for (int i = 0; i < bookCount; i++)
        {
            data >> name;
            bookNames.push_back(name);
        }
        int fineCount;
        data >> fineCount;
        AllUsersDetails[userName] = bookNames;
        AllUsersFines[userName] = fineCount;
    }
}

void Librarian::update_users_to_file()
{
    ofstream data(USER_ALL_FILE_NAME, ios::out);

    for (auto user : AllUsersDetails)
    {
        data << user.first << " " << user.second.size() << " ";
        for (auto books : user.second)
        {
            data << books << " ";
        }
        data << AllUsersFines[user.first];
        data << endl;
    }
}

void Librarian::show_all_borrowed_books()
{
    cout << "User Id       -\t\tBooks" << endl;
    for (auto list : AllUsersDetails)
    {
        cout << list.first << " - ";
        for (auto book : list.second)
        {
            cout << book << "  ";
        }
        cout << endl;
    }
}

void Librarian::show_all_current_fines_record()
{
    cout << "User Id       -\t\tAmount" << endl;
    for (auto list : AllUsersFines)
    {
        cout << list.first << " - \t" << list.second << endl;
    }
}

void Librarian::edit_book_name(string original_name, string changed_name)
{
    AllBooks[changed_name] = AllBooks[original_name];
    AllBooks.erase(original_name);
}

void Librarian::store_book_issue_dates_to_map()
{
    ifstream data(FINES_DATES);
    string userName;

    while (data >> userName)
    {

        vector<pair<int, pair<int, int>>> allDates;
        pair<int, pair<int, int>> fullDate;

        for (int i = 0; i < AllUsersDetails[userName].size(); i++)
        {
            data >> fullDate.first;
            data >> fullDate.second.first;
            data >> fullDate.second.second;
            allDates.push_back(fullDate);
        }
        AllBookIssueDates[userName] = allDates;
    }
}

void Librarian::update_book_issue_dates_to_file()
{
    ofstream data(FINES_DATES, ios::out);

    for (auto list : AllBookIssueDates)
    {
        data << list.first << " ";
        for (auto dates : list.second)
        {
            data << dates.first << " " << dates.second.first << " " << dates.second.second << " ";
        }
        data << endl;
    }
}

bool Librarian::borrowerd_any_book(string roll_no)
{
    if (AllUsersDetails[roll_no].size() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Librarian::add_new_book(string book_name, int quantity)
{
    AllBooks[book_name] = quantity;
}

void Librarian::borrow_book(string roll_no, string book_name)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    pair<int, pair<int, int>> date;

    date.first = tm.tm_mday;
    date.second.first = tm.tm_mon + 1;
    date.second.second = tm.tm_year + 1900;

    AllBooks[book_name]--;
    AllUsersDetails[roll_no].push_back(book_name);
    AllBookIssueDates[roll_no].push_back(date);
}

void Librarian::renew_book(string roll_no, string book_name, int index)
{

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    pair<int, pair<int, int>> fullDate;

    int date, month, year;
    date = AllBookIssueDates[roll_no][index].first;
    month = AllBookIssueDates[roll_no][index].second.first;
    year = AllBookIssueDates[roll_no][index].second.second;
    cout << endl;

    int fine = Calculate_Time(date, month, year, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    fullDate.first = tm.tm_mday;
    fullDate.second.first = tm.tm_mon + 1;
    fullDate.second.second = tm.tm_year + 1900;

    AllBookIssueDates[roll_no][index] = fullDate;
    if (fine > 15)
    {
        fine = (fine - 15) * 2;
        AllUsersFines[roll_no] += fine;
    }
}

void Librarian::return_book(string roll_no, string book_name, int index)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int date, month, year;
    date = AllBookIssueDates[roll_no][index].first;
    month = AllBookIssueDates[roll_no][index].second.first;
    year = AllBookIssueDates[roll_no][index].second.second;
    cout << endl;
    int fine = Calculate_Time(date, month, year, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    AllUsersDetails[roll_no].erase(AllUsersDetails[roll_no].begin() + index);
    AllBookIssueDates[roll_no].erase(AllBookIssueDates[roll_no].begin() + index);
    AllBooks[book_name]++;
    if (fine > 15)
    {
        fine = (fine - 15) * 2;
        AllUsersFines[roll_no] += fine;
    }
}

void Librarian::pay_fines(string roll_no)
{
    int temp = AllUsersPaidFinesRecord[roll_no];
    AllUsersPaidFinesRecord[roll_no] += AllUsersFines[roll_no];
    AllUsersFines[roll_no] = 0;
}

void Librarian::store_paid_fines_to_map()
{
    ifstream data(PAID_FINES);
    string rollNumber;

    while (data >> rollNumber)
    {
        int fineAmount;
        data >> fineAmount;
        AllUsersPaidFinesRecord[rollNumber] = fineAmount;
    }
}

void Librarian::update_paid_fines_to_file()
{
    ofstream data(PAID_FINES, ios::out);

    for (auto fines : AllUsersPaidFinesRecord)
    {
        data << fines.first << " " << fines.second << endl;
    }
}

void Librarian::add_user(string roll_no, string password)
{
    vector<string> temp;
    AllUsersDetails[roll_no] = temp;
    AllUsersFines[roll_no] = 0;
    AllBookIssueDates[roll_no];
    userDetails[roll_no] = password;
}

void Librarian::show_all_previous_fines_record()
{
    cout << "User Id       -\tAmount Paid" << endl;
    for (auto list : AllUsersPaidFinesRecord)
    {
        cout << list.first << " - \t" << list.second << endl;
    }
}
