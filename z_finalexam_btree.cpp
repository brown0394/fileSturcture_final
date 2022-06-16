
#include <iostream>
#include <string>
#include "btree.h"
#include "btnode.h"
#include "student.h"
#include "CourseRegistration.h"
#include "length.h"
#include "strclass.h"
#include <iomanip>
#include "direct.h"
#include <conio.h>
#include "recording.h"
#include <fstream>
#include "iobuffer.h"
#include "fixfld.h"
#include "length.h"
#include "buffile.h"
#include "recfile.h"
#include "indbuff.h"

#define STRECORDS 7
#define CRRECORDS 35

using namespace std;

const char* keys;
const int BTreeSize = 4;//Btree node 용량은 4개로 한다


void markDeletedStudent(Student& student) {
	student.setId("0000");
	student.setName("0000");
	student.setDepartment("0000");
	student.setYear(0);
}

void markDeletedCourse(CourseRegistration& course) {
	course.setCourseName("0000");
	course.setStudentId("0000");
	course.setGrade(0);
}

void getKey(const char* sid, const char* courseName, char* at) {
	strcpy(at, sid);
	char half[10];
	int i = 0;
	for (; i < strlen(at); ++i) {
		if (courseName[i] == ' ') break;
		half[i] = courseName[i];
	}
	half[i] = 0;
	strcat(at, half);
}

int createStudentDataFile(RecordFile<Student>& dataFile, char* filename) {
	int result = dataFile.Create(filename, ios::in | ios::out);
	cout << "Create " << filename << endl;
	if (!result)
	{
		cout << "Unable to Create file : " << "studentDataFile.dat" << endl;
		return -1;
	}
	Student students[STRECORDS] = {
		{"s123", "KilDong Hong", "Computer", 4}, {"s124", "KilSun Hong", "Electronics", 4},
		{"s125", "ChulSoo Kim", "Computer", 2}, {"s126", "YeongHee Kim", "Computer", 1},
		{"s127", "KukJeong Lim", "Electronics", 3}, {"s128", "MongRyong Lee", "Electronics", 3},
		{"s129", "ChunHyang Seong", "Computer", 4}
	};
	int recaddr = 0;
	for (int i = 0; i < STRECORDS; ++i) {
		recaddr = dataFile.Write(students[i]);
		cout << "DataFile R[" << i << "] at recaddr " << recaddr << endl;
	}
	dataFile.Close();
	return 0;
}
int createCourseDataFile(RecordFile<CourseRegistration>& dataFile, char* filename) {
	CourseRegistration courses[CRRECORDS] = {
		{"s123", "OS", 4.5}, {"s123", "DB", 4.0}, {"s123", "CPP", 4.0}, {"s123", "AI", 3.5}, {"s123", "Big Data", 3.5},
		{"s124", "C Programming", 4.0}, {"s124", "Data Structure", 4.0}, {"s124", "OS", 3.5}, {"s124", "AI", 3.0}, {"s124", "Web Programming", 3.5},
		{"s125", "DB", 3.5}, {"s125", "Network", 4.0}, {"s125", "OS", 4.0}, {"s125", "Big Data", 3.5}, {"s125", "File Structure", 4.5},
		{"s126", "AI", 3.0}, {"s126", "File Structure", 4.0}, {"s126", "Data Structure", 3.5}, {"s126", "C Programming", 4.5}, {"s126", "CPP", 4.0},
		{"s127", "CPP", 4.5}, {"s127", "Web Programming", 4.5}, {"s127", "Big Data", 4.5}, {"s127", "File Structure", 4.5}, {"s127", "DB", 4.5},
		{"s128", "OS", 4.5}, {"s128", "DB", 3.5}, {"s128", "CPP", 3.5}, {"s128", "Network", 3.5}, {"s128", "Web Programming", 3.0},
		{"s129", "File Structure", 4.0}, {"s129", "Data Structure", 4.0}, {"s129", "CPP", 4.5}, {"s129", "AI", 3.5}, {"s129", "DB", 4.5}
	};
	int recaddr = 0;
	int result = dataFile.Create(filename, ios::in | ios::out);
	if (!result)
	{
		cout << "Unable to open data file" << endl;
		dataFile.Close();
		return -1;
	}
	for (int i = 0; i < CRRECORDS; ++i) {
		recaddr = dataFile.Write(courses[i]);
		cout << "DataFile R[" << i << "] at recaddr " << recaddr << endl;
	}
	dataFile.Close();
	return 0;
}


