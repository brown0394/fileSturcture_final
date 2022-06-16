// strclass.cc

#include "strclass.h"
#include <string.h>

String::String ()
{
	string = new char[10];
}

String::String (const String& str) //copy constructor
{
	string = strdup(str.string);
}

String::String (const char * str) // create from C string
{
	string = strdup (str);
}

String::~String ()
{
	delete[] string;
}

String & String::operator = (const String & str)
// assignment
{
	strcpy (string, str.string);
	return *this;
}

int String::operator < (const String &str) const
// less than, lexicographic
{	return strcmp(string, str.string) < 0;}

int String::operator <= (const String & str) const
{	return strcmp(string, str.string) <= 0;}

int String::operator == (const String & str) const
{	return strcmp(string, str.string) == 0;}

char * String::str () const // return a copy of the string
{	return strdup(string);}

char* String::getStr(){
	return this->string;
}