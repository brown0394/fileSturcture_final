#pragma once

#ifndef STUDENT
#define STUDENT

#include <iostream>
#include "fixfld.h"
#include "iobuffer.h"
#include "length.h"
#define SIDMAX 5
#define SNAMEMAX 17
#define DEPTMAX 14

class Student
{
	// data members
	char SId[SIDMAX];
	char SName[SNAMEMAX]; 
	char Dept[DEPTMAX];	
	int SYear;

	friend std::ostream& operator << (std::ostream& stream, const Student& student);
	friend std::istream& operator >> (std::istream& stream, Student& student);

public:
	// method
	Student(); // default constructor
	Student(const Student& student);
	Student(char* sId, char* sName, char* department, int sYear);
	Student(const char* sId, const char* sName, const char* department, int sYear);
	~Student();

	Student& operator =(const Student&);
	void setId(const char*);
	char *getId();
	void setName(const char*);
	char *getName();
	void setDepartment(const char*);
	char *getDepartment();
	void setYear(const int);
	int getYear();

	void Clear();
	char* Key() const;
	int Unpack(IOBuffer&);
	int Pack(IOBuffer&) const;
	static int InitBuffer(FixedFieldBuffer& Buffer);
	static int InitBuffer(LengthFieldBuffer&);
	//static int InitBuffer(DelimFieldBuffer&);

};

#endif