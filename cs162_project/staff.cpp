#include "staff.h"

#include "system.h"
#include "student.h"
#include "school.h"
#include <iostream>
#include <fstream>
#include <iomanip>

Staff::Staff() : Staff("", "", "") {}

Staff::Staff(std::string username, std::string password, std::string name)
{
	this->username = username;
	this->password = password;
	this->name = name;
}

void Staff::getInformation()
{
	clrscr();
	std::cout << "SELF-INFORMATION\n";
	std::cout << "Full name: " << this->name << '\n';
	std::cout << "Username: " << this->username << '\n';
	pressKeyToContinue();
}

Staff *findStaff(Staff *staff, std::string username)
{
	while (staff && staff->username != username)
		staff = staff->next;
	return staff;
}

void initStaffList(Staff *&staff)
{
	std::ifstream staff_file;
	staff_file.open(STAFF_TXT);
	std::string username, password, name;
	if (staff_file)
	{
		while (!staff_file.eof())
		{
			getline(staff_file, username);
			getline(staff_file, password);
			getline(staff_file, name);
			if (!staff_file.fail())
			{
				Staff *temp = new Staff(username, password, name);
				temp->next = staff;
				staff = temp;
			}
		}
		reverseList(staff);
	}
}

void cacheStaffList(Staff *staff)
{
	std::ofstream staff_file;
	staff_file.open(STAFF_TXT);

	if (staff_file)
	{
		while (staff)
		{
			staff_file << staff->username << '\n';
			staff_file << staff->password << '\n';
			staff_file << staff->name << '\n';
			staff = staff->next;
		}
	}
}

void staffLogin(Staff *staff, Student *&student, SchoolYear *&school_year)
{
	clrscr();

	std::string username, password;
	inputAccount(username, password);

	Staff *find_staff = findStaff(staff, username);

	if (find_staff && find_staff->password == password)
		staffGUI(find_staff, student, school_year);
	else
	{
		std::cout << "Invalid username or password!\n";
		pressKeyToContinue();
	}
}

void staffGUI(Staff *&staff, Student *&student, SchoolYear *&school_year)
{
	int choice = -1;

	while (choice != 0)
	{
		clrscr();

		std::cout << "WELCOME " << staff->name << '\n';

		std::cout << "0. Logout\n";
		std::cout << "1. Self-information\n";
		std::cout << "2. Change password\n";
		std::cout << "3. School Year and Courses manager\n";
		std::cout << "4. Student manager\n";
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
			std::cout << "Logged out!\n";
			pressKeyToContinue();
			break;
		case 1:
			staff->getInformation();
			break;
		case 2:
			staff->changePassword();
			break;
		case 3:
			courseManagerGUI(student, school_year);
			break;
		case 4:
			studentManagerGUI(student, school_year);
			break;
		default:
			std::cout << "Invalid choice!\n";
			pressKeyToContinue();
			break;
		}
	}
}

void courseManagerGUI(Student *student, SchoolYear *&school_year)
{
	int choice = -1;

	SchoolYear *cur_school_year = chooseSchoolYear(school_year);

	while (choice != 0)
	{
		clrscr();

		std::cout << "Course manager section: \n";

		std::cout << "0. Back to main menu\n";
		std::cout << "1. Create a new school year\n";
		std::cout << "2. Create new semester\n";
		std::cout << "3. Add a course to a semester\n";
		std::cout << "4. Update course information\n";
		std::cout << "5. Delete course\n";
		std::cout << "6. View courses list\n";
		std::cout << "7. View students in a course\n";
		std::cout << "8. Export list of student in a course to CSV\n";
		std::cout << "9. Export score template\n";
		std::cout << "10. Import the scoreboard\n";
		std::cout << "11. Update the score of one student\n";
		std::cout << "12. View scoreboard in courses\n";

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
			return;
		case 1:
			createSchoolYear(school_year);
			cur_school_year = chooseSchoolYear(school_year);
			break;
		case 2:
			createSemester(cur_school_year);
			break;
		case 3:
			addCourse(cur_school_year);
			break;
		case 4:
			updateCourseInfo(cur_school_year);
			break;
		case 5:
			deleteCourse(cur_school_year, student);
			break;
		case 6:
			viewCoursesList(cur_school_year);
			break;
		case 7:
			viewStudentInACourse(cur_school_year, student);
			break;
		case 8:
			exportStudentListToCSV(student, cur_school_year);
			break;
		case 9:
			exportScoreTemplate(student, cur_school_year);
			break;
		case 10:
			importScoresFromCSV(cur_school_year);
			break;
		case 11:
			updateStudentResult(student, school_year);
			break;
		case 12:
			viewScoreBoardOfCourse(student, school_year);
			break;
		default:
			std::cout << "Invalid choice!\n";
			pressKeyToContinue();
			break;
		}
	}
}

