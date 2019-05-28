/////////////////////////////////////////////////////////////////////////
// executive.cpp - Executive file for Project 1 Spring 2019            //
// ver 1.1                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 2 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////

#include "executive.h"
#include "../converter/converter.h"
#include "../loader/regexMatching.h"
#include "../Dependencies/findDependency.h"
#include <iostream>

executive::executive()
{
}

executive::~executive()
{
}


std::vector<std::string> executive::exec(int argc, char *argv[])
//void executive::exec(int argc, char *argv[])
{
	std::cout << "\nRequirement 1: This project uses Visual Studio 2017 and its C++ Windows Console Projects";
	try
	{
		converter conv;
		regexMatching regMatch;
		int flag = regMatch.checkParseError(argc, argv);
		if (flag)
			std::cout << "\nRegex Parsing error";
		auto files = regMatch.ProcessFiles(argc, argv);
		if (files.size() == 0)
		{
			std::cout << "\n No matching files found to process";
			std::cout << "\n";
			//return;
		}
		files = regMatch.MatchFiles(argc, argv, files);
		files_ = files;
		findDependency dep;
		auto file = dep.findDep(files);

		dependency DT;
		DT.DTdisplay();

		conv.checkDir();
		auto htmlFiles = conv.createHTML(file);
		//files_ = htmlFiles;

		std::cout << "\n\n";
	}
	catch (std::exception&)
	{
		std::cout << "\n Error\n";
	}
	catch (...)
	{
		std::cout << "\n Unknown exception \n";
	}
	return files_;
}


#ifdef testExec
int main(int argc, char *argv[])
{
	executive exc;
	exc.exec(argc, argv);
	return 0;
}
#endif // testExec
