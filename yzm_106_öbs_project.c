/*
 * Author        : Mustafa YALCIN
 * Number        : 210229038
 * Lecture       : YZM104 && YZM106
 * Final Project : OBS Project
 *
 */
#include <stdio.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE -1
#define LOGOUT 2
#define STRCMP_EQUAL 0

#define SIZE 32
#define SIZE_LINE 256
#define SIZE_USER 100
#define SIZE_COURSE 50

#define LIMIT_TERM 6
#define LIMIT_AKTS 20
#define LIMIT_OFFICER 1
#define LIMIT_ACADEMICIAN 4
#define LIMIT_STUDENT 100

typedef enum { R_NONE, ADMIN, OFFICER, ACADEMICIAN, STUDENT} Role;

typedef enum { D_NONE, SENG, CENG, EEE } Department;

typedef enum { UNGRADED, PASS, FAIL} State;

typedef struct
{
	char name[SIZE];
	int term;
	Department department;
} Course;

typedef struct
{
	Course *courses[SIZE];
} Academician;

typedef struct
{
	int term;
	Department department;
	Course *courses[SIZE];
	int midtermGrade[SIZE];
	int finalGrade[SIZE];
	int internship;
	State state[SIZE];
	int akts;
} Student;

typedef struct
{
	char username[SIZE];
	char password[SIZE];
	Role role;
	union
	{
		Academician academician;
		Student student;
	} info;
} User;

void askLogin(char username[], char password[]);
int login(User users[]);

int menu(User users[], Course courses[], int sign);
int adminMenu(User users[]);
int officerMenu(User users[], Course courses[]);
int academicianMenu(User users[], Course courses[], int sign);
int studentMenu(User users[], Course courses[], int sign);

int registerUser(User users[], Role role, int limit);
void registerStudent (User users[], Role role, int limit);
void openCourse(User users[], Course courses[]);
void choiceStudentCourse(User *user, Course courses[]);
int enterGrade(User users[], Course courses[], int sign);
void gradeStudent(User *user);
void createDocument(User *user);
void statusCheck(User *user);
void enterInternship(User users[]);

void deleteUser(User users[]);
void deleteCourse(Course courses[]);

void saveUsers(User users[]);
void saveCourses(Course courses[]);
void saveAcademicians(User users[]);
void saveStudents(User users[]);

int loadUsers(User users[]);
int loadCourses(Course courses[]);
int loadAcademicians(User users[], Course courses[]);
int loadStudents(User users[], Course courses[]);

int main()
{
	int ret=LOGOUT, sign=0;
	User users[SIZE_USER]={0};
	Course courses[SIZE_COURSE]={0};

	/*it fills the information in the file into arrays*/
	loadUsers(users);
	loadCourses(courses);  
	loadAcademicians(users, courses);
	loadStudents(users, courses);

	do
	{
		if (ret == LOGOUT)
		{
			sign = login(users);
			if(sign == FAILURE)
			{
				break;
			}
		}
		ret = menu(users, courses, sign);
	} while (ret != FAILURE);
	
	/*writes the information in the array to the file*/
	saveUsers(users);
	saveCourses(courses);
	saveAcademicians(users);
	saveStudents(users);

	return SUCCESS;
}


void askLogin(char username[], char password[])
{
	printf("----- ----- ----- -----\n");
	printf("|->Please enter username<-|\n");
	printf("Username: ");
	scanf(" %[^\n]%*c", username);
	printf("|->Please enter password<-|\n");
	printf("Password: ");
	scanf("%s", password);
}

/*it finds the role of the user who enters username and password.*/
int login(User users[])
{
	int i=0;
	char cont='a', username[SIZE]={0}, password[SIZE]={0};

	do
	{
		askLogin(username, password);
		for (i=0; i < SIZE_USER; ++i)
		{
			if (users[i].role != R_NONE &&
				STRCMP_EQUAL == strcmp(username, users[i].username) &&
				STRCMP_EQUAL == strcmp(password, users[i].password))
			{
				return i;
			}
		}

		printf("Wrong username or password!\n");
		printf("Do you want to try again? (y/n)\n");
		scanf(" %c", &cont);
	} while (cont == 'y');

	return FAILURE;
}

