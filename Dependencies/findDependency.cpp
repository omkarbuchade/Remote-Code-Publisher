/////////////////////////////////////////////////////////////////////////
// findDependency.cpp - Supports insertion, in each analyzed file,     // 
//                    links to all the files in the directory tree,    // 
//                    on which it depends a vaild regex pattern given  //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 1 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////

#include "findDependency.h"


#ifdef testFindDep
int main()
{
	using fpath = std::vector<std::string>;
	using pair = std::pair<std::string, int>;
	using parserData = std::unordered_map<int, pair>;
	using fname = std::string;

	findDependency fd;

	fpath vec, vec1;


	vec.push_back("../FindDependency/findDependency.h");
	vec1.push_back("../FindDependency/findDependency.cpp");

	fd.findDep(vec);
	vec1 = fd.checkDupliactes(vec, vec1);
	parserTable = fd.createParserTable("../FindDependency/findDependency.h");
	return 0;
}

#endif // testFindDep