template <class BTreeType>
int createBtFile(RecordFile<Student>& dataFile, BTree<BTreeType>& bt, char* dataFilename, char* btFilename) {

	int result = bt.Create(btFilename, ios::out | ios::in);
	if (!result) {
		cout << "Please delete " << btFilename << endl;
		system("pause");
		return 0;
	}

	int recaddr = 0;
	Student dataElem;
	result = dataFile.Open(dataFilename, ios::in);
	if (!result)
	{
		cout << "Unable to open data file" << endl;
		return -1;
	}

	dataFile.Rewind();
	while (1) {
		recaddr = dataFile.Read(dataElem);
		if (recaddr == -1) break;
		char* keys = dataElem.getId();
		cout << "Inserting with key = " << keys << endl;

		result = bt.Insert(keys, recaddr);
		//bt.Print(cout);
		//putchar('\n');
		//delete[] keys;
	}
	dataFile.Close();
	bt.Close();
	cout << endl;
	return 0;
}
template <class BTreeType>
int createBtFile(RecordFile<CourseRegistration>& dataFile, BTree<BTreeType>& bt, char* dataFilename, char* btFilename) {

	int result = bt.Create(btFilename, ios::out | ios::in);
	if (!result) {
		cout << "Please delete " << btFilename << endl;
		system("pause");
		return 0;
	}

	int recaddr = 0;
	CourseRegistration dataElem;
	result = dataFile.Open(dataFilename, ios::in);
	if (!result)
	{
		cout << "Unable to open data file" << endl;
		return -1;
	}

	dataFile.Rewind();
	while (1) {
		recaddr = dataFile.Read(dataElem);
		if (recaddr == -1) break;
		char* keys = new char[20];
		getKey(dataElem.getId(), dataElem.getCourseName(), keys);
		cout << "Inserting with key = " << keys << endl;

		result = bt.Insert(keys, recaddr);
		//bt.Print(cout);
		//putchar('\n');
		delete[] keys;
	}
	dataFile.Close();
	bt.Close();
	cout << endl;
	return 0;
}
template <class BTreeType>
int printCourseGrades(RecordFile<Student>& stDataFile, RecordFile<CourseRegistration>& crDataFile, BTree<BTreeType>& stBt, BTree<BTreeType>& crBt,
	char* studentDataFilename, char* studentBtFilename, char* courseDataFilename, char* courseBtFilename, const char courseList[][20]) {
	Student student;
	CourseRegistration course;
	int recaddr = 0;
	int result = stDataFile.Open(studentDataFilename, ios::in);
	if (!result)
	{
		cout << "Unable to open student data file" << endl;
		return -1;
	}
	result = stBt.Open(studentBtFilename, ios::in | ios::out);
	if (result == 0)
	{
		cout << "Please create " << studentBtFilename << ".dir and " << studentBtFilename << ".bkt" << endl;
		return -1;
	}
	char inputKey[30];
	cout << "Insert student id to search : ";
	cin >> inputKey;
	recaddr = stBt.Search(inputKey);
	if (recaddr == -1) {
		cout << "Found no matching key with " << inputKey << endl;
		return -1;
	}
	stDataFile.Read(student, recaddr);
	stDataFile.Close();
	stBt.Close();
	cout.setf(ios::left, ios::adjustfield);
	cout << '\n' << setw(5) << student.getId() << setw(17) << student.getName() << setw(10) <<
		student.getDepartment() << setw(2) << student.getYear() << endl;

	result = crDataFile.Open(courseDataFilename, ios::in);
	if (!result)
	{
		cout << "Unable to open course data file" << endl;
		return -1;
	}
	result = crBt.Open(courseBtFilename, ios::in | ios::out);
	if (result == 0)
	{
		cout << "Please create " << courseBtFilename << ".dir and " << courseBtFilename << ".bkt" << endl;
		return -1;
	}
	for (int i = 0; i < 10; ++i) {
		getKey(student.getId(), courseList[i], inputKey);
		recaddr = crBt.Search(inputKey);
		if (recaddr < 0) continue;
		crDataFile.Read(course, recaddr);
		cout.setf(ios::left, ios::adjustfield);
		cout << "      " << setw(18) << course.getCourseName();
		cout.setf(ios::left, ios::fixed | ios::floatfield);
		cout << setw(5) << setprecision(2) << course.getGrade() << endl;
	}

	crDataFile.Close();
	crBt.Close();
	return 0;
}

