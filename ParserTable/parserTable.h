#pragma once
/////////////////////////////////////////////////////////////////////////
// parserTable.h - Parser table related operations. Add,display and    //
//                 insert                                              //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 2 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////
/*
 * - addEntry(int,std::string,int)         Add entry to parser table (unordered_map(int,pair))
 * - displayParserTable(parserData)        Display the parserTable
 * - clearparsedData()                     Method to clear the parserTable
 * -  getParserTable()                     Return the parser table
 *
 * Build Process:
 * --------------
 * Required Files:
 *	parserTable.h
 *
* Maintenance History:
* --------------------
* ver 1.0 : 04 March 2019
* - first release
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include<iostream>
#include <vector>
#include <unordered_map>
#include <string>
class parserTable
{
public:
	using pair = std::pair<std::string, int>;
	using parserData = std::unordered_map<int, pair>;
	using fname = std::string;
	parserTable();
	~parserTable();
	void displayParserTable();
	void clearparsedData();
	void addEntry(int,std::string,int);
	parserData getParserTable();
private: 
	static parserData parsedData_;
};
std::unordered_map<int, parserTable::pair> parserTable::parsedData_;

parserTable::parserTable()
{
}

parserTable::~parserTable()
{
}
void parserTable::displayParserTable()
{
	std::cout << "\n Parser Dependency table is: \n";
	for (auto row : parsedData_)
	{
		std::cout << row.first << "\t- " << row.second.first << "\t- " << row.second.second << "\n";
	}
}

void parserTable::addEntry(int start, std::string type, int end)
{
	auto pair = std::make_pair(type, end);
	parsedData_.insert({ start,pair });
}
inline parserTable::parserData parserTable::getParserTable()
{
	return parsedData_;
}
inline void parserTable::clearparsedData() {
	parsedData_.clear();
}