void studentManagerGUI(Student *&student, SchoolYear *school_year)
{
	int choice = -1;

	SchoolYear *cur_school_year = chooseSchoolYear(school_year);

	while (choice != 0)
	{
		clrscr();

		std::cout << "Course manager section: \n";

		std::cout << "0. Back to main menu\n";
		std::cout << "1. Create new class\n";
		std::cout << "2. View classes list\n";
		std::cout << "3. View students in a class\n";
		std::cout << "4. View scoreboard of a class\n";

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
			return;
		case 1:
			createClass(student);
			break;
		case 2:
			viewClassesList(student, 1);
			break;
		case 3:
			viewStudentsList(student);
			break;
		case 4:
			viewScoreBoardOfClass(student, school_year);
			break;
		default:
			std::cout << "Invalid choice!\n";
			pressKeyToContinue();
			break;
		}
	}
}

void createSchoolYear(SchoolYear *&school_year)
{
	clrscr();
	SchoolYear *dummy = new SchoolYear;
	dummy->next = school_year;
	school_year = dummy;

	int begin_year, end_year;
	std::cout << "Input the school year (ex: 2021-2022): ";
	char sep;
	std::cin >> begin_year >> sep >> end_year;
	std::cin.ignore(1000, '\n');

	if (chooseSchoolYear(school_year, begin_year))
	{
		std::cout << "School year already exists!\n";
		pressKeyToContinue();
	}
	else
	{
		while (school_year->next)
			school_year = school_year->next;
		SchoolYear *temp = new SchoolYear;
		temp->begin_year = begin_year;
		temp->end_year = end_year;
		school_year->next = temp;
		std::cout << "School year created!\n";
		pressKeyToContinue();
	}
	school_year = dummy->next;
	delete dummy;
}

void createClass(Student *&student)
{
	clrscr();
	std::string class_code;
	std::cout << "Input the class: ";
	std::cin >> class_code;

	int choice;
	std::cout << "1. Add a new student manually\n";
	std::cout << "2. Add a new student from csv file\n";
	std::cout << "Enter your choice: ";
	std::cin >> choice;
	std::cin.ignore(1000, '\n');

	if (choice == 1)
	{
		if (importClassManually(student, class_code))
			std::cout << "Class created / Students added!\n";
		pressKeyToContinue();
	}
	else if (choice == 2)
	{
		if (importClassFromCSV(student, class_code))
		{
			std::cout << "Class created!\n";
			pressKeyToContinue();
		}
	}
	else
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
	}
}

bool importClassFromCSV(Student *&student, std::string class_name)
{
	Student *traverse = student;
	while (traverse)
	{
		if (traverse->current_class == class_name)
		{
			std::cout << "Class already exists! Can only be added manually...\n";
			pressKeyToContinue();
			return 0;
		}
		traverse = traverse->next;
	}

	std::ifstream student_csv;
	student_csv.open(class_name + ".csv");

	if (student_csv.is_open())
	{
		Student *class_head = nullptr;
		std::string cache;

		getline(student_csv, cache);

		while (true)
		{
			getline(student_csv, cache);
			if (student_csv.fail())
				break;

			std::string words[9];
			int index = 0;

			for (size_t i = 0; i < cache.size(); i++)
			{
				if (cache[i] == ',' || cache[i] == '/')
					index++;
				else
					words[index].push_back(cache[i]);
			}

			std::string id = words[1], first_name = words[2], last_name = words[3];
			int day = std::stoi(words[4]), month = std::stoi(words[5]), year = std::stoi(words[6]);
			bool gender = std::stoi(words[7]);
			std::string social_id = words[8];

			Student *temp_student = new Student(id, class_name,
												first_name, last_name, gender,
												day, month, year, social_id);
			temp_student->next = class_head;
			class_head = temp_student;
		}
		reverseList(class_head);

		if (!student)
			student = class_head;
		else
		{
			Student *temp = student;
			while (temp->next)
				temp = temp->next;
			temp->next = class_head;
		}
	}
	return 1;
}