int menu(User users[], Course courses[], int sign)
{
	int ret=SUCCESS;

	switch (users[sign].role)
	{
		case ADMIN      : ret=adminMenu(users);                      break;
		case OFFICER    : ret=officerMenu(users, courses);           break;
		case ACADEMICIAN: ret=academicianMenu(users, courses, sign); break;
		case STUDENT    : ret=studentMenu(users, courses, sign);     break;
		default         : break;
	}

	return ret;
}

int adminMenu(User users[])
{
	int ret=SUCCESS, choice=0;

	printf("----- ----- ----- -----\n");
	printf("[1] Register Academician\n");
	printf("[2] Register Officer\n");
	printf("[3] Delete Users\n");
	printf("[4] Logout\n");
	printf("[5] Exit\n");
	scanf("%d", &choice);

	switch(choice)
	{
		case 1: registerUser(users, ACADEMICIAN, LIMIT_ACADEMICIAN); break;
		case 2: registerUser(users, OFFICER, LIMIT_OFFICER);         break;
		case 3: deleteUser(users);                                   break;
		case 4: ret=LOGOUT;                                          break;
		case 5: ret=FAILURE;                                         break;
	}

	return ret;
}

int officerMenu(User users[], Course courses[])
{
	int ret=SUCCESS, choice=0;

	printf("----- ----- ----- -----\n");
	printf("[1] Open Course\n");
	printf("[2] Delete Course\n");
	printf("[3] Register Student\n");
	printf("[4] Logout\n");
	printf("[5] Exit\n");
	scanf("%d", &choice);

	switch(choice)
	{
		case 1: openCourse(users, courses);                     break;
		case 2: deleteCourse(courses);                          break;
		case 3: registerStudent(users, STUDENT, LIMIT_STUDENT); break;
		case 4: ret=LOGOUT;                                     break;
		case 5: ret=FAILURE;                                    break;
	}

	return ret;
}

int academicianMenu(User users[], Course courses[], int sign)
{
	int ret=SUCCESS, choice=0;

	printf("----- ----- ----- -----\n");
	printf("[1] Enter Grade\n");
	printf("[2] Logout\n");
	printf("[3] Exit\n");
	scanf("%d", &choice);

	switch(choice)
	{
		case 1: enterGrade(users, courses, sign); break;
		case 2: ret=LOGOUT;                       break;
		case 3: ret=FAILURE;                      break;
	}

	return ret;
}

int studentMenu(User users[], Course courses[], int sign)
{
	int ret=SUCCESS, choice=0;

	printf("----- ----- ----- -----\n");
	printf("[1] Grade Status\n");
	printf("[2] Create Student Document\n");
	printf("[3] Choice Lesson\n");
	printf("[4] Logout\n");
	printf("[5] Exit\n");
	scanf("%d", &choice);

	switch(choice)
	{
		case 1: gradeStudent(&users[sign]);                 break;
		case 2: createDocument(&users[sign]);               break;
		case 3: choiceStudentCourse(&users[sign], courses); break;
		case 4: ret=LOGOUT;                                 break;
		case 5: ret=FAILURE;                                break;
	}

	return ret;
}

int registerUser(User users[], Role role, int limit)
{
	int i=0, count=0;

	for (i=0; i < SIZE_USER; ++i)
	{
		if (users[i].role == role)
		{
			++count;
		}
	}
	if (count < limit)
	{
		for (i=0; i < SIZE_USER; ++i)
		{
			if (users[i].role == R_NONE)
			{
				askLogin(users[i].username, users[i].password);
				users[i].role = role;
				break;
			}
		}
	}
	else
	{
		printf("You have reached the number of users limit!\n");
	}
	return i;
}

