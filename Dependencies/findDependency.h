#pragma once
/////////////////////////////////////////////////////////////////////////
// findDependency.h - Supports finding the dependency for a file       //
//                     It also supports parsing of the file to find    //
//                    the starting and endling line of each function   //
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 1 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////
/*
 *
 * - findDep(fpath files)                  For a given vector of files find all dependencies for each file
 * - checkDupliactes(fpath, fpath)         Check if the file is going to get converted into html, if not convert it
 * - ExecuteParser(fname)                  Create a parser table (unordered_map(int,pair)) that stores member type, start line and end line of the member
 * - myTreeWalk(CodeAnalysis::ASTNode *)   recursiverly parse the given file to find the type of members in the file
 *
 * Build Process:
 * --------------
 * Required Files:
 *	findDependency.h
 *  parser.h
 *  Utilities.h
 *	toker.h
 *  Semi.h
 *  ActionsAndRules.h
 *  ConfigureParser.h 
 *  dependency.h
 *  FileSystem.h
 *  parserTable.h
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

#include "../Parser/Parser.h"
#include "../Utilities/Utilities.h"
#define Util StringHelper
#include<utility>
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../dependencyTable/dependency.h"
#include "../FileSystem/FileSystem.h"
#include <unordered_map>
#include "../ParserTable/parserTable.h"

class findDependency
{
public:
	using fpath = std::vector<std::string>;
	using pair = std::pair<std::string, int>;
	using parserData = std::unordered_map<int, pair>;
	using fname = std::string;
	findDependency();
	~findDependency();
	fpath findDep(fpath);
	fpath checkDupliactes(fpath, fpath);
	void ExecuteParser(fname);
	void myTreeWalk(CodeAnalysis::ASTNode *);

private:
	fpath includes_, tempVec_;
};


findDependency::findDependency()
{
}

findDependency::~findDependency()
{
}

findDependency::fpath findDependency::findDep(fpath files)
{
	//std::cout << "\n Requirement 4: This is the dependency package. \n Requirement 7: This package supports insertion, in each analyzed file, links to all the files in the directory tree, on which it depends.";
	dependency DT;
	tempVec_ = files;
	while (!files.empty())
	{
		std::string file = files.back();
		files.pop_back();
		std::string name = FileSystem::Path::getName(file);
		includes_.clear();
		if (DT.isExistsDependency(name))
			continue;

		std::string fileSpec = FileSystem::Path::getFullFileSpec(file);
		std::string msg = "Processing file- " + fileSpec;
		Util::title(msg);
		CodeAnalysis::ConfigParseForCodeAnal configure;
		CodeAnalysis::Parser* pParser = configure.Build();
		configure.Attach(fileSpec);
		CodeAnalysis::Repository* pRepo = CodeAnalysis::Repository::getInstance();
		pRepo->package() = name;

		while (pParser->next())
			pParser->parse();

		CodeAnalysis::ASTNode* pGlobalScope = pRepo->getGlobalScope();
		complexityEval(pGlobalScope);
		auto iterator = pGlobalScope->statements_.begin();
		while (iterator != pGlobalScope->statements_.end()) {
			includes_.push_back((*iterator)->ToString());
			++iterator;
		}
		fpath depVec;
		depVec.clear();
		auto depVector = DT.findDependency(file, includes_);
		for (auto temp : depVector)
			depVec.push_back(FileSystem::Path::getName(temp));

		DT.addDependency(file, depVec);
		tempVec_ = checkDupliactes(tempVec_, depVector);
		for (auto i : depVector)
			files.push_back(i);
	}
	return tempVec_;

}
findDependency::fpath findDependency::checkDupliactes(fpath tempVec, fpath depVector)
{
	int flag = 0;
	for (auto element : depVector) {
		flag = 0;
		for (auto elem : tempVec)
		{
			if (FileSystem::Path::getName(element) == FileSystem::Path::getName(elem)) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			tempVec.push_back(element);
		}
	}
	return tempVec;

}

void findDependency::ExecuteParser(fname fileName)
{
	parserTable pt;
	std::string fileSpec = FileSystem::Path::getFullFileSpec(fileName);
	std::string name = FileSystem::Path::getName(fileName);

	CodeAnalysis::ConfigParseForCodeAnal configure;
	CodeAnalysis::Parser* pParser = configure.Build();
	configure.Attach(fileSpec);

	CodeAnalysis::Repository* pRepo = CodeAnalysis::Repository::getInstance();
	pRepo->package() = name;

	while (pParser->next())
	{
		pParser->parse();
	}
	CodeAnalysis::ASTNode* pGlobalScope = pRepo->getGlobalScope();
	complexityEval(pGlobalScope);

	auto iterator = pGlobalScope->statements_.begin();
	pt.clearparsedData();
	myTreeWalk(pGlobalScope);
}

void findDependency::myTreeWalk(CodeAnalysis::ASTNode* pGlobalScope)
{
	parserTable pt;
	auto iter = pGlobalScope->children_.begin();
	//std::cout << "inside data is:" << pGlobalScope->type_ << " " << pGlobalScope->startLineCount_ << " " << pGlobalScope->endLineCount_ << "\n";
	//std::cout << "\n pair is: " << pair.first << " " << pair.second;
	if (!(pGlobalScope->type_ == "control" || pGlobalScope->type_ == "anonymous"))
		pt.addEntry(pGlobalScope->startLineCount_, pGlobalScope->type_, pGlobalScope->endLineCount_);

	while (iter != pGlobalScope->children_.end())
	{
		myTreeWalk(*iter);
		++iter;
	}
}