bool importClassManually(Student *&student, std::string class_name)
{
	Student *dummy = new Student("", "", "", "", 0, 0, 0, 0, "");
	dummy->next = student;
	student = dummy;

	Student *tail = student;

	while (tail->next && tail->next->current_class != class_name)
		tail = tail->next;

	bool cont = 1;

	if (tail->next && tail->next->current_class == class_name)
	{
		std::cout << "Class already exists! Press Y to add more... ";

		char ch = std::cin.get();

		if (ch != 'Y' && ch != 'y')
			cont = 0;
	}

	if (cont)
	{
		while (tail->next && tail->next->current_class == class_name)
			tail = tail->next;

		std::string id, current_class;
		std::string first_name, last_name;
		bool gender; // 0 for male, 1 for female
		std::string social_id;
		Date dob;

		while (true)
		{
			clrscr();

			char check;
			std::cout << "Input a character, 0 to stop: ";
			std::cin >> check;
			std::cin.ignore(1000, '\n');

			if (check == '0')
				break;

			std::cout << "Enter ID: ";
			getline(std::cin, id);

			while (findStudent(student, id))
			{
				std::cout << "Student is available!\n";
				std::cout << "Enter ID: ";
				getline(std::cin, id);
			}

			std::cout << "Enter first name: ";
			getline(std::cin, first_name);
			std::cout << "Enter last name: ";
			getline(std::cin, last_name);
			std::cout << "Enter gender, 0 for male, 1 for female: ";
			std::cin >> gender;
			std::cin.ignore(1000, '\n');
			dob.consoleInputDate("Enter DOB");
			std::cin.ignore(1000, '\n');
			std::cout << "Enter social ID: ";
			std::cin >> social_id;

			Student *temp = new Student(id, class_name, first_name, last_name,
										gender, dob.day, dob.month, dob.year, social_id);
			if (!student)
			{
				student = tail = temp;
			}
			else
			{
				temp->next = tail->next;
				tail->next = temp;
			}
		}
	}
	else
		return 0;

	student = dummy->next;
	delete dummy;
	return 1;
}

void createSemester(SchoolYear *&school_year)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return;

	int choice;
	std::cout << "There are 3 semestes in a school year\n";
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> choice;
	std::cin.ignore(1000, '\n');

	if (choice < 1 || choice > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	choice--;

	for (int i = 0; i < choice; i++)
	{
		if (school_year->semester[i].begin_date.day == DAY_INIT)
		{
			std::cout << "Semester " << i + 1 << " must be created first!\n";
			pressKeyToContinue();
			return;
		}
	}

	if (school_year->semester[choice].begin_date.day != DAY_INIT)
	{
		std::cout << "Semester " << choice + 1 << " is available!\n";
		pressKeyToContinue();
		return;
	}

	std::cout << "Input the start and end date of semester " << choice + 1 << "\n";
	school_year->semester[choice].begin_date.consoleInputDate("Start date");
	std::cin.ignore(1000, '\n');
	school_year->semester[choice].end_date.consoleInputDate("End date");
	std::cin.ignore(1000, '\n');
	std::cout << "Input the start and end date of semester " << choice + 1 << " registration day\n";
	;
	school_year->semester[choice].reg_begin_date.consoleInputDate("Start date");
	std::cin.ignore(1000, '\n');
	school_year->semester[choice].reg_end_date.consoleInputDate("End date");
	std::cin.ignore(1000, '\n');

	std::cout << "Semester added successfully!\n";
	pressKeyToContinue();
}

