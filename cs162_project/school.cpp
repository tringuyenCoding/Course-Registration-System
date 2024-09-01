#include "school.h"

#include "system.h"
#include "student.h"
#include <iostream>
#include <fstream>

void Session::printSession()
{
	std::cout << "The option to choose a section are:\n";
	std::cout << "- MON to SAT (input 2 to 7, wrong input will set to MON)\n";
	std::cout << "- Time (S1 is default): ";
	for (int i = 0; i < max_time; i++)
	{
		std::cout << times[i] << "  ";
	}
	std::cout << '\n';
}

void Session::setDay(int day)
{
	if (day >= 2 && day <= 7)
		this->day = day - 2;
	else
		this->day = 0;
}

void Session::setTime(int time)
{
	if (time >= 1 && time <= max_time)
		this->time = time - 1;
	else
		this->time = 0;
}

std::string Session::getDay() { return days[day]; }

std::string Session::getTime() { return times[time]; }

void Session::getDayTimeInInt(int &day, int &time)
{
	day = this->day + 2;
	time = this->time + 1;
}

Session &Session::operator=(Session &other)
{
	day = other.day;
	time = other.time;
	return *this;
}

bool Session::operator==(Session &other)
{
	return (day == other.day && time == other.time);
}

void Course::printCourse()
{
	std::cout << "\tCourse ID: " << id << '\n';
	std::cout << "\tCourse Name: " << name << '\n';
	std::cout << "\tTeacher Name: " << teacher_name << '\n';
	std::cout << "\tSession 1: " << class_session[0].getDay() << " " << class_session[0].getTime() << '\n';
	std::cout << "\tSession 2: " << class_session[1].getDay() << " " << class_session[1].getTime() << '\n';
}

Course::~Course()
{
	deleteList(mark);
}

Semester::~Semester()
{
	deleteList(course_list);
}

void initSchoolYearList(Student *student, SchoolYear *&school_year)
{

	std::ifstream school(SCHOOL_TXT);

	if (school.is_open())
	{
		int count;
		school >> count;
		while (count--)
		{
			int begin_year, end_year, semester_num;
			school >> begin_year >> end_year >> semester_num;

			SchoolYear *temp = new SchoolYear;
			temp->begin_year = begin_year;
			temp->end_year = end_year;

			std::string folder = DATA_DIR + std::to_string(begin_year) + '-' + std::to_string(end_year) + "/";

			for (int i = 1; i <= semester_num; i++)
			{
				int semester_index = i - 1;

				std::string semester_folder = folder + "Sem" + std::to_string(i) + "/";

				std::ifstream semester_file(semester_folder + SEM_TXT);

				if (semester_file.is_open())
				{
					semester_file >> temp->semester[semester_index].begin_date;
					semester_file >> temp->semester[semester_index].end_date;
					semester_file >> temp->semester[semester_index].reg_begin_date;
					semester_file >> temp->semester[semester_index].reg_end_date;
					semester_file.ignore(1000, '\n');

					std::string course_id;

					while (true)
					{
						getline(semester_file, course_id);

						if (semester_file.fail())
							break;

						std::string course_folder = semester_folder + course_id + "/";

						std::string course_info = course_id + "_info.txt";
						std::string course_mark = course_id + "_mark.txt";

						std::ifstream info_file(course_folder + course_info);
						std::ifstream mark_file(course_folder + course_mark);

						if (info_file.is_open() && mark_file.is_open())
						{
							std::string name;
							getline(info_file, name);

							if (info_file.fail())
								break;

							Course *temp_course = new Course;

							temp_course->id = course_id;

							temp_course->name = name;
							getline(info_file, temp_course->teacher_name);

							info_file >> temp_course->credits;
							info_file.ignore(1000, '\n');

							info_file >> temp_course->student_max;
							info_file.ignore(1000, '\n');

							int day, time;
							info_file >> day >> time;
							info_file.ignore(1000, '\n');
							temp_course->class_session[0].setDay(day);
							temp_course->class_session[0].setTime(time);

							info_file >> day >> time;
							info_file.ignore(1000, '\n');
							temp_course->class_session[1].setDay(day);
							temp_course->class_session[1].setTime(time);

							while (true)
							{
								std::string id;
								getline(mark_file, id);

								if (mark_file.fail())
									break;

								Mark *temp_mark = new Mark;
								temp_mark->id = id;

								mark_file >> temp_mark->midterm >> temp_mark->final >> temp_mark->other >> temp_mark->total;
								mark_file.ignore(1000, '\n');

								Student *temp_student = student;

								while (student && student->id != id)
									student = student->next;

								if (student)
								{
									CourseEnrolled *newData = new CourseEnrolled;
									newData->year_enrolled = temp->begin_year;
									newData->course_id = temp_course->id;
									newData->semester_index = semester_index;

									newData->next = student->enrolled;
									student->enrolled = newData;

									temp_mark->next = temp_course->mark;
									temp_course->mark = temp_mark;
								}

								student = temp_student;
							}
							reverseList(temp_course->mark);

							temp_course->next = temp->semester[semester_index].course_list;
							temp->semester[semester_index].course_list = temp_course;
						}

						reverseList(temp->semester[semester_index].course_list);
					}
				}
			}
			temp->next = school_year;
			school_year = temp;
		}
		reverseList(school_year);
	}
}

