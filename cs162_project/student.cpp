#include "student.h"

#include "system.h"
#include "school.h"
#include <iostream>
#include <iomanip>
#include <fstream>

Student::Student(std::string id, std::string current_class,
                 std::string first_name, std::string last_name, bool gender,
                 int day, int month, int year, std::string social_id)
{
    this->id = id;
    this->current_class = current_class;
    this->first_name = first_name;
    this->last_name = last_name;
    this->gender = gender;
    this->social_id = social_id;
    this->day = day;
    this->month = month;
    this->year = year;
    this->password = addZero(day) + addZero(month) + std::to_string(year);
    this->username = this->id;
}

Student::Student(std::string id, std::string current_class,
                 std::string first_name, std::string last_name, bool gender,
                 int day, int month, int year, std::string social_id, std::string password)
    : Student(id, current_class, first_name, last_name, gender, day, month, year, social_id)
{
    this->password = password;
}

void Student::getInformation()
{
    clrscr();
    std::cout << "SELF-INFORMATION\n";
    std::cout << "Student ID: " << id << '\n';
    std::cout << "Class: " << current_class << '\n';
    std::cout << "Name: " << last_name + ' ' + first_name << '\n';
    std::cout << "Gender: " << (gender ? "Female" : "Male") << '\n';
    std::cout << "Date of birth: " << addZero(day) << '/' << addZero(month) << '/' << year << '\n';
    std::cout << "Social ID: " << social_id << '\n';
    pressKeyToContinue();
}

Student::~Student()
{
    deleteList(enrolled);
}

Student *findStudent(Student *student, std::string username)
{
    while (student && student->username != username)
    {
        student = student->next;
    }
    return student;
}

Student *addNewStudent(Student *&student, Student *new_student)
{
    if (!student)
    {
        student = new_student;
    }
    else
    {
        Student *temp = student;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = new_student;
    }
    return student;
}

void initStudentList(Student *&student)
{
    std::ifstream student_file;
    student_file.open(CLASS_TXT);

    if (student_file.is_open())
    {
        std::string class_id;

        Student *tail = student;

        while (true)
        {
            student_file >> class_id;

            if (student_file.fail())
                break;

            std::ifstream class_file(STUDENT_DIR + class_id + ".txt");

            std::string line;

            if (class_file.is_open())
            {
                std::string line;

                while (true)
                {
                    getline(class_file, line);

                    if (class_file.fail())
                        break;

                    std::string words[9];
                    int index = 0;

                    for (size_t i = 0; i < line.size(); i++)
                    {
                        if (line[i] == ',')
                        {
                            index++;
                        }
                        else
                        {
                            words[index] += line[i];
                        }
                    }

                    Student *new_student = new Student(words[0], class_id, words[1],
                                                       words[2], std::stoi(words[3]), std::stoi(words[4]),
                                                       std::stoi(words[5]), std::stoi(words[6]), words[7], words[8]);

                    if (!student)
                    {
                        student = tail = new_student;
                    }
                    else
                    {
                        tail->next = new_student;
                        tail = tail->next;
                    }
                }
            }
        }
    }
}

void cacheStudentList(Student *student)
{
    rmdir(STUDENT_DIR);

    if (student)
    {
        mkdir(STUDENT_DIR);

        std::ofstream student_file;
        student_file.open(CLASS_TXT);

        if (student_file.is_open())
        {
            std::string class_id;

            while (student)
            {
                if (class_id.empty() || student->current_class != class_id)
                {
                    class_id = student->current_class;
                    student_file << class_id << '\n';

                    std::ofstream class_file(STUDENT_DIR + class_id + ".txt");

                    if (class_file.is_open())
                    {
                        while (student && student->current_class == class_id)
                        {
                            class_file << student->id << ',';
                            class_file << student->first_name << ',';
                            class_file << student->last_name << ',';
                            class_file << student->gender << ',';
                            class_file << student->day << ',';
                            class_file << student->month << ',';
                            class_file << student->year << ',';
                            class_file << student->social_id << ',';
                            class_file << student->password << '\n';

                            student = student->next;
                        }
                    }
                }
            }
        }
    }
}