void addCourse(SchoolYear *school_year)
{
	clrscr();

	std::cout << "Input the semester (1 - 3): ";
	int choice;
	std::cin >> choice;

	if (choice < 1 || choice > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	choice--;

	if (school_year->semester[choice].begin_date.day == DAY_INIT)
	{
		std::cout << "Semester " << choice + 1 << " is not available!\n";
		pressKeyToContinue();
		return;
	}

	Course *temp = school_year->semester[choice].course_list;

	if (!temp)
	{
		std::cout << "There is no course in semester " << choice + 1 << "!\n";
		pressKeyToContinue();
		return;
	}

	std::cout << "Available courses in semester " << choice + 1 << ":\n";

	int count = 1;

	while (temp)
	{
		std::cout << count++ << ". " << temp->id << " " << temp->name << "\n";
		temp = temp->next;
	}

	std::cin.ignore(1000, '\n');

	addCoursesNode(school_year->semester[choice].course_list);
}

void viewCoursesList(SchoolYear *school_year)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return;

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	semester--;

	if (!school_year->semester[semester].course_list)
	{
		std::cout << "There is no course in semester " << semester + 1 << "!\n";
		pressKeyToContinue();
		return;
	}

	std::cout << "Course list of semester " << semester + 1 << ":\n";

	int count = 1;

	Course *temp = school_year->semester[semester].course_list;
	while (temp)
	{
		std::cout << "Course #" << count++ << '\n';
		temp->printCourse();
		std::cout << "\n\n";
		temp = temp->next;
	}

	pressKeyToContinue();
}

void updateCourseInfo(SchoolYear *school_year)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return;

	std::string course_id;

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	do
	{
		clrscr();

		if (!viewCourses(school_year, semester - 1))
			break;

		std::cout << "Enter id of the course to update, 0 to return: ";
		std::cin >> course_id;
		std::cin.ignore(1000, '\n');

		if (course_id == "0")
			break;

		Course *temp = chooseCourse(school_year, semester - 1, course_id, school_year->begin_year);

		if (!temp)
		{
			std::cout << "Course does not exist.\n";
			pressKeyToContinue();
			continue;
		}

		int choice = -1, day, time;

		while (choice != 0)
		{
			clrscr();

			std::cout << "Update course " << temp->id << " information: \n";
			std::cout << "1. ID\n";
			std::cout << "2. Name of course\n";
			std::cout << "3. Teacher's name\n";
			std::cout << "4. Number of credits\n";
			std::cout << "5. Max students \n";
			std::cout << "6. Day of session 1\n";
			std::cout << "7. Time of session 1\n";
			std::cout << "8. Day of session 2\n";
			std::cout << "9. Time of session 2\n";
			std::cout << "0. Return\n";
			std::cout << "Please choose each option above to update: ";

			std::cin >> choice;
			std::cin.ignore(1000, '\n');

			if (choice >= 6 && choice <= 9)
				temp->class_session->printSession();

			switch (choice)
			{
			case 1:
				std::cout << "New ID: ";
				getline(std::cin, temp->id);
				break;
			case 2:
				std::cout << "New name: ";
				getline(std::cin, temp->name);
				break;
			case 3:
				std::cout << "New teacher's name: ";
				getline(std::cin, temp->teacher_name);
				break;
			case 4:
				std::cout << "New number of credits: ";
				std::cin >> temp->credits;
				std::cin.ignore(1000, '\n');
				break;
			case 5:
				std::cout << "New max students: ";
				std::cin >> temp->student_max;
				std::cin.ignore(1000, '\n');
				break;
			case 6:
				std::cout << "New day of session 1: ";
				std::cin >> day;
				temp->class_session[0].setDay(day);
				std::cin.ignore(1000, '\n');
				break;
			case 7:
				std::cout << "New time of session 1: ";
				std::cin >> time;
				temp->class_session[0].setTime(time);
				std::cin.ignore(1000, '\n');
				break;
			case 8:
				std::cout << "New day of session 2: ";
				std::cin >> day;
				temp->class_session[1].setDay(day);
				std::cin.ignore(1000, '\n');
				break;
			case 9:
				std::cout << "New time of session 2: ";
				std::cin >> time;
				temp->class_session[1].setTime(time);
				std::cin.ignore(1000, '\n');
				break;
			case 0:
				clrscr();
				break;
			default:
				std::cout << "Invalid choice!\n";
				pressKeyToContinue();
				break;
			}

			if (choice >= 1 && choice <= 9)
			{
				std::cout << "Updated!\n";
				pressKeyToContinue();
			}
		}
	} while (course_id != "0");
}