void cacheSchoolYearList(SchoolYear *school_year)
{
	std::ofstream school(SCHOOL_TXT);

	if (school.is_open())
	{
		int count = countYears(school_year);
		school << count << '\n';
		while (school_year)
		{
			int begin_year = school_year->begin_year, end_year = school_year->end_year, semester_num = countSemesters(school_year);
			school << begin_year << ' ' << end_year << ' ' << semester_num << '\n';

			std::string folder = DATA_DIR + std::to_string(begin_year) + '-' + std::to_string(end_year) + "/";

			rmdir(folder);

			if (semester_num)
				mkdir(folder);

			for (int i = 1; i <= semester_num; i++)
			{
				int semester_index = i - 1;

				std::string semester_folder = folder + "Sem" + std::to_string(i) + "/";
				mkdir(semester_folder);

				std::ofstream semester_file(semester_folder + SEM_TXT);

				if (semester_file.is_open())
				{
					semester_file << school_year->semester[semester_index].begin_date << '\n';
					semester_file << school_year->semester[semester_index].end_date << '\n';
					semester_file << school_year->semester[semester_index].reg_begin_date << '\n';
					semester_file << school_year->semester[semester_index].reg_end_date << '\n';

					Course *course = school_year->semester[semester_index].course_list;

					while (course)
					{
						std::string course_id = course->id;

						semester_file << course_id << '\n';

						std::string course_folder = semester_folder + course_id + "/";
						mkdir(course_folder);

						std::string course_info = course_id + "_info.txt";
						std::string course_mark = course_id + "_mark.txt";

						std::ofstream info_file(course_folder + course_info);
						std::ofstream mark_file(course_folder + course_mark);

						if (info_file.is_open() && mark_file.is_open())
						{
							info_file << course->name << '\n'
									  << course->teacher_name << '\n'
									  << course->credits << '\n'
									  << course->student_max << '\n';
							int day, time;
							course->class_session[0].getDayTimeInInt(day, time);
							info_file << day << '\n'
									  << time << '\n';
							course->class_session[1].getDayTimeInInt(day, time);
							info_file << day << '\n'
									  << time << '\n';

							Mark *mark = course->mark;
							while (mark)
							{
								mark_file << mark->id << '\n';
								mark_file << mark->midterm << ' ';
								mark_file << mark->final << ' ';
								mark_file << mark->other << ' ';
								mark_file << mark->total << '\n';

								mark = mark->next;
							}

							course = course->next;
						}
					}
				}
			}
			school_year = school_year->next;
		}
	}
}

int countYears(SchoolYear *school_year)
{
	int count = 0;
	while (school_year)
	{
		count++;
		school_year = school_year->next;
	}
	return count;
}

int countSemesters(SchoolYear *school_year)
{
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (school_year->semester[i].begin_date.day != DAY_INIT)
			count++;
	}
	return count;
}

int countMarks(Mark *mark)
{
	int count = 0;
	while (mark)
	{
		count++;
		mark = mark->next;
	}
	return count;
}

bool checkSchoolYearValid(SchoolYear *school_year)
{
	if (!school_year)
	{
		std::cout << "School year list is empty.\n";
		pressKeyToContinue();
	}

	return school_year != nullptr;
}

bool checkTimeOverlap(Session s1[2], Session s2[2])
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (s1[i] == s2[j])
				return 1;
		}
	}
	return 0;
}

