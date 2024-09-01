#ifndef CS162_PROJECT_STUDENT
#define CS162_PROJECT_STUDENT
#include "fwd.h"

struct CourseEnrolled
{
    int year_enrolled = -1;
    int semester_index = -1;
    std::string course_id;
    CourseEnrolled *next = nullptr;
};

struct Student : Login, Date
{
    std::string id, current_class;
    std::string first_name, last_name;
    bool gender;
    std::string social_id;
    CourseEnrolled *enrolled = nullptr;
    Student *next = nullptr;

    Student(std::string, std::string,
            std::string, std::string, bool,
            int, int, int, std::string);

    Student(std::string, std::string,
            std::string, std::string, bool,
            int, int, int, std::string, std::string);

    void getInformation();

    ~Student();
};

Student *findStudent(Student *student, std::string username);
Student *addNewStudent(Student *&student, Student *new_student);
void initStudentList(Student *&student);
void cacheStudentList(Student *student);

void studentLogin(Student *&student, SchoolYear *&school_year, Date date);
void studentGUI(Student *&student, SchoolYear *&school_year, Date date);

/**
 * Task #12-15
 * Enrool course tasks
 **/

void enrollCourse(Student *student, SchoolYear *school_year, Date date);
void listOfEnrolledCourses(Student *student, SchoolYear *school_year);
void removeEnrolledCourse(Student *student, SchoolYear *school_year, Date date);

/**
 * Task #26
 * View the scoreboard
 **/

void viewScoreboard(Student *student, SchoolYear *school_year);

#endif /* CS162_PROJECT_STUDENT */