void deleteCourse(SchoolYear *&school_year, Student *student)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return;

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	semester--;

	std::string course_id;

	do
	{
		clrscr();

		if (!viewCourses(school_year, semester))
			break;

		std::cout << "Enter id of the course to delete, 0 to return: ";
		std::cin >> course_id;
		std::cin.ignore(1000, '\n');

		if (course_id == "0")
			break;

		bool check_delete = 0;

		Course *dummy = new Course;
		Course *cur = school_year->semester[semester].course_list;

		dummy->next = cur;
		cur = dummy;

		while (cur->next && cur->next->id != course_id)
			cur = cur->next;

		if (cur->next)
		{
			Course *temp = cur->next;
			cur->next = temp->next;

			Mark *temp_mark = temp->mark;

			while (temp_mark)
			{
				Student *find_student = findStudent(student, temp_mark->id);

				if (find_student)
				{
					CourseEnrolled *dummy = new CourseEnrolled;
					dummy->next = find_student->enrolled;
					find_student->enrolled = dummy;

					while (find_student->enrolled->next && find_student->enrolled->next->course_id != temp->id)
						find_student->enrolled = find_student->enrolled->next;

					if (find_student->enrolled->next)
					{
						CourseEnrolled *temp_enrolled = find_student->enrolled->next;
						find_student->enrolled->next = temp_enrolled->next;
						delete temp_enrolled;
					}

					find_student->enrolled = dummy->next;
					delete dummy;
				}

				temp_mark = temp_mark->next;
			}

			deleteList(temp->mark);
			delete temp;
			check_delete = 1;
		}

		school_year->semester[semester].course_list = dummy->next;
		delete dummy;

		if (!check_delete)
			std::cout << "Course does not exist. \n";
		else
			std::cout << "Course deleted. \n";

		pressKeyToContinue();

	} while (course_id != "0");
}

void viewClassesList(Student *student, bool pressKey)
{
	clrscr();
	int count = 0;
	std::string class_code;
	Student *cur_student = student;
	std::cout << "LIST OF CLASSES:\n";
	while (cur_student)
	{
		if (cur_student->current_class != class_code)
		{
			count++;
			class_code = cur_student->current_class;
			std::cout << count << ". " << class_code << '\n';
		}
		cur_student = cur_student->next;
	}
	if (pressKey)
		pressKeyToContinue();
	else
		std::cout << '\n';
}

void viewStudentsList(Student *student)
{
	std::string class_name = "1";
	while (class_name != "0")
	{
		clrscr();

		viewClassesList(student);

		std::cout << "Enter the name of class (enter 0 to return): ";
		getline(std::cin, class_name);

		if (class_name == "0")
			return;

		int count = 0;
		Student *cur_student = student;

		clrscr();

		std::cout << "LIST OF STUDENTS IN CLASS " << class_name << ":\n";

		std::cout << std::setw(5) << std::left << "No";
		std::cout << std::setw(10) << std::left << "ID";
		std::cout << std::setw(30) << std::left << "Name";
		std::cout << std::setw(15) << std::left << "DOB";
		std::cout << std::setw(20) << std::left << "Social ID" << '\n';

		while (cur_student && cur_student->current_class != class_name)
			cur_student = cur_student->next;

		while (cur_student && cur_student->current_class == class_name)
		{
			std::cout << std::setw(5) << std::left << ++count;
			std::cout << std::setw(10) << std::left << cur_student->id;
			std::cout << std::setw(30) << std::left << cur_student->last_name + ' ' + cur_student->first_name;
			std::cout << std::setw(15) << std::left << addZero(cur_student->day) + '/' + addZero(cur_student->month) + '/' + std::to_string(cur_student->year);
			std::cout << std::setw(20) << std::left << cur_student->social_id << '\n';
			cur_student = cur_student->next;
		}

		if (count == 0)
		{
			clrscr();
			std::cout << "Invalid class!\n";
		}

		pressKeyToContinue();
	}
}