void addCoursesNode(Course *&course_list)
{
	int day_temp, time_temp;
	std::string id;

	std::cout << "Input course id: ";
	getline(std::cin, id);

	Course *tail = course_list, *pre = nullptr;

	while (tail)
	{
		if (tail->id == id)
		{
			std::cout << "Course id already exists.\n";
			pressKeyToContinue();
			return;
		}
		else
		{
			pre = tail;
			tail = tail->next;
		}
	}

	if (!tail)
		tail = pre;

	Course *temp = new Course;

	temp->id = id;

	std::cout << "Input course name: ";
	getline(std::cin, temp->name);

	std::cout << "Input course lecturer: ";
	getline(std::cin, temp->teacher_name);

	std::cout << "Credits: ";
	std::cin >> temp->credits;

	temp->class_session->printSession();

	std::cout << "Day of session 1 in week: ";
	std::cin >> day_temp;
	temp->class_session[0].setDay(day_temp);

	std::cout << "Time: ";
	std::cin >> time_temp;
	temp->class_session[0].setTime(time_temp);

	std::cout << "Day of session 2 in week: ";
	std::cin >> day_temp;
	temp->class_session[1].setDay(day_temp);

	std::cout << "Time: ";
	std::cin >> time_temp;
	temp->class_session[1].setTime(time_temp);
	std::cin.ignore(1000, '\n');

	if (!course_list)
		tail = course_list = temp;
	else
	{
		tail->next = temp;
		tail = tail->next;
	}
	
	std::cout << "Course added done!\n";
	pressKeyToContinue();
}

void deleteCourseNode(Student *student, std::string id, int year, int semester, std::string course_id)
{
	while (student && student->id != id)
		student = student->next;

	if (!student)
		return;

	CourseEnrolled *list = student->enrolled;
	CourseEnrolled *dummy = new CourseEnrolled;
	dummy->next = list;
	list = dummy;

	while (list->next && list->next->course_id != course_id && list->next->year_enrolled != year && list->next->semester_index != semester)
		list = list->next;

	if (list->next && list->next->course_id == course_id && list->next->year_enrolled == year && list->next->semester_index == semester)
	{
		CourseEnrolled *temp = list->next;
		list->next = temp->next;
		delete temp;
	}

	list = dummy->next;
	delete dummy;
}

bool viewCourses(SchoolYear *school_year, int semester_index)
{
	clrscr();

	if (!checkSchoolYearValid(school_year))
		return 0;

	std::cout << "All available courses in semester " << semester_index + 1 << ":\n";
	int count = 0;

	Course *temp = school_year->semester[semester_index].course_list;
	while (temp)
	{
		std::cout << ++count << ". ";
		std::cout << "ID: " << temp->id << " Name: " << temp->name << '\n';
		temp = temp->next;
	}

	if (count == 0)
	{
		clrscr();
		std::cout << "No course is created in this semester. \n";
		pressKeyToContinue();
		return 0;
	}
	return 1;
}

SchoolYear *chooseSchoolYear(SchoolYear *school_year, int begin_year)
{
	if (begin_year == 0)
	{
		while (school_year && school_year->next)
			school_year = school_year->next;
		return school_year;
	}
	while (school_year && school_year->begin_year != begin_year)
		school_year = school_year->next;
	return school_year;
}

Course *chooseCourse(SchoolYear *school_year, int semester_index, std::string course_id, int begin_year)
{
	SchoolYear *find_year = chooseSchoolYear(school_year, begin_year);
	if (!find_year)
		return nullptr;

	if (semester_index < 0 || semester_index > 2)
		return nullptr;

	Course *find_course = find_year->semester[semester_index].course_list;

	while (find_course && find_course->id != course_id)
		find_course = find_course->next;
	return find_course;
}

float calculateTotalGPA(SchoolYear *school_year, Student *student)
{
	float total_gpa = 0.0;
	int total_credit = 0;
	CourseEnrolled *traverse = student->enrolled;

	while (traverse)
	{
		Course *course = chooseCourse(school_year, traverse->semester_index, traverse->course_id, traverse->year_enrolled);
		if (course)
		{
			Mark *find_mark = course->mark;

			while (find_mark && find_mark->id != student->id)
				find_mark = find_mark->next;

			if (find_mark && find_mark->final != -1)
			{
				total_gpa += course->credits * find_mark->total;
				total_credit += course->credits;
			}
		}
		traverse = traverse->next;
	}

	if (total_credit == 0)
		return 0.0;

	return total_gpa / total_credit;
}

float calculateSemesterGPA(SchoolYear *current_year, Student *student, int semester_index)
{
	float total_gpa = 0.0;
	int total_credit = 0;
	CourseEnrolled *traverse = student->enrolled;

	while (traverse)
	{
		if (traverse->semester_index == semester_index)
		{
			Course *course = chooseCourse(current_year, semester_index, traverse->course_id, current_year->begin_year);
			if (course)
			{
				Mark *find_mark = course->mark;

				while (find_mark && find_mark->id != student->id)
					find_mark = find_mark->next;

				if (find_mark && find_mark->total != -1)
				{
					total_gpa += course->credits * find_mark->total;
					total_credit += course->credits;
				}
			}
		}
		traverse = traverse->next;
	}

	if (total_credit == 0)
		return 0.0;

	return total_gpa / total_credit;
}