void studentLogin(Student *&student, SchoolYear *&school_year, Date date)
{
    clrscr();

    std::string username, password;
    inputAccount(username, password);

    Student *find_student = findStudent(student, username);

    if (find_student && find_student->password == password)
        studentGUI(find_student, school_year, date);
    else
    {
        std::cout << "Invalid username or password!\n";
        pressKeyToContinue();
    }
}

void studentGUI(Student *&student, SchoolYear *&school_year, Date date)
{
    int choice = -1;

    SchoolYear *latest_year = chooseSchoolYear(school_year);

    while (choice != 0)
    {
        clrscr();
        std::cout << "Welcome " << student->last_name << ' ' << student->first_name << '\n';

        std::cout << "0. Log Out\n";
        std::cout << "1. Self-information\n";
        std::cout << "2. Change password\n";
        std::cout << "3. Enroll course\n";
        std::cout << "4. View Enrolled Course\n";
        std::cout << "5. Remove Enrolled Course\n";
        std::cout << "6. View scoreboard\n";
        std::cout << "Enter your choice : ";

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
            std::cout << "Logged out!\n";
            pressKeyToContinue();
            break;
        case 1:
            student->getInformation();
            break;
        case 2:
            student->changePassword();
            break;
        case 3:
            enrollCourse(student, latest_year, date);
            break;
        case 4:
            listOfEnrolledCourses(student, latest_year);
            break;
        case 5:
            removeEnrolledCourse(student, latest_year, date);
            break;
        case 6:
            viewScoreboard(student, school_year);
            break;
        default:
            std::cout << "Invalid choice!\n";
            pressKeyToContinue();
            break;
        }
    }
}

void enrollCourse(Student *student, SchoolYear *school_year, Date date)
{
    const int MAX_ENROLL = 5;

    clrscr();
    std::cout << "Input the semester (1 - 3): ";
    int semester;
    std::cin >> semester;
    std::cin.ignore(1000, '\n');

    if (semester < 1 || semester > 3)
    {
        std::cout << "Invalid semester!\n";
        pressKeyToContinue();
        return;
    }

    semester--;

    if (!school_year || !school_year->semester[semester].course_list)
    {
        std::cout << "No course in this semester!\n";
        pressKeyToContinue();
        return;
    }

    Date begin_date = school_year->semester[semester].reg_begin_date;
    Date end_date = school_year->semester[semester].reg_end_date;

    if (datecmp(date, begin_date) < 0 || datecmp(date, end_date) > 0)
    {
        std::cout << "Registration period is not open!\n";
        pressKeyToContinue();
        return;
    }

    CourseEnrolled *dummy_course_enrolled = new CourseEnrolled;
    dummy_course_enrolled->next = student->enrolled;
    student->enrolled = dummy_course_enrolled;

    int count = 0;
    while (student->enrolled)
    {
        if (student->enrolled->year_enrolled == school_year->begin_year && student->enrolled->semester_index == semester)
            count++;
        student->enrolled = student->enrolled->next;
    }

    student->enrolled = dummy_course_enrolled;
    CourseEnrolled *enrolled_head = student->enrolled;

    while (true)
    {
        clrscr();

        if (count >= MAX_ENROLL)
        {
            std::cout << "You have reached the maximum number of courses!\n";
            pressKeyToContinue();
            break;
        }

        if (!viewCourses(school_year, semester))
        {
            std::cout << "No course available!\n";
            pressKeyToContinue();
            break;
        }

        std::cout << "\nInput the ID of the course you want to enroll (input \"break\" to exit): ";
        std::string course_id;
        getline(std::cin, course_id);

        if (course_id == "break")
            break;

        Course *course_head = chooseCourse(school_year, semester, course_id, school_year->begin_year);

        if (!course_head)
        {
            std::cout << "Course not found!\n";
            pressKeyToContinue();
            continue;
        }

        if (countMarks(course_head->mark) == course_head->student_max)
        {
            std::cout << "Course is full!\n";
            pressKeyToContinue();
            continue;
        }

        Mark *dummy_mark = new Mark;
        dummy_mark->next = course_head->mark;
        course_head->mark = dummy_mark;

        Mark *mark_head = course_head->mark;
        while (mark_head->next && mark_head->next->id != student->id)
            mark_head = mark_head->next;

        if (mark_head->next)
        {
            std::cout << "You have already enrolled this course!\n";
            pressKeyToContinue();
        }
        else
        {
            enrolled_head = student->enrolled;

            while (enrolled_head->next)
            {
                if (enrolled_head->next->year_enrolled == school_year->begin_year && enrolled_head->next->semester_index == semester)
                {
                    Course *find_course = chooseCourse(school_year, semester, enrolled_head->next->course_id, school_year->begin_year);

                    if (find_course && checkTimeOverlap(find_course->class_session, course_head->class_session))
                        break;

                    enrolled_head = enrolled_head->next;
                }
            }

            if (enrolled_head->next)
            {
                std::cout << "Time session to overlapped!\n";
                pressKeyToContinue();
            }
            else
            {
                enrolled_head->next = new CourseEnrolled;
                enrolled_head->next->course_id = course_id;
                enrolled_head->next->semester_index = semester;
                enrolled_head->next->year_enrolled = school_year->begin_year;
                mark_head->next = new Mark;
                mark_head->next->id = student->id;
            }
        }
        course_head->mark = dummy_mark->next;
        delete dummy_mark;
    }

    student->enrolled = dummy_course_enrolled->next;
    delete dummy_course_enrolled;
}