void viewStudentInACourse(SchoolYear *school_year, Student *student)
{
	clrscr();

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	semester--;
	std::string course_id = "1";
	while (course_id != "0")
	{
		clrscr();

		if (!viewCourses(school_year, semester))
			break;

		std::cout << '\n';
		std::cout << "Enter the id of course (enter 0 to return): ";
		getline(std::cin, course_id);

		if (course_id == "0")
			return;

		Course *temp = chooseCourse(school_year, semester, course_id, school_year->begin_year);

		if (!temp)
		{
			std::cout << "Course does not exist.\n";
			pressKeyToContinue();
			continue;
		}

		clrscr();
		Mark *temp_mark = temp->mark;

		if (!temp_mark)
		{
			clrscr();
			std::cout << "No student enrolled this course!\n";
		}
		else
		{
			int count = 0;

			std::cout << "LIST OF STUDENTS IN COURSE " << course_id << ":\n";

			std::cout << std::setw(5) << std::left << "No";
			std::cout << std::setw(10) << std::left << "ID";
			std::cout << std::setw(30) << std::left << "Name";
			std::cout << std::setw(15) << std::left << "DOB" << '\n';

			while (temp_mark)
			{
				Student *find_student = findStudent(student, temp_mark->id);

				std::cout << std::setw(5) << std::left << ++count;
				std::cout << std::setw(10) << std::left << find_student->id;
				std::cout << std::setw(30) << std::left << find_student->last_name + ' ' + find_student->first_name;
				std::cout << std::setw(15) << std::left << addZero(find_student->day) + '/' + addZero(find_student->month) + '/' + std::to_string(find_student->year);
				std::cout << '\n';

				temp_mark = temp_mark->next;
			}
		}

		pressKeyToContinue();
	}
}

void exportStudentListToCSV(Student *student, SchoolYear *school_year)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return;

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	if (!viewCourses(school_year, semester - 1))
		return;

	std::string course_id;
	std::cout << "Please input the course id to export: ";
	std::cin >> course_id;

	Course *course = chooseCourse(school_year, semester - 1, course_id, school_year->begin_year);

	if (!course)
	{
		std::cout << "Invalid course ID!\n";
		pressKeyToContinue();
		return;
	}

	std::string file_name = course_id + ".csv";
	std::ofstream student_csv(file_name);

	if (student_csv.is_open())
	{
		student_csv << "No,Student ID,Full Name\n";
		int no = 1;
		Mark *list = course->mark;

		while (list)
		{
			std::string id = list->id;

			Student *find_student = findStudent(student, id);

			if (find_student)
			{
				student_csv << no++ << ',' << id << ',' << find_student->last_name << ' ' << find_student->first_name << '\n';
			}

			list = list->next;
		}
	}

	std::cout << "File exported successfully!\n";
	pressKeyToContinue();
}

void exportScoreTemplate(Student *student, SchoolYear *school_year)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return;

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	if (!viewCourses(school_year, semester - 1))
		return;

	std::cout << "Please input the course to export: ";
	std::string course_id;
	std::cin >> course_id;

	Course *course = chooseCourse(school_year, semester - 1, course_id, school_year->begin_year);

	if (!course)
	{
		std::cout << "Invalid course ID!\n";
		pressKeyToContinue();
		return;
	}

	std::string file_name = course_id + "_scoreboard.csv";
	std::ofstream score_csv(file_name);

	if (score_csv.is_open())
	{
		score_csv << "No,Student ID,Total,Final,Midterm,Other\n";
		int no = 1;
		Mark *list = course->mark;

		while (list)
		{
			std::string id = list->id;

			score_csv << no++ << ',' << id << '\n';

			list = list->next;
		}
	}

	std::cout << "File exported successfully!\n";
	pressKeyToContinue();
}

