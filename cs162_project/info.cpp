#include "info.h"

#include "system.h"
#include <iostream>
#include <fstream>

std::istream &operator>>(std::istream &in, Date &date)
{
    char sep1, sep2;
    in >> date.day >> sep1 >> date.month >> sep2 >> date.year;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Date &date)
{
    out << date.day << "/" << date.month << "/" << date.year;
    return out;
}

void Date::consoleInputDate(std::string call)
{
    std::cout << call << " (dd/mm/yyyy): ";
    char sep1, sep2;
    std::cin >> day >> sep1 >> month >> sep2 >> year;

    while (std::cin.fail() || !checkDayValid())
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid date!\n";
        std::cout << call << " (dd/mm/yyyy): ";
        std::cin >> day >> sep1 >> month >> sep2 >> year;
    }
}

bool Date::checkDayValid()
{
    int leap;
    if (year % 4)
        leap = 0;
    else if (year % 100)
        leap = 1;
    else if (year % 400)
        leap = 0;
    else
        leap = 1;

    const int days[12] = {31, 28 + leap, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12)
        return 0;

    return (day >= 1 && day <= days[month - 1]);
}

bool Login::checkPassword(std::string username, std::string password)
{
    return this->username == username && this->password == password;
}

std::string Login::inputPassword()
{
    std::string password;
    char ch = getch();

    while (true)
    {
        if (ch == 13)
        {
            break;
        }
        else if (ch == 8)
        {
            if (password.size() > 0)
            {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else
        {
            password += ch;
            std::cout << '*';
        }
        ch = getch();
    }

    std::cout << '\n';

    return password;
}

bool Login::changePassword()
{
    clrscr();

    std::cout << "CHANGE PASSWORD\n\n";

    std::string check, n, recheck;
    std::cout << "Input old password: ";
    check = inputPassword();
    std::cout << "Input new password: ";
    n = inputPassword();
    std::cout << "Reinput new password: ";
    recheck = inputPassword();

    if (check != password || n != recheck || !n.size())
    {
        std::cout << "Change password failed!\n";
    }
    else if (n == password)
    {
        std::cout << "New password is the same as old password!\n";
    }
    else
    {
        password = n;
        std::cout << "Change password successfully!\n";
    }
    pressKeyToContinue();
    return (check == password && n == recheck);
}

void inputAccount(std::string &username, std::string &password)
{
    clrscr();
    std::cout << "LOGIN\n";
    std::cout << "---------------------------\n";
    std::cout << "Username: ";
    getline(std::cin, username);

    std::cout << "Password: ";
    password = Login::inputPassword();
}

int datecmp(const Date &d1, const Date &d2)
{
    if (d1.year != d2.year)
        return d1.year - d2.year;
    if (d1.month != d2.month)
        return d1.month - d2.month;
    return d1.day - d2.day;
}

std::string addZero(int val)
{
    std::string str = std::to_string(val);
    if (val < 10)
        str = "0" + str;
    return str;
}
