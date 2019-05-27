#pragma once
/////////////////////////////////////////////////////////////////////////
// executive.h - header file for executive                             //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project1 Spring 2019, CSE687 - Object Oriented Design  //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////

/*
* Build Process:
* --------------
* Required Files:
*   executive.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 3 Feb 2019
* - first release
* Notes:
* ------
* - none yet
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include<vector>
#include<iostream>
class executive
{
public:
	executive();
	~executive();
	std::vector<std::string> exec(int, char **);
	//void exec(int, char **);

private:
	std::vector<std::string> files_;
};