void importScoresFromCSV(SchoolYear *school_year)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return;

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	if (!viewCourses(school_year, semester - 1))
		return;

	std::cout << "Please input the course id to import: ";
	std::string course_id;
	std::cin >> course_id;

	Course *course = chooseCourse(school_year, semester - 1, course_id, school_year->begin_year);

	if (!course)
	{
		std::cout << "Invalid course ID!\n";
		pressKeyToContinue();
		return;
	}

	std::string file_name = course_id + "_scoreboard.csv";
	std::ifstream score_csv(file_name);

	if (!score_csv.is_open())
	{
		std::cout << "File not found!\n";
		pressKeyToContinue();
		return;
	}

	std::string read_line;
	getline(score_csv, read_line);

	while (!score_csv.eof())
	{
		getline(score_csv, read_line);
		if (score_csv.fail())
			break;

		std::string words[6];
		int index = 0;
		for (size_t i = 0; i < read_line.size(); i++)
		{
			if (read_line[i] == ',')
				index++;
			else
				words[index].push_back(read_line[i]);
		}

		Mark *mark_head = course->mark;
		while (mark_head)
		{
			if (mark_head->id == words[1])
			{
				mark_head->total = std::stof(words[2]);
				mark_head->final = std::stof(words[3]);
				mark_head->midterm = std::stof(words[4]);
				mark_head->other = std::stof(words[5]);
				break;
			}
			mark_head = mark_head->next;
		}
	}

	std::cout << "Import completed!\n";
	pressKeyToContinue();
}

void updateStudentResult(Student *student, SchoolYear *school_year)
{
	clrscr();

	std::string student_id;
	std::string course_id;
	Course *course = nullptr;

	clrscr();

	std::cout << "Please enter id of the student to update, 0 to return: ";
	std::cin >> student_id;
	std::cin.ignore(1000, '\n');

	if (student_id == "0")
		return;

	Student *find_student = findStudent(student, student_id);

	if (!find_student)
	{
		std::cout << "Wrong ID! \n";
		pressKeyToContinue();
		return;
	}

	int semester;
	std::cout << "Input the semester (1 - 3): ";
	std::cin >> semester;

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	if (!viewCourses(school_year, semester - 1))
		return;

	semester--;

	std::cout << "Student with ID " << student->id << " selected successfully!\n";

	std::cout << "Please enter id of course you want to change result, 0 to return: ";
	std::cin >> course_id;
	std::cin.ignore(1000, '\n');

	if (course_id == "0")
		return;

	course = chooseCourse(school_year, semester, course_id, school_year->begin_year);

	if (!course)
	{
		std::cout << "Course not found. Please try again!\n";
		pressKeyToContinue();
		return;
	}

	Mark *find_mark = course->mark;

	while (find_mark && find_mark->id != student_id)
		find_mark = find_mark->next;

	if (!find_mark)
	{
		std::cout << "This student does not enroll this course. Please try again\n";
		pressKeyToContinue();
		return;
	}

	int choice = -1;
	std::cout << "Update result in course " << course->id << ":\n";
	std::cout << "1. Midterm\n";
	std::cout << "2. Final\n";
	std::cout << "3. Other\n";
	std::cout << "4. Total\n";
	std::cout << "0. Return\n";
	while (choice != 0)
	{
		std::cout << "Please choose each option above to update: ";

		std::cin >> choice;
		std::cin.ignore(1000, '\n');

		switch (choice)
		{
		case 1:
			std::cout << "New midterm mark: ";
			std::cin >> find_mark->midterm;
			break;
		case 2:
			std::cout << "New final mark: ";
			std::cin >> find_mark->final;
			break;
		case 3:
			std::cout << "New other mark: ";
			std::cin >> find_mark->other;
			break;
		case 4:
			std::cout << "New total mark: ";
			std::cin >> find_mark->total;
			break;
		case 0:
			clrscr();
			break;
		default:
			std::cout << "Invalid choice!\n";
			pressKeyToContinue();
			break;
		}

		if (choice >= 1 && choice <= 4)
		{
			std::cout << "Update completed!\n";
			pressKeyToContinue();
		}
	}
}

