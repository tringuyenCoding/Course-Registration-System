#ifndef CS162_PROJECT_SYSTEM
#define CS162_PROJECT_SYSTEM
#include <string>

typedef unsigned short WORD;

const std::string DATA_DIR = "data/";
const std::string STUDENT_DIR = "data/student/";
const std::string CLASS_TXT = "data/class.txt";
const std::string SCHOOL_TXT = "data/school_year.txt";
const std::string STAFF_TXT = "data/staff.txt";
const std::string SEM_TXT = "semester_info.txt";

/**
 * Folder related functions
 **/

void rmdir(std::string path);
void mkdir(std::string path);

/**
 * File related functions
 **/

void SetBGColor(WORD color);
void gotoxy(short x, short y);
void SetColor(WORD color);
void clrscr();
char getch();
void pressKeyToContinue();

#endif /* CS162_PROJECT_SYSTEM */