void listOfEnrolledCourses(Student *student, SchoolYear *school_year)
{
    clrscr();

    int cur_semester;

    std::cout << "Input the semester (1 - 3): ";
    std::cin >> cur_semester;
    std::cin.ignore(1000, '\n');

    if (cur_semester < 1 || cur_semester > 3)

    {
        std::cout << "Invalid semester!\n";
        pressKeyToContinue();
        return;
    }

    cur_semester--;

    if (!school_year || !school_year->semester[cur_semester].course_list)
    {
        std::cout << "No course in this semester!\n";
        pressKeyToContinue();
        return;
    }

    int cur_year = school_year->begin_year;

    std::cout << "Courses enrolled in semester " << cur_semester + 1 << ", schoolyear " << cur_year << "-" << cur_year + 1 << '\n';

    CourseEnrolled *cur_course = student->enrolled;
    int count = 0;
    while (cur_course)
    {
        if (cur_course->year_enrolled == cur_year && cur_course->semester_index == cur_semester)
        {
            Course *course_head = chooseCourse(school_year, cur_course->semester_index, cur_course->course_id);
            if (course_head)
            {
                std::cout << "Course #" << ++count << '\n';
                course_head->printCourse();
            }
        }
        cur_course = cur_course->next;
    }
    if (count == 0)
    {
        std::cout << "You had not enroll any class yet!\n";
    }
    pressKeyToContinue();
}