void registerStudent (User users[], Role role, int limit)
{
	int sign=0;

	sign = registerUser(users, role, limit);
	users[sign].info.student.term = 1;
	printf("Please enter department\n");
	printf("[1] Software engineering\n");
	printf("[2] Computer engineering\n");
	printf("[3] Electric electronic engineering\n");
	scanf("%u", &users[sign].info.student.department);
}

/*it determines which academician will teach the opened course.*/
void openCourse(User users[], Course courses[])
{
	int i=0, j=0, k=0;
	char academicianChoice[SIZE]={0};

	for (i=0; i < SIZE_COURSE; ++i)
	{
		if (courses[i].department == D_NONE)
		{
			printf("Please enter the name of the course\n");
			scanf(" %[^\n]%*c", courses[i].name);
			printf("Please enter the course department\n");
			printf("[1] Software engineering\n");
			printf("[2] Computer engineering\n");
			printf("[3] Electric electronic engineering\n");
			scanf("%u", &courses[i].department);
			printf("Please enter the course term:\n");
			printf("[1] 1st grade & autumn semester\n");
			printf("[2] 1st grade & spring semester\n");
			printf("[3] 2st grade & autumn semester\n");
			printf("[4] 2st grade & spring semester\n");
			scanf("%d", &courses[i].term);
			printf("Select the instructor you want to match the entered course with\n");
			
			for (j=0; j < SIZE_USER; ++j)
			{
				if (users[j].role == ACADEMICIAN)
				{
					printf("%s\n", users[j].username);
				}
			}
			scanf(" %[^\n]%*c", academicianChoice);

			for (j=0; j < SIZE_USER; ++j)
			{
				if (STRCMP_EQUAL == strcmp(academicianChoice, users[j].username))
				{
					for (k=0 ; k < SIZE; ++k)
					{
						if (users[j].info.academician.courses[k] == NULL)
						{
							users[j].info.academician.courses[k] = &courses[i];
							break;
						}
					}
					break;
				}			
			}
			break;
		}
	}
}

/*The student chooses the courses opened by the officer according to the Akts limit. 
  If the student has reached the akst limit and has not done an internship, he/she can only do the internship.*/
void choiceStudentCourse(User *user, Course courses[])
{
	int i=0, j=0, count=0;
 	char choiceCourse[SIZE]={0};

 	if (user->info.student.akts < LIMIT_AKTS)
 	{
		for (i=0; i < SIZE; ++i)
		{
			if (user->info.student.courses[i] != NULL)
			{
				if (user->info.student.state[i] == UNGRADED)
				{
					++count;
				}
			}
		}
		if (count == 2)
		{
			printf("You have reached the number of course select limit!\n");
			return;
		}

	 	count=0;
		for (i=0; i < SIZE; ++i)
		{
			if (user->info.student.department == courses[i].department)
			{
				if (user->info.student.term == courses[i].term)
				{
					printf("%s\n", courses[i].name);
					++count;
				}
				else if (user->info.student.term > 4)
				{
					if (courses[i].term >= 2 && user->info.student.state[i] != PASS)
					{
						printf("%s\n", courses[i].name);
						++count;
					}
				}
			}
		}
		if (count == 0)
		{
			printf("There are no courses that the student can choose!\n");
			return;
		}
		scanf(" %[^\n]%*c", choiceCourse);

		for (i=0; i < SIZE; ++i)
		{
			if (STRCMP_EQUAL == strcmp(choiceCourse, courses[i].name))
			{
				for (j=0 ; j < SIZE; ++j)
				{
					if (user->info.student.courses[j] == NULL)
					{
						user->info.student.courses[j] = &courses[i];
						break;
					}
				}
				break;
			}
		}
	}

	else if (user->info.student.internship == 0)
	{
		for (i=0; i < SIZE; ++i)
		{
			if (STRCMP_EQUAL == strcmp(user->info.student.courses[i]->name, "staj"))
			{
				printf("You have reached the Akts limit and you cannot choose a course\n");
				printf("You have an internship\n");
				return;
			}
		}
		for (i=0; i < SIZE; ++i)
		{
			if (STRCMP_EQUAL == strcmp(courses[i].name, "staj"))
			{
				printf("staj\n");
				scanf(" %[^\n]%*c", choiceCourse);
				if (STRCMP_EQUAL == strcmp(choiceCourse, "staj"))
				{
					for (j=0; j < SIZE; ++j)
					{
						if (user->info.student.courses[j] == NULL)
						{
							user->info.student.courses[j] = &courses[i];
							break;
						}
					}
				}
			}
		}
	}

	return;
}

