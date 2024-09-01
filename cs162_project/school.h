#ifndef CS162_PROJECT_SCHOOL
#define CS162_PROJECT_SCHOOL
#include "fwd.h"

class Session
{
    const int max_day = 6, max_time = 4;
    const std::string days[6] = {"MON", "TUE", "WED", "THU", "FRI", "SAT"};
    const std::string times[4] = {"S1(07:30)", "S2(09:30)", "S3(13:30)", "S4(15:30)"};
    int day = 0, time = 0;

public:
    void printSession();
    void setDay(int);
    void setTime(int);
    std::string getDay();
    std::string getTime();
    void getDayTimeInInt(int &, int &);
    Session &operator=(Session &);
    bool operator==(Session &);
};

struct Mark
{
    std::string id;
    float midterm = -1.0;
    float final = -1.0;
    float other = -1.0;
    float total = -1.0;
    Mark *next = nullptr;
};

struct Course
{
    std::string id;
    std::string name;
    std::string teacher_name;
    int credits = 0;
    int student_max = 50; // By default, the maximum student will be 50
    Session class_session[2];
    Mark *mark = nullptr;
    Course *next = nullptr;

    void printCourse();

    ~Course();
};

struct Semester
{
    Date begin_date, end_date;
    Date reg_begin_date, reg_end_date;
    Course *course_list = nullptr;

    ~Semester();
};

struct SchoolYear
{
    int begin_year = 9999, end_year = 9999;
    Semester semester[3]{};
    SchoolYear *next = nullptr;
};

void initSchoolYearList(Student *student, SchoolYear *&school_year);
void cacheSchoolYearList(SchoolYear *school_year);

int countYears(SchoolYear *school_year);
int countSemesters(SchoolYear *school_year);
int countMarks(Mark *mark);

bool checkSchoolYearValid(SchoolYear *school_year);

bool checkTimeOverlap(Session s1[], Session s2[]);

void addCoursesNode(Course *&course_list);
void deleteCourseNode(Student *student, std::string id, int year, int semester, std::string course_id);
bool viewCourses(SchoolYear *school_year, int semester_index);

SchoolYear *chooseSchoolYear(SchoolYear *school_year, int begin_year = 0);
Course *chooseCourse(SchoolYear *school_year, int semester_index, std::string course_id, int begin_year = 0);

float calculateTotalGPA(SchoolYear *school_year, Student *student);
float calculateSemesterGPA(SchoolYear *current_year, Student *student, int semester_index);

#endif /* CS162_PROJECT_SCHOOL */