void removeEnrolledCourse(Student *student, SchoolYear *school_year, Date date)
{
    clrscr();

    std::cout << "Input the semester (1 - 3): ";
    int semester;
    std::cin >> semester;
    std::cin.ignore(1000, '\n');

    if (semester < 1 || semester > 3)
    {
        std::cout << "Invalid semester!\n";
        pressKeyToContinue();
        return;
    }

    semester--;

    if (!school_year || !school_year->semester[semester].course_list)
    {
        std::cout << "No course in this semester!\n";
        pressKeyToContinue();
        return;
    }

    Date begin_date = school_year->semester[semester].reg_begin_date;
    Date end_date = school_year->semester[semester].reg_end_date;

    if (datecmp(date, begin_date) < 0 || datecmp(date, end_date) > 0)
    {
        std::cout << "Registration period is not open!\n";
        pressKeyToContinue();
        return;
    }

    CourseEnrolled *enrolled_head = student->enrolled;
    int count = 0;
    while (enrolled_head)
    {
        if (enrolled_head->semester_index == semester && enrolled_head->year_enrolled == school_year->begin_year)
            count++;
        enrolled_head = enrolled_head->next;
    }

    while (true)
    {
        if (count == 0)
        {
            std::cout << "No course to remove'\n";
            pressKeyToContinue();
            return;
        }

        if (!viewCourses(school_year, semester))
        {
            std::cout << "No course available!\n";
            pressKeyToContinue();
            break;
        }

        std::cout << "\nInput the ID of the course you want to remove (input \"break\" to exit): ";
        std::string course_id;
        getline(std::cin, course_id);

        if (course_id == "break")
            break;

        Course *course_head = chooseCourse(school_year, semester, course_id);

        if (!course_head)
        {
            std::cout << "Course not found!\n";
            pressKeyToContinue();
            continue;
        }

        CourseEnrolled *dummy_course_enrolled = new CourseEnrolled;
        dummy_course_enrolled->next = student->enrolled;
        student->enrolled = dummy_course_enrolled;
        Mark *dummy_mark = new Mark;
        dummy_mark->next = course_head->mark;
        course_head->mark = dummy_mark;

        while (student->enrolled->next && student->enrolled->next->course_id != course_id)
            student->enrolled = student->enrolled->next;

        while (course_head->mark->next && course_head->mark->next->id != student->id)
            course_head->mark = course_head->mark->next;

        if (student->enrolled->next && course_head->mark->next)
        {
            CourseEnrolled *temp_course = student->enrolled->next;
            student->enrolled->next = temp_course->next;
            Mark *temp_mark = course_head->mark->next;
            course_head->mark->next = temp_mark->next;
            delete temp_course;
            delete temp_mark;
            --count;
        }
        else
        {
            std::cout << "Course not found!\n";
            pressKeyToContinue();
        }

        student->enrolled = dummy_course_enrolled->next;
        course_head->mark = dummy_mark->next;
        delete dummy_course_enrolled;
        delete dummy_mark;
    }
}

void viewScoreboard(Student *student, SchoolYear *school_year)
{
    clrscr();

    SchoolYear *temp = school_year;
    school_year = chooseSchoolYear(school_year);

    std::cout << "Input the semester (1 - 3): ";
    int semester;
    std::cin >> semester;
    std::cin.ignore(1000, '\n');

    if (semester < 1 || semester > 3)
    {
        std::cout << "Invalid semester!\n";
        pressKeyToContinue();
        return;
    }

    semester--;

    if (!school_year || !school_year->semester[semester].course_list)
    {
        std::cout << "No course in this semester!\n";
        pressKeyToContinue();
        return;
    }

    CourseEnrolled *enrolled_head = student->enrolled;
    Course *course_head;

    int count = 1;

    std::cout << "No\t";
    std::cout << std::setw(20) << std::left << "Course ID";
    std::cout << std::setw(10) << std::left << "Total";
    std::cout << std::setw(10) << std::left << "Midterm";
    std::cout << std::setw(10) << std::left << "Final";
    std::cout << std::setw(10) << std::left << "Other";
    std::cout << '\n';

    while (enrolled_head)
    {
        std::string course_id = enrolled_head->course_id;
        int semester_index = enrolled_head->semester_index;

        course_head = school_year->semester[semester].course_list;

        while (course_head && course_head->id != course_id)
            course_head = course_head->next;

        if (course_head)
        {
            Mark *mark_head = course_head->mark;
            while (mark_head && mark_head->id != student->id)
                mark_head = mark_head->next;

            if (mark_head && mark_head->final != -1.0)
            {
                std::cout << count++ << '\t';
                std::cout << std::setw(20) << course_id;
                std::cout << std::setw(10) << mark_head->total;
                std::cout << std::setw(10) << mark_head->midterm;
                std::cout << std::setw(10) << mark_head->final;
                std::cout << std::setw(10) << mark_head->other;
                std::cout << '\n';
            }
        }

        enrolled_head = enrolled_head->next;
    }

    std::cout << '\n';
    std::cout << "\t\t\t\tGPA: " << std::setprecision(2) << calculateSemesterGPA(school_year, student, semester);
    std::cout << "\t GPA Total: " << std::setprecision(2) << calculateTotalGPA(temp, student) << '\n';

    pressKeyToContinue();
}