/*The academician gives grades to the students who take the courses she teaches,
  and if the course is an internship, she makes a successful or unsuccessful entry*/
int enterGrade(User users[], Course courses[], int sign)
{
	int i=0, j=0, k=0; 
	int examChoice=0, lessonEqual=0;
	int gradeOrt=0;
	char lessonChoice[SIZE]={0};

	for (i=0; i < SIZE; ++i)
	{	
		if (users[sign].info.academician.courses[i] != NULL)
		{
			printf("%s\n", users[sign].info.academician.courses[i]->name);
		}
	}
	printf("Please select the course you want to enter the grade for\n");
	scanf(" %[^\n]%*c", lessonChoice);

	if (STRCMP_EQUAL == strcmp(lessonChoice, "staj"))
	{
		enterInternship(users);
		return SUCCESS;
	}
	for (i=0; i < SIZE; ++i)
	{
		if (users[sign].info.academician.courses[i] != NULL)
		{
			if (STRCMP_EQUAL == strcmp(lessonChoice, users[sign].info.academician.courses[i]->name))
			{
				lessonEqual = 1;
				break;
			}
		}
	}

	if (lessonEqual == 1)
	{
		printf("Select the grade you want to enter for the selected student\n");
		printf("[1] Midterm\n");
		printf("[2] Final\n");
		scanf("%d", &examChoice);

		printf("Enter the grades of the students who match the selected course:\n");
		for (j=0; j < SIZE_USER; ++j)
		{
			if (users[j].role == STUDENT)	
			{	
				for (k=0; k < SIZE; ++k)
				{
					if (users[j].info.student.courses[k] != NULL && users[j].info.student.state[k] == UNGRADED)
					{
						if (STRCMP_EQUAL == strcmp(users[j].info.student.courses[k]->name, users[sign].info.academician.courses[i]->name))
						{
							printf("%s: ", users[j].username);
							
							if (examChoice == 1)
							{
								scanf("%d", &users[j].info.student.midtermGrade[k]);
							}
							else if (examChoice == 2)
							{
								scanf("%d", &users[j].info.student.finalGrade[k]);
								gradeOrt = users[j].info.student.midtermGrade[k] + users[j].info.student.finalGrade[k];
								gradeOrt *= 0.5;
								if (gradeOrt >= 50)
								{
									users[j].info.student.state[k] = PASS;
									users[j].info.student.akts += 5;
								}
								else
								{
									users[j].info.student.state[k] = FAIL;
								}
							}
						}
					}
				}
			statusCheck(&users[j]);
			}
		}
	}

	return SUCCESS;
}

/*The student learns the grades and status of the courses taken during the semester*/
void gradeStudent(User *user)
{
	int i=0;

	for (i=0; i<SIZE; ++i)
	{
		if (user->info.student.courses[i] != NULL)
		{
			if (user->info.student.state[i] == UNGRADED)
			{
				printf("Lesson\tMidterm\tFinal\tState\n");
				printf("%s\t", user->info.student.courses[i]->name);
				printf("%d\t", user->info.student.midtermGrade[i]);
				printf("%d\t", user->info.student.finalGrade[i]);
				if (user->info.student.state[i] == PASS)
				{
					printf("PASS\n");
				}
				else if (user->info.student.state[i] == FAIL)
				{
					printf("FAIL\n");
				}
				else
				{
					printf("UNGRADED\n");
				}
			}
		}
	}
	printf("AKTS:%d\n", user->info.student.akts);
}

