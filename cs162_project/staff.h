#ifndef CS162_PROJECT_STAFF
#define CS162_PROJECT_STAFF
#include "fwd.h"

struct Staff : Login
{
    std::string name;
    Staff *next = nullptr;

    Staff();
    Staff(std::string, std::string, std::string);

    void getInformation();
};

Staff *findStaff(Staff *staff, std::string username);
void initStaffList(Staff *&staff);
void cacheStaffList(Staff *staff);

void staffLogin(Staff *staff, Student *&student, SchoolYear *&school_year);
void staffGUI(Staff *&staff, Student *&student, SchoolYear *&school_year);

void courseManagerGUI(Student *student, SchoolYear *&school_year);
void studentManagerGUI(Student *&student, SchoolYear *school_year);

/**
 * Task #1
 * Create a new school year
 **/

void createSchoolYear(SchoolYear *&school_year);

/**
 * Task #2,3,4
 * Create a new class
 **/

void createClass(Student *&student);
bool importClassFromCSV(Student *&student, std::string class_name);
bool importClassManually(Student *&student, std::string class_name);

/**
 * Task #6-11
 * Input Create a new Semester with Courses
 **/

void createSemester(SchoolYear *&school_year);
void addCourse(SchoolYear *school_year);
void viewCoursesList(SchoolYear *school_year);
void updateCourseInfo(SchoolYear *school_year);
void deleteCourse(SchoolYear *&school_year, Student *student);

/**
 * Task #17-20
 * View list of classes
 * View list of students in a class
 * View list of courses in a semster
 * View a list of student in a course
 **/

void viewClassesList(Student *student, bool pressKey = false);
void viewStudentsList(Student *student);
void viewStudentInACourse(SchoolYear *school_year, Student *student);

/**
 * Task #21-25
 * Export list of students in a course
 * Export the base csv file and input the new csv file (contains scores)
 * View the scoreboard of a course / class (courses / one semester + all semester + overall GPA)
 * Update the result
 **/

void exportStudentListToCSV(Student *student, SchoolYear *school_year);
void exportScoreTemplate(Student *student, SchoolYear *school_year);
void importScoresFromCSV(SchoolYear *school_year);
void updateStudentResult(Student *student, SchoolYear *school_year);
void viewScoreBoardOfCourse(Student *student, SchoolYear *school_year);
void viewScoreBoardOfClass(Student *student, SchoolYear *school_year);

#endif /* CS162_PROJECT_STAFF */