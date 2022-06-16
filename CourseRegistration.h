#pragma once

#ifndef COURSE
#define COURSE

#include <iostream>
#include "iobuffer.h"
#include "fixfld.h"
#include "length.h"
//#include "delim.h"
#define SIDMAX 5
#define CNAMEMAX 17

class CourseRegistration
{
	char SId[SIDMAX];
	char CourseName[CNAMEMAX];
	float Grade;

	friend std::ostream& operator << (std::ostream& stream, const CourseRegistration& course);
	friend std::istream& operator >> (std::istream& stream, CourseRegistration& course);
public:
	// method
	CourseRegistration(); // default constructor
	CourseRegistration(const CourseRegistration&);
	CourseRegistration(char *studentId, char *courseName, float grade);
	CourseRegistration(const char* studentId, const char* courseName, float grade);
	~CourseRegistration();

	CourseRegistration& operator =(const CourseRegistration&);
	void setStudentId(const char*);
	char* getId();
	void setGrade(float);
	float getGrade();
	void setCourseName(const char* courseName);
	char* getCourseName();


	void Clear();
	char* Key() const;
	int Unpack(IOBuffer&);
	int Pack(IOBuffer&) const;
	static int InitBuffer(FixedFieldBuffer& Buffer);
	static int InitBuffer(LengthFieldBuffer&);
	//static int InitBuffer(DelimFieldBuffer&);

	ostream& PrintLine(ostream& stream);
};

#endif