void createDocument(User *user)
{
	int i=0;
	FILE *fptr = fopen("Student Document.txt", "w");

	fprintf(fptr, "USERNAME:%s\n", user->username);
	fprintf(fptr, "Term:%d\n", user->info.student.term);
	if (user->info.student.department == SENG)
	{
		fprintf(fptr, "DEPARTMENT:SOFTWARE ENGINEERING\n");
	}
	else if (user->info.student.department == CENG)
	{
		fprintf(fptr, "DEPARTMENT:COMPUTER ENGINEERING\n");
	}
	else if (user->info.student.department == EEE)
	{
		fprintf(fptr, "DEPARTMENT:ELECTRIC-ELECTRONIC ENGINEERING\n");
	}
	
	for (i=0; i<SIZE; ++i)
	{
		if (user->info.student.courses[i] != NULL)
		{
			fprintf(fptr, "Lesson\tMidterm\tFinal\tState\n");
			fprintf(fptr, "%s\t", user->info.student.courses[i]->name);
			fprintf(fptr, "%d\t", user->info.student.midtermGrade[i]);
			fprintf(fptr, "%d\t", user->info.student.finalGrade[i]);
			
			if (user->info.student.state[i] == PASS)
			{
				fprintf(fptr, "SUCCESS\n");
			}
			else if (user->info.student.state[i] == FAIL)
			{
				fprintf(fptr, "FAILURE\n");
			}
		}
	}
	fprintf(fptr, "TOTAL AKTS:%d\n", user->info.student.akts);
}

/*It checks the status of the student and if he/she needs to skip a semester, he/she graduates if he/she has to graduate,
  if he/she cannot graduate in the given time, he/she is expelled from the school.*/
void statusCheck(User *user)
{
	int i=0;
	int stateCount=0, coursesCount=0;

	if (user->role == STUDENT)
	{
		for (i=0; i < SIZE; ++i)
		{
			if (user->info.student.courses[i] != NULL)
			{
				
				if (user->info.student.term == user->info.student.courses[i]->term)
				{
					++coursesCount;
					if (user->info.student.state[i] != UNGRADED)
					{
						++stateCount;
					}
				}
			}
		}
		if (coursesCount == stateCount && coursesCount != 0)
		{
			++user->info.student.term;
		}
		if (user->info.student.term > LIMIT_TERM)
		{
			user->info.student.department = D_NONE;
		}
		if (LIMIT_AKTS == user->info.student.akts &&
			SUCCESS == user->info.student.internship)
		{
			user->role = R_NONE;
		}
	}
}

/*Gets the information that the student's internship is successful or unsuccessful*/
void enterInternship(User users[])
{
	int i=0, j=0;

	printf("Internship Status:\n");
	printf("[1] SUCCESS\n");
	printf("[0] FAIL\n");

	for (i=0; i < SIZE_USER; ++i)
	{
		if (users[i].role == STUDENT)
		{
			for (j=0; j < SIZE; ++j)
			{
				if (users[i].info.student.courses[j] != NULL)
				{
					if (STRCMP_EQUAL == strcmp(users[i].info.student.courses[j]->name, "staj"))
					{
						printf("%s: ", users[i].username);
						scanf("%d", &users[i].info.student.internship);
						if (users[i].info.student.internship == 1)
						{
							users[i].info.student.state[j] = PASS;
						}
						else if (users[i].info.student.internship == 0)
						{
							users[i].info.student.state[j] = FAIL;
						}
						else
						{
							printf("Please try again\n");
						}
						statusCheck(&users[i]);
						break;
					}
				}
			}
		}
	}
}

/*Writes the username and password of the users to the file*/
void saveUsers(User users[])
{
	int i=0;
	FILE *fptr = fopen("users.txt", "w");

	for (i=0; i < SIZE_USER; ++i)
	{
		if (users[i].role != R_NONE)
		{
			fprintf(fptr, "%s;%s;%u\n",
				users[i].username,
				users[i].password,
				users[i].role);
		}
	}
	fclose(fptr);
}

