#pragma once
/////////////////////////////////////////////////////////////////////////
// dependency.h - Provides functionality to main a dependency          //
//                table in the form of an unordered map                //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project2 Spring 2019, CSE687 - Object Oriented Design  //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
 * The dependency table package provides functionality to manage the dependency 
 * table.  It supports adding a depndency to the dependency table and check for 
 * vaild dependencies. It also has the functionality to display dependency table
 *
* - addDependency(std::string, vector)      Add a dependency to the dependency table
* -	findDependency(std::string,dependent)   Checks for vaild dependency files and returns them 
* -	isExistsDependency(keys)                Checks if there exists a given key
* -	DTdisplay()                             Display the dependency table
* -	dependencies(std::string)               Return a list of dependencies for a given file

 * Build Process:
 * --------------
 * Required Files:
 *  dependency.h
 *	FileSystem.h
* Maintenance History:
* --------------------
* ver 1.0 : 03 March 2019
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



#include <iostream>
#include <unordered_map>
#include <regex>
#include "../FileSystem/FileSystem.h"

using namespace FileSystem;
class dependency
{
public:
	dependency();
	~dependency();

	using file = std::string;
	using keys = std::string;
	using dependent = std::vector<std::string>;
	void addDependency(file, dependent);
	dependent findDependency(std::string,dependent);
	bool isExistsDependency(keys);
	void DTdisplay();
	dependent dependencies(std::string);
	
private:
	static std::unordered_map<file, dependent> depTable_;
};
std::unordered_map<dependency::file, dependency::dependent> dependency::depTable_;

dependency::dependency()
{
}

dependency::~dependency()
{
}
void dependency::addDependency(dependency::file File, dependency::dependent Dep)
{
	std::string fname = FileSystem::Path::getName(File);

	auto Key = depTable_.find(fname);
	if (Key != depTable_.end()) {
		std::cout << "\n dep already present";
	}
	else
	{
		depTable_.insert({ fname,Dep });
	}
}

dependency::dependent dependency::findDependency(std::string f, dependency::dependent Dependencies)
{
	std::string reStr = "(^\\s*#\\s*include\\s*\")(.*)(\"\\s*$)";
	std::regex re(reStr);
	std::smatch match;
	dependent dependentList;
	for (auto statement : Dependencies)
	{
		if (std::regex_match(statement, match, re))
		{
			std::string path = match[2];

			path = FileSystem::Path::getPath(f) + path;
			if (FileSystem::File::exists(path))
			{
				std::string fullFile = FileSystem::Path::getFullFileSpec(path);
				dependentList.push_back(fullFile);
			}
			else
				std::cout << "\nDependency file: " << path << " not found" << "\n";
		}
	}
	return (dependentList);
}
bool dependency::isExistsDependency(keys key)
{
	auto Key = depTable_.find(key);
	if (Key != depTable_.end()) {
		return true;
	}
	else
		return false;
}

void dependency::DTdisplay()
{
	std::cout << "\n Dependency table is: \n";
	for (auto row : depTable_)
	{
		std::cout << "\n + " << row.first << "\n";

		for (auto element : row.second)
		{
			std::cout << "\t- " << element << "\n";
		}
	}
}

dependency::dependent dependency::dependencies(std::string key)
{
	auto Key = depTable_.find(key);
	if (Key != depTable_.end()) {
		return Key->second;
	}
	else
	{
		std::cout << "no key found \n";
		return dependent();
	}
}