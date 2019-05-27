#pragma once
/////////////////////////////////////////////////////////////////////////
// converter.h - Matches files to a regular expression and             //
//               converters those file into HTML script                //
// ver 1.1                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project1 Spring 2019, CSE687 - Object Oriented Design  //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
 * converter provides a class, of the same name. The createHTML method
 * is used to generate HTML pages for the files that match the regular 
 * expression and stores them by creating a directory in the solution 
 * folder.  getConvertedDir() method is used to get the path of the 
 * directory in which the converted files are saved. There are several 
 * other helper functions which help the createHTML method
 *
 * This package provides method:
 * - checkDir()                             Creates a directory to store HTML files if it does not exist 
 * - createHTML(fpath)                      Generates HTML scripts of files that match the regular expression
 * - line beginHtml()                       Returns the html tags that need to be added in the html file
 * - line EndHtml()                         Returns a string with the closing html tags as required	
 * - line titleHtml(line)                   Returns a string that servs as the title tag for the html file
 * - line headHtmlEnd()                     Returns a string to end the head tag of html
 * - line preHtml()                         Returns a sting with the opening pre tag that needs to be added to html file
 * - line preCloseHtml()                    Returns a sting with the closing pre tag that needs to be added to html file
 * - line writeDepHtml(line)                Returns with <a href> tags for hyperlinks to the dependent files
 * - line findMarkov(line)                  Finds and replaces markup characters that need to be changed in html file 
 * - line addSingleLineCommentDiv(line)     Function to add a div tag for single line comments
 * - line addMultiLineCommentDiv(line)      Method to add div tag to multi line comments 
 * - line addDiv(parserData, int, line)     Medthod to add div and button tags for methods, classes and namespaces
 * - line createButton(line,int)            Method to create a button with name given as parameter to the function
 *
 * Build Process:
 * --------------
 * Required Files:
 *	converter.h
 *	FileSystem.h
 *	dependency.h     //dependency table
 *	findDependency.h //find dependencies for a given file
 *
* Maintenance History:
* --------------------
* ver 1.1 : 03 March 2019
* - Made the converter package independent by removing the pattern matching methods
* - added method to add div tags and buttons for comments, functions,classes and namespaces
* - added methods to read required html tags from a template file
* - added method to remove markup characters from the .cpp and .h files
* ver 1.0 : 03 Feb 2019
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

#include "../FileSystem/FileSystem.h"
#include "../dependencyTable/dependency.h"
#include "../Dependencies/findDependency.h"
#include "../ParserTable/parserTable.h"

class converter
{
public:
	converter();
	~converter();

	using fpath = std::vector<std::string>;
	using pair = std::pair<std::string, int>;
	using parserData = std::unordered_map<int, pair>;
	using line = std::string;

	void checkDir();
	fpath createHTML(fpath);
	line beginHtml();
	line EndHtml();
	line titleHtml(line);
	line headHtmlEnd();
	line preHtml();
	line preCloseHtml();
	line writeDepHtml(line);
	line findMarkov(line);
	line addSingleLineCommentDiv(line);
	line addMultiLineCommentDiv(line);
	line addDiv(parserData, int, line);
	line createButton(line,int);
	
private:
	std::string dirName_, preClose_, pre_,fileFinal_,hrefName_;
	fpath files_,htmlFiles_, depfiles_,includes_;
};
converter::converter()
{
	dirName_ = "../ConvertedPages";
	hrefName_ = ".";

}
converter::~converter()
{
}

void converter::checkDir()
{
	if (!FileSystem::Directory::exists(dirName_))
	{
		FileSystem::Directory::create(dirName_);
	}
}

converter::fpath converter::createHTML(fpath files)
{
	parserData parsedData;
	findDependency Depobj;
	parserTable pt;
	//std::cout << "\n\n Requirement 4 and 6: This is converter package. Converter supports translating the markup characters into corresponding escape sequence";
	std::cout << "\nConverting ";

	for (auto file : files)
	{
		std::cout << ".";
		int lineCnt = 0;
		Depobj.ExecuteParser(file);
		parsedData = pt.getParserTable();

		std::string fname = FileSystem::Path::getName(file);

		std::ifstream readptr(file);
		std::ofstream writeptr(dirName_ + "/" + fname + ".html");
		std::string data = beginHtml() + titleHtml(fname) + headHtmlEnd() + writeDepHtml(file) + createButton("comments", 1) + preHtml();
		writeptr << data;

		for (std::string line; std::getline(readptr, line); )
		{
			lineCnt++;
			line = findMarkov(line);
			line = addDiv(parsedData, lineCnt, line);
			auto line2 = addSingleLineCommentDiv(line);
			if (!line2.empty())
				writeptr << line2;

			else
			{
				line = addMultiLineCommentDiv(line);
				writeptr << "\n";
				writeptr << line;
			}
		}
		writeptr << preCloseHtml();
		writeptr << EndHtml();
		std::string fileName = FileSystem::Path::getFullFileSpec(dirName_ + "/" + fname + ".html");
		//htmlFiles_.push_back(dirName_ + "/" + fname + ".html");
		htmlFiles_.push_back(fileName);
		readptr.close();
		writeptr.close();
	}
	return(htmlFiles_);
}