template <class BTreeType>
int printStudent(RecordFile<Student>& stDataFile, BTree<BTreeType>& bt, char* studentDataFilename, char* studentBtFilename) {
	Student student;
	CourseRegistration course;
	int recaddr = 0;
	int result = stDataFile.Open(studentDataFilename, ios::in);
	if (!result)
	{
		cout << "Unable to open student data file" << endl;
		return -1;
	}
	result = bt.Open(studentBtFilename, ios::in | ios::out);
	if (result == 0)
	{
		cout << "Please create " << studentBtFilename << ".dir and " << studentBtFilename << ".bkt" << endl;
		return -1;
	}
	char inputKey[30];
	cout << "Insert student id to search : ";
	cin >> inputKey;
	recaddr = bt.Search(inputKey);
	if (recaddr == -1) {
		cout << "Found no matching key with " << inputKey << endl;
		return -1;
	}
	stDataFile.Read(student, recaddr);
	cout << student;
	stDataFile.Close();
	bt.Close();
	return 0;
}


int main(int argc, char** argv)
{
	//변수 선언 코드
	const char courseList[10][20] = {
		{"DB"}, {"OS"}, {"File Structure"}, {"Data Structure"},
		{"CPP"}, {"C Programming"}, {"Network"},
		{"Web Programming"}, {"AI"}, {"Big Data"}
	};
	char* studentDataFileName = new char[20] {"studentDataFile.dat"};
	char* courseDataFileName = new char[20] {"courseDataFile.dat"};
	char* studentBtFileName = new char[20] {"studentBtFile.dat"};
	char* courseBtFileName = new char[20] {"courseBtFile.dat"};
	int select = 0;
	FixedFieldBuffer stBuffer(4);
	Student::InitBuffer(stBuffer);
	FixedFieldBuffer crBuffer(3);
	CourseRegistration::InitBuffer(crBuffer);
	RecordFile<Student> stDataFile(stBuffer);
	RecordFile<CourseRegistration> crDataFile(crBuffer);

	BTree <String> stBt(BTreeSize, 5);
	BTree <String> crBt(BTreeSize, 12);

	stDataFile.Close();
	while (1)
	{
		cout << "\nSelect command 1: insert student record set, 2: insert course registration record set, 3. create a btree file from an student record file," <<
			"4. create a btree file from an course registration record file, 5. print the student that match the input Sid" <<
			"6. print the all course grades that match the input Sid, 7. Quit => ";
		cin >> select;

		switch (select) {
		case 1://1: insert student record set
			createStudentDataFile(stDataFile, studentDataFileName);
			//R[0] = new Recording("LON", "2312", "Romeo and Juliet", "Prokofiev", "Maazel");
			//6개의 record일 data file에 쓰기
			break;
		case 2://2: insert course registration record set
			createCourseDataFile(crDataFile, courseDataFileName);
			//R[0] = new Recording("LON", "2312", "Romeo and Juliet", "Prokofiev", "Maazel");
			//6개의 record일 data file에 쓰기
			break;

		case 3://3. create a btree file from an student record file
			createBtFile(stDataFile, stBt, studentDataFileName, studentBtFileName);
			break;

		case 4://4. create a btree file from an course registration record file
			createBtFile(crDataFile, crBt, courseDataFileName, courseBtFileName);
			break;
		case 5://5. print the student that match the input Sid
			printStudent(stDataFile, stBt, studentDataFileName, studentBtFileName);
			break;
		case 6://6. print the all course grades that match the input Sid
			printCourseGrades(stDataFile, crDataFile, stBt, crBt, studentDataFileName, studentBtFileName,
				courseDataFileName, courseBtFileName, courseList);
			break;

		default:
			exit(0);
			break;
		}
	}

	system("pause");
	return 1;
}