/*writes the courses to the file*/
void saveCourses(Course courses[])
{
	int i=0;
	FILE *fptr = fopen("courses.txt", "w");

	for (i=0; i < SIZE_COURSE; ++i)
	{
		if (courses[i].department != D_NONE)
		{
			fprintf(fptr, "%s;%d;%u\n",
				courses[i].name,
				courses[i].term,
				courses[i].department);
		}
	}
	fclose(fptr);
}

/*writes the student to the file*/
void saveStudents(User users[])
{
	int i=0, j=0;
	FILE *fptr1 = fopen("students_info.txt", "w");
	FILE *fptr2 = fopen("students_courses_notes.txt", "w");

	for (i=0; i < SIZE_USER; ++i)
	{
		if (users[i].role == STUDENT)
		{
			fprintf(fptr1, "%s;%d;%u;%d\n",
				users[i].username,
				users[i].info.student.term,
				users[i].info.student.department,
				users[i].info.student.akts);
			
			for (j=0; j < SIZE; ++j)
			{
				if (users[i].info.student.courses[j] != NULL)
				{
					fprintf(fptr2, "%s;%s;%d;%d;%u\n",
					users[i].username,
					users[i].info.student.courses[j]->name,
					users[i].info.student.midtermGrade[j],
					users[i].info.student.finalGrade[j],
					users[i].info.student.state[j]);
				}
			}
		}
	}
	fclose(fptr1);
	fclose(fptr2);
}

/*writes the user to the file.*/
void saveAcademicians(User users[])
{
	int i=0, j=0;
	FILE *fptr = fopen("academicians_courses.txt", "w");

	for (i=0; i < SIZE_USER; ++i)
	{
		if (users[i].role == ACADEMICIAN)
		{
			for (j=0; j < SIZE; ++j)
			{
				if (users[i].info.academician.courses[j] != NULL)
				{
					fprintf(fptr, "%s;%s\n",
						users[i].username,
						users[i].info.academician.courses[j]->name);
				}
			}
		}
	}
	fclose(fptr);
}

/*It pulls the users username and password from the file*/
int loadUsers(User users[])
{
	int i=0;
	char line[SIZE_LINE]={0};
	FILE *fptr;

	if ((fptr = fopen("users.txt", "r")) == NULL) 
	{
		printf("LOAD USERS ERROR!\n");
		return FAILURE;
	}
	for (i=0; fgets(line, SIZE_LINE, fptr); ++i)
	{
		sscanf(line, "%[^;];%[^;];%u",
			users[i].username,
			users[i].password,
			&users[i].role);
	}
	fclose(fptr);
	return SUCCESS;
}

/*Extracts course information from the file*/
int loadCourses(Course courses[])
{
	int i=0;
	char line[SIZE_LINE]={0};
	FILE *fptr;

	if ((fptr = fopen("courses.txt", "r")) == NULL) 
	{
		printf("LOAD COURSES ERROR!\n");
		return FAILURE;
	}
	for (i=0; fgets(line, SIZE_LINE, fptr); ++i)
	{
		sscanf(line, "%[^;];%d;%u",
			courses[i].name,
			&courses[i].term,
			&courses[i].department);
	}
	return SUCCESS;
}

