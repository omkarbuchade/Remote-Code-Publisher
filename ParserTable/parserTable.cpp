/////////////////////////////////////////////////////////////////////////
// parserTable.cpp - Parser table related operations. Add,display and  //
//                 insert                                              //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 2 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////

#include "parserTable.h"

#ifdef TestPT
int main()
{
	parserTable pt;
	parserTable::parserData pd;
	pt.clearparsedData();
	pt.addEntry(1, "main", 20);
	pt.addEntry(3, "add",14);
	pt.displayParserTable();
	pd=pt.getParserTable();

	return 0;
}
#endif // TestPT