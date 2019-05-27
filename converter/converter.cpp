/////////////////////////////////////////////////////////////////////////
// converter.cpp - Matches files to a regular expression and           //
//                 converters that file to HTML script                 //
// ver 1.2                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 2 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, Syracuse Univeristy            //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////

#include "converter.h"
#include "../Utilities/Utilities.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../FileSystem/FileSystem.h"


#ifdef TEST_CONVERTER // only compile the following when defined
int main()
{
	std::cout << "Requirement 8: This file includes an automated test that demonstrates that this package meets all the functional requirements";
	using file = std::vector<std::string>;
	file fname;
	fname.push_back("../converter/converter.h");

	converter conv;
	conv.checkDir();
	conv.createHTML(fname);


	return 0;
}

#endif // TEST_CONVERTER