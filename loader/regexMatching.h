#pragma once
/////////////////////////////////////////////////////////////////////////
// regexMatching.h - Checks for a vaild regex pattern given            //
//					on command line and matches files to a             // 
//					regular expression and returns the matched files   //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 1 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
 * Checks if the regex given on command prompt is a valid regex and if
 * the regex is valid it returns files that are matched to the regex
 *
 * This package provides method:
 * - ProcessFiles(int argc, char *argv[])              Returns files matched to the pattern given on command line
 * - checkParseError(int argc, char *argv[])           Returns a boolean value=false if the regex is invalid
 * - fpath MatchFiles(int argc, char *argv[], fpath);  Returns files matched to the regex given on command line
 *
 * Build Process:
 * --------------
 * Required Files:
 *	regexMatching.h
 *  Utilities.h
 *	DirExplorerN.h
 *
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

#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/Utilities.h"
#include <iostream>
#include <regex>

class regexMatching
{
public:
	regexMatching();
	~regexMatching();

	using fpath = std::vector<std::string>;

	fpath ProcessFiles(int argc, char *argv[]);
	bool checkParseError(int argc, char *argv[]);
	fpath MatchFiles(int argc, char *argv[], fpath);

private:
	fpath files_;

};