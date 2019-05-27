/////////////////////////////////////////////////////////////////////////
// dependency.h - Provides functionality to main a dependency          //
//                table in the form of an unordered map                //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project2 Spring 2019, CSE687 - Object Oriented Design  //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////

#include "dependency.h"



#ifdef TestDependency
int main()
{
	using file = std::string;
	using dependent = std::vector<std::string>;

	dependent dp;
	dp.push_back("#include \"../Display/display.h \" ");
	dp.push_back("#include \"../converter/converter.h \" ");
	dp.push_back("#include <../dep>");
	dependency dt;
	
	dt.addDependency("dependency.h", dp);
	dt.findDependency("dependency.h", dp);
	dt.isExistsDependency("dependency.h");
	dt.DTdisplay();

	return 0;
}
#endif // TestDependency