std::string converter::beginHtml()
{
	std::string str;
	std::ifstream htmlSt("../Converter/htmlStart.txt");
	for (std::string line; std::getline(htmlSt, line);)
		str.append(line + "\n");
	htmlSt.close();
	return str;
}

std::string converter::EndHtml()
{
	std::string str;
	std::ifstream htmlEd("../Converter/htmlEnd.txt");
	for (std::string line; std::getline(htmlEd, line);)
		str.append(line + "\n");
	htmlEd.close();
	return str;
}

std::string converter::titleHtml(line fname)
{
	std::string title = "<title> " + fname + "</title>";
	return title;
}

std::string converter::headHtmlEnd()
{
	std::string str;
	std::ifstream htmlHeadEd("../Converter/headHtmlEnd.txt");
	for (std::string line; std::getline(htmlHeadEd, line);)
		str.append(line + "\n");
	htmlHeadEd.close();
	return str;
}

std::string converter::preHtml()
{
	return ("\n <pre>\n");
}

std::string converter::preCloseHtml()
{
	return ("\n </pre>\n");
}

std::string converter::writeDepHtml(line file)
{
	dependency Depobj;
	std::string fname = FileSystem::Path::getName(file);
	auto dependencies = Depobj.dependencies(fname);
	std::string str = "";
	if (dependencies.size() == 0)
		str = " <h3> File Name: " + fname + " </h3> <h3> No dependencies found \n";
	else
	{
		str.append("\t <h3> File Name: " + fname + " </h3> <h3> Dependencies: ");
		for (auto iter : dependencies)
		{
			str.append("<a href =\"" + hrefName_ + "/" + iter + ".html\">" + iter + "</a> &emsp; \n");
		}
	}
	str.append("</h3>");
	return str;
}

std::string converter::findMarkov(line line)
{
	size_t offset = line.find('<');
	while (offset != std::string::npos)
	{
		line.replace(offset, 1, "&lt");
		offset = line.find('<', offset + 1);
	}
	size_t offset1 = line.find('>');

	while (offset1 != std::string::npos)
	{
		line.replace(offset1, 1, "&gt");
		offset1 = line.find('>', offset1 + 1);
	}
	return line;
}

std::string converter::addSingleLineCommentDiv(line line)
{
	//std::string btn = createButton("comments", 1);
	size_t pos = line.find('/');
	if (line[pos + 1] == '/' && pos != std::string::npos)
	{
		//std::cout << "line: " << cnt++ << " this is comment line: " << line << "\n";
		line.replace(pos, 0, "<div class=\"comments\">");
		line += "</div>";
		return line;
	}
	return "";
}

std::string converter::addMultiLineCommentDiv(line line)
{
	size_t pos = line.find('/');
	size_t pos1 = line.find('*');
	size_t pos2 = line.rfind('*');

	if (line[pos + 1] == '*' && line[pos2 + 1] == '/' &&pos != std::string::npos) {
		line.replace(pos, 0, "<div class = \"comments\">");
		line.replace(pos + 24, 0, "</div>");
		return line;
	}

	else if (line[pos + 1] == '*'  &&pos != std::string::npos) {
		line.replace(pos, 0, "<div class = \"comments\">");
		return line;
	}
	else if (line[pos1 + 1] == '/'  &&pos != std::string::npos) {
		line.replace(pos1 + 2, 0, "</div>");
		return line;
	}
	else
	{
		return line;
	}
}

std::string converter::addDiv(converter::parserData parserTable, int lineNo, line line)
{
	auto Key1 = parserTable.find(lineNo);
	if (Key1 != parserTable.end()) {
		if (lineNo == Key1->second.second)
			return (line);
	}
	std::string cnt1 = std::to_string(lineNo);
	auto Key = parserTable.find(lineNo);
	if (Key != parserTable.end()) {
		std::string str = "<div class=\"" + Key->second.first + cnt1 + "\">";
		line.replace(0, 0, str);
		std::string str1 = createButton(Key->second.first, lineNo);
		line = str1 + line;
		return (line);
	}
	parserData::iterator it = parserTable.begin();
	while (it != parserTable.end()) {
		if (it->second.second == lineNo) {
			return (line + "</div>");
		}
		it++;
	}
	return line;
}

std::string converter::createButton(line name, int cnt)
{
	if (name == "comments")
		return ("\n <button type = \"button\" onclick = \"toggleDisplay('" + name + "')\" id = \"" + name + "\"> Hide " + name + "</button>");
	else
	{
		std::string cnt1 = std::to_string(cnt);
		return ("\n <button type = \"button\" onclick = \"toggleDisplay('" + name + cnt1 + "')\" id = \"" + name + cnt1 + "\"> - </button>");
	}
}


