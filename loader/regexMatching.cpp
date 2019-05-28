/////////////////////////////////////////////////////////////////////////
// regexMatching.cpp - Checks for a vaild regex pattern given          //
//					   on command line and matches files to a          // 
//					   regular expression and returns the matched files//
// ver 1.0                                                             //
// Language:    C++, Visual Studio 2017                                //
// Application: Project 1 Spring 2019, CSE687 - Object Oriented Design //
// Author:      Omkar Buchade, Student, CSE 687                        //
//              opbuchad@syr.edu                                       //
/////////////////////////////////////////////////////////////////////////

#include "regexMatching.h"

regexMatching::regexMatching()
{
}

regexMatching::~regexMatching()
{
}


bool regexMatching::checkParseError(int argc, char * argv[])
{
	Utilities::ProcessCmdLine pcl(argc, argv);
	Utilities::preface("Command Line: ");
	pcl.showCmdLine();
	//Utilities::putline();
	if (pcl.parseError())
	{
		pcl.usage();
		return true;
	}
	else
		return false;
}

regexMatching::fpath regexMatching::ProcessFiles(int argc, char *argv[])
{
	Utilities::ProcessCmdLine pcl(argc, argv);

	FileSystem::DirExplorerN de(pcl.path());
	for (auto patt : pcl.patterns())
	{
		de.addPattern(patt);
	}

	if (pcl.hasOption('s'))
	{
		de.recurse();
	}

	de.search();
	auto files = de.GetFiles();	//GetFiles is a function to get the files
	std::cout << "\nFiles matched to given patterns:";
	for (auto temp : files)
		std::cout << "\n" << temp;
	de.showStats();
	return files;
}

regexMatching::fpath regexMatching::MatchFiles(int argc, char *argv[], regexMatching::fpath files)
{
	Utilities::ProcessCmdLine pcl(argc, argv);
	if (pcl.regexes().size() == 0)
	{
		std::cout << "\nNo regex found";
		return files;
	}
	int flag = 0;
	for (auto regex : pcl.regexes())
	{
		std::cout << "\n\nFiles matched to Regular Expression: " << regex << "\n";
		std::regex r(regex);
		for (auto file : files)
		{
			auto fname = FileSystem::Path::getName(file);
			if (std::regex_match(fname, r))
			{
				for (auto item : files_)
				{
					if (FileSystem::Path::getName(item) == file)
						flag = 1;
				}
				if (flag == 0)
				{
					files_.push_back(file);
					std::cout << file << "\n";
				}
				flag = 0;
			}
		}
	}
	std::cout << "\n";
	return files_;
}


#ifdef TestRegexMatching
int main(int argc, char *argv[])
{
	regexMatching RegexMatch;
	using fpath = std::vector<std::string>;
	fpath processedFiles, matchedfiles;

	if (!(RegexMatch.checkParseError(argc, argv)))
	{
		processedFiles = RegexMatch.ProcessFiles(argc, argv);
		matchedfiles = RegexMatch.MatchFiles(argc, argv, processedFiles);
	}
	else
	{
		std::cout << "parse error";
	}
	return 0;
}
#endif // TestRegexMatching