void viewScoreBoardOfCourse(Student *student, SchoolYear *school_year)
{
	clrscr();

	int semester;
	std::cout << "Choose semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	if (!viewCourses(school_year, semester - 1))
		return;

	std::cout << "Please input the course to view: ";
	std::string course_id;
	std::cin >> course_id;

	Course *course = chooseCourse(school_year, semester - 1, course_id, school_year->begin_year);
	if (!course)
	{
		std::cout << "Invalid course ID!\n";
		pressKeyToContinue();
		return;
	}

	std::cout << std::setw(20) << std::left << "Student ID";
	std::cout << std::setw(10) << std::left << "Midterm";
	std::cout << std::setw(10) << std::left << "Final";
	std::cout << std::setw(10) << std::left << "Other";
	std::cout << std::setw(10) << std::left << "Total" << '\n';

	Mark *mark_head = course->mark;
	while (mark_head && mark_head->final != -1)
	{
		std::cout << std::setw(20) << std::left << mark_head->id;
		std::cout << std::setw(10) << std::left << mark_head->midterm
				  << std::setw(10) << std::left << mark_head->final;
		std::cout << std::setw(10) << std::left << mark_head->other
				  << std::setw(10) << std::left << mark_head->total << '\n';
		mark_head = mark_head->next;
	}
	pressKeyToContinue();
}

void viewScoreBoardOfClass(Student *student, SchoolYear *school_year)
{
	clrscr();

	int year, year_end;
	char sep;
	std::cout << "Input schoolyear (ex 2021-2022): ";
	std::cin >> year >> sep >> year_end;
	std::cin.ignore(1000, '\n');

	SchoolYear *find_schoolyear = chooseSchoolYear(school_year, year);

	if (!find_schoolyear || school_year->end_year != year_end)
	{
		std::cout << "Invalid year!\n";
		pressKeyToContinue();
		return;
	}

	int semester;
	std::cout << "Choose semester (1 - 3): ";
	std::cin >> semester;
	std::cin.ignore(1000, '\n');

	if (semester < 1 || semester > 3)
	{
		std::cout << "Invalid choice!\n";
		pressKeyToContinue();
		return;
	}

	semester--;

	if (!find_schoolyear->semester[semester].course_list)
	{
		std::cout << "No course in this semester!\n";
		pressKeyToContinue();
		return;
	}

	viewClassesList(student);

	std::string name_class;
	std::cout << "Input class: ";
	getline(std::cin, name_class);

	int count = 0;
	Student *student_traverse = student;

	while (student_traverse)
	{
		if (student_traverse->current_class == name_class)
		{
			count++;

			if (count == 1)
				std::cout << "\nScoreboard of class " << name_class << ":\n";

			std::cout << count << ". ";
			std::cout << "Student ID: " << student_traverse->id << '\t';
			std::cout << "Student name: " << student_traverse->last_name << " " << student_traverse->first_name << '\n';

			CourseEnrolled *enrolled = student_traverse->enrolled;

			while (enrolled)
			{
				Course *find_course = chooseCourse(school_year, enrolled->semester_index, enrolled->course_id, enrolled->year_enrolled);

				std::cout << std::setw(15) << std::left << "Course ID";
				std::cout << std::setw(35) << std::left << "Course's name";
				std::cout << std::setw(10) << std::left << "Final";
				std::cout << std::setw(10) << std::left << "Total" << '\n';

				if (find_course)
				{
					Mark *mark_head = find_course->mark;
					while (mark_head)
					{
						if (mark_head->id == student_traverse->id && mark_head->final != -1)
						{
							std::cout << std::setw(15) << std::left << enrolled->course_id
									  << std::setw(35) << std::left << find_course->name
									  << std::setw(10) << std::left << mark_head->final
									  << std::setw(10) << std::left << mark_head->total << '\n';
							break;
						}
						mark_head = mark_head->next;
					}
				}

				enrolled = enrolled->next;
			}

			std::cout << "\nGPA: " << std::setprecision(2) << calculateSemesterGPA(find_schoolyear, student_traverse, semester) << " ";
			std::cout << "Overall GPA: " << std::setprecision(2) << calculateTotalGPA(school_year, student_traverse) << "\n\n\n";
		}

		student_traverse = student_traverse->next;
	}

	if (count == 0)
	{
		std::cout << "Invalid class!\n";
		pressKeyToContinue();
		return;
	}

	pressKeyToContinue();
}