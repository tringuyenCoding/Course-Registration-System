#ifndef CS162_PROJECT_INFO
#define CS162_PROJECT_INFO
#include <string>
#include <iosfwd>

const int DAY_INIT = 32;
const int MONTH_INIT = 13;
const int YEAR_INIT = 9999;

// For date input and check score date
class Date
{
private:
    bool checkDayValid();

public:
    int day = DAY_INIT, month = MONTH_INIT, year = YEAR_INIT;
    friend std::istream &operator>>(std::istream &in, Date &date);
    friend std::ostream &operator<<(std::ostream &out, const Date &date);
    void consoleInputDate(std::string call);
};

// For user login and change password
struct Login
{
    std::string username, password;
    bool checkPassword(std::string, std::string);
    static std::string inputPassword();
    bool changePassword();
};

void inputAccount(std::string &username, std::string &password);

int datecmp(const Date &d1, const Date &d2);

std::string addZero(int val);

#endif /* CS162_PROJECT_INFO */
