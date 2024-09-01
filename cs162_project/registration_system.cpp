#include "registration_system.h"

#include "student.h"
#include "staff.h"
#include "school.h"
#include "system.h"
#include <iostream>

void courseRegistrationSystem()
{
    Date test_date;

    Student *student = nullptr;
    Staff *staff = nullptr;
    SchoolYear *school_year = nullptr;

    std::cout << "LOADING DATA...\n";

    initStaffList(staff);
    initStudentList(student);
    initSchoolYearList(student, school_year);

    int choice = -1;
    while (true)
    {
        clrscr();
        std::cout << "Welcome to the course registration system!\n";
        std::cout << "0. Exit\n";
        std::cout << "1. Student mode\n";
        std::cout << "2. Staff mode\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(1000, '\n');

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input!\n";
            pressKeyToContinue();
            continue;
        }

        switch (choice)
        {
        case 0:
            clrscr();
            std::cout << "GOOD BYE\n";
            cacheStaffList(staff);
            cacheStudentList(student);
            cacheSchoolYearList(school_year);
            return;
        case 1:
            std::cout << "LOADING...\n";
            studentLogin(student, school_year, test_date);
            break;
        case 2:
            std::cout << "LOADING...\n";
            staffLogin(staff, student, school_year);
            break;
        case 3:
            clrscr();
            test_date.consoleInputDate("Change date");
            break;
        default:
            std::cout << "Invalid choice!\n";
            pressKeyToContinue();
            break;
        }
    }
}
