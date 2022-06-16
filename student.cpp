#include "student.h"
#include <strstream>

Student::Student() :SYear{ 0 }, SId{ '\0' }, Dept{ '\0' }, SName{'\0'}
{
}

Student::Student(const Student& student) {
	strcpy_s(this->SId, SIDMAX, student.SId);
	strcpy_s(this->SName, SNAMEMAX, student.SName);
	strcpy_s(this->Dept, DEPTMAX, student.Dept);
	this->SYear = student.SYear;
}

Student::Student(char* sId, char* sName, char* department, int sYear) {
	strcpy_s(this->SId, SIDMAX, sId);
	strcpy_s(this->SName, SNAMEMAX, sName);
	strcpy_s(this->Dept, DEPTMAX, department);
	this->SYear = sYear;
}

Student::Student(const char* sId, const char* sName, const char* department, int sYear) {
	strcpy_s(this->SId, SIDMAX, sId);
	strcpy_s(this->SName, SNAMEMAX, sName);
	strcpy_s(this->Dept, DEPTMAX, department);
	this->SYear = sYear;
}

Student::~Student() {

}

Student& Student::operator =(const Student& student) {
	strcpy_s(this->SId, SIDMAX, student.SId);
	strcpy_s(this->SName, SNAMEMAX, student.SName);
	strcpy_s(this->Dept, DEPTMAX, student.Dept);
	this->SYear = student.SYear;
	return *this;
}

void Student::setId(const char * id) {
	strcpy_s(this->SId, SIDMAX, id);
}
char* Student::getId() {
	return this->SId;
}

void Student::setName(const char *name) {
	strcpy_s(this->SName, SNAMEMAX, name);
}
char* Student::getName() {
	return this->SName;
}
void Student::setDepartment(const char *department) {
	strcpy_s(this->Dept, DEPTMAX, department);
}
char* Student::getDepartment() {
	return this->Dept;
}
void Student::setYear(const int year) {
	this->SYear = year;
}
int Student::getYear() {
	return this->SYear;
}

std::ostream& operator << (std::ostream& stream, const Student& student) {
	stream << "Student Id : " << student.SId << "\nStudent Name : " << student.SName <<
		"\nDepartment : " << student.Dept << "\nAcademic Year : " << student.SYear << "\n\n";
	return stream;
}

std::istream& operator >> (std::istream& stream, Student& student) {
	char buffer[14]{'\0'};
	std::cout << "Enter student id, or <cr> to end: " << std::flush;
	stream.getline(buffer, SIDMAX);
	student.setId(buffer);
	if (strlen(student.SId) == 0) return stream;
	std::cout << "Enter name: " << std::flush; stream.getline(buffer, SNAMEMAX);
	student.setName(buffer);
	std::cout << "Enter department: " << std::flush; stream.getline(buffer, DEPTMAX);
	student.setDepartment(buffer);

	std::cout << "Enter current academic year: " << std::flush; stream.getline(buffer, 2);
	student.setYear(buffer[0] - '0');
	return stream;
}

void Student::Clear()
{
	// set each field to an empty string
	SId[0] = 0;
	SName[0] = 0;
	Dept[0] = 0;
	SYear = 0;
}

int Student::InitBuffer(FixedFieldBuffer& Buffer)
// initialize a FixedTextBuffer to be used for Student
{
	int result;
	result = Buffer.AddField(SIDMAX-1); //SId
	result = result && Buffer.AddField(SNAMEMAX-1); //SName
	result = result && Buffer.AddField(DEPTMAX-1); //Dept
	result = result && Buffer.AddField(1);//SYear
	return result;
}

int Student::InitBuffer(LengthFieldBuffer& Buffer)
// initialize a LengthTextBuffer to be used for Persons
{
	return TRUE;
}
/*
int Student::InitBuffer(DelimFieldBuffer& Buffer)
// initialize a DelimTextBuffer to be used for Persons
{
	return TRUE;
}*/

int Student::Pack(IOBuffer& Buffer) const
{// pack the fields into a FixedFieldBuffer, 
 // return TRUE if all succeed, FALSE o/w
	int numBytes;
	Buffer.Clear();
	numBytes = Buffer.Pack(SId);//동적 바인딩
	if (numBytes == -1) return FALSE;
	numBytes = Buffer.Pack(SName);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer.Pack(Dept);
	if (numBytes == -1) return FALSE;
	char year[2]{ "0" };
	year[0] = '0' + this->SYear;
	numBytes = Buffer.Pack(year);
	if (numBytes == -1) return FALSE;
	return TRUE;
}

int Student::Unpack(IOBuffer& Buffer)
{
	Clear();
	int numBytes;
	//power of virtual functions
	numBytes = Buffer.Unpack(SId);//Unpack()에 대항 F12를 누르면 일치하는 3개 함수 존재 - 동적 바인딩
	if (numBytes == -1) return FALSE;
	SId[numBytes] = 0;
	numBytes = Buffer.Unpack(SName);
	if (numBytes == -1) return FALSE;
	SName[numBytes] = 0;
	numBytes = Buffer.Unpack(Dept);
	if (numBytes == -1) return FALSE;
	Dept[numBytes] = 0;
	char num[2];
	numBytes = Buffer.Unpack(num);
	if (numBytes == -1) return FALSE;
	num[numBytes] = 0;
	SYear = num[0] - '0';
	return TRUE;
}

char* Student::Key() const
{// produce key as concatenation of Label and IdNum
	std::ostrstream key;
	key << SId << ends;
	return key.str();
}