/*Extracts student information from the file.*/
int loadStudents(User users[], Course courses[])
{
	int i=0, j=0, k=0, found=0, term=0, midtermGrade=0, finalGrade=0, akts=0;
	State state;
	char line[SIZE_LINE]={0};
	char username[SIZE_LINE]={0};
	char coursename[SIZE_LINE]={0};
	Department department=D_NONE;
	FILE *fptr1, *fptr2;

	if ((fptr1 = fopen("students_info.txt", "r")) == NULL)
	{
		printf("LOAD STUDENT INFO ERROR!\n");
		return FAILURE;
	}
	if ((fptr2 = fopen("students_courses_notes.txt", "r")) == NULL)
	{
		printf("LOAD STUDENT COURSES NOTES ERROR!\n");
		return FAILURE;
	}

	while (fgets(line, SIZE_LINE, fptr1))
	{
		sscanf(line, "%[^;];%d;%u;%d", username, &term, &department, &akts);
		for (i=0; i < SIZE_USER ; ++i)
		{
			if (STRCMP_EQUAL == strcmp(username, users[i].username))
			{
				++found;
				break;
			}
		}
		users[i].info.student.term = term;
		users[i].info.student.department = department;
		users[i].info.student.akts = akts;
	}
	while (fgets(line, SIZE_LINE, fptr2))
	{
		found = 0;
		sscanf(line, "%[^;];%[^;];%d;%d;%u", username, coursename, &midtermGrade, &finalGrade, &state);
		for (i=0; i < SIZE_USER ; ++i)
		{
			if (STRCMP_EQUAL == strcmp(username, users[i].username))
			{
				++found;
				break;
			}
		}
		for (j=0; j < SIZE; ++j)
		{
			if (STRCMP_EQUAL == strcmp(coursename, courses[j].name))
			{
				++found;
				break;
			}
		}
		if (found == 2)
		{
			for (k=0; k < SIZE; ++k)
			{
				if (users[i].info.student.courses[k] == NULL)
				{
					users[i].info.student.courses[k] = &courses[j];
					users[i].info.student.midtermGrade[k] = midtermGrade;
					users[i].info.student.finalGrade[k] = finalGrade;
					users[i].info.student.state[k] = state;
					break;
				}
			}
		}
	}
	fclose(fptr1);
	fclose(fptr2);
	return SUCCESS;
}

/*Extracts academician information from the file.*/
int loadAcademicians(User users[], Course courses[])
{
	int i=0, j=0, k=0, found=0;
	char line[SIZE_LINE]={0};
	char username[SIZE_LINE]={0};
	char coursename[SIZE_LINE]={0};
	FILE *fptr;

	if ((fptr = fopen("academicians_courses.txt", "r")) == NULL) 
	{
		printf("LOAD ACADEMICIAN ERROR!\n");
		return FAILURE;
	}
	while (fgets(line, SIZE_LINE, fptr))
	{
		found = 0;
		sscanf(line, "%[^;];%[^\n]", username, coursename);
		for (i=0; i < SIZE_USER ; ++i)
		{
			if (STRCMP_EQUAL == strcmp(username, users[i].username))
			{
				++found;
				break;
			}
		}
		for (j=0; j < SIZE; ++j)
		{
			if (STRCMP_EQUAL == strcmp(coursename, courses[j].name))
			{
				++found;
				break;
			}
		}
		if (found == 2)
		{
			for (k=0; k < SIZE; ++k)
			{
				if (users[i].info.academician.courses[k] == NULL)
				{
					users[i].info.academician.courses[k] = &courses[j];
					break;
				}
			}
		}
	}
	fclose(fptr);
	return SUCCESS;
}

/*Deletes the registered user*/
void deleteUser(User users[])
{
	int i=0, role=0;
	char choiceUser[SIZE]={0};

	printf("Enter the role of the user you want to delete\n");
	printf("[1] OFFICER\n");
	printf("[2] ACADEMICIAN\n");
	printf("[3] STUDENT\n");
	scanf("%d", &role);
	role += 1;
	for (i=0; i < SIZE_USER; ++i)
	{
		if (users[i].role == role)
		{
			printf("%s\n", users[i].username);
		}
	}
	scanf(" %[^\n]%*c", choiceUser);
	for (i=0; i < SIZE_USER; ++i)
	{
		if (STRCMP_EQUAL == strcmp(choiceUser, users[i].username))
		{
			users[i].role = R_NONE;
			break;
		}
	}
}

/*Deletes the registered course*/
void deleteCourse(Course courses[])
{
	int i=0;
	char choiceCourse[SIZE]={0};

	for (i=0; i < SIZE; ++i)
	{
		if (courses[i].department != D_NONE)
		{
			printf("%s\n", courses[i].name);
		}
	}
	scanf(" %[^\n]%*c", choiceCourse);
	for (i=0; i < SIZE; ++i)
	{
		if (STRCMP_EQUAL == strcmp(choiceCourse, courses[i].name))
		{
			courses[i].department = D_NONE;
			break;
		}
	}
}
