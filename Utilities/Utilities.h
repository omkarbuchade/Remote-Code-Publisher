#ifndef UTILITIES_H
#define UTILITIES_H
///////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally useful, helper classes             //
// ver 1.4                                                           //
// Language:    C++, Visual Studio 2015                              //
// Platform:    Dell XPS 8900, Windows 10                            //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for 
* awhile to provide convenience functions for general C++ applications.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Utilities.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.4 : 26 Feb 2017
* - changed StringHelper::split to use isspace from <locale> instead of <cctype>
* ver 1.3 : 12 Aug 2016
* - added overloads of title functions for std::ostringstreams
* ver 1.2 : 10 Aug 2016
* - added new function StringHelper::sTitle(...)
* - changed the way string arguments are passed to each of the
*   title functions
* ver 1.1 : 06 Feb 2015
* - fixed bug in split which turns a comma separated string into
*   a vector of tokens.
* - added comments
* ver 1.0 : 05 Feb 2016
* - first release
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/
#include <string>
#include <functional>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>

namespace Utilities
{
	inline void Title(const std::string& text, std::ostream& out = std::cout, char underline = '=')
	{
		out << "\n  " << text;
		out << "\n " << std::string(text.size() + 2, underline);
	}
	//----< display underlined subtitle >--------------------------------

	inline void title(const std::string& text, std::ostream& out = std::cout, char underline = '-')
	{
		out << "\n  " << text;
		out << "\n " << std::string(text.size() + 2, underline);
	}

	inline void preface(const std::string& msg = "", bool doReturn = true, std::ostream& out = std::cout, const std::string& prefix = "  ")
	{
		if (doReturn) out << "\n" << prefix << msg;
	}

	inline std::string defaultUsage()
	{
		std::string usage;
		usage += "\n  Command Line: path [/option]* [/pattern]* [integer]";
		usage += "\n    path is relative or absolute path where processing begins";
		usage += "\n    [/option]* are one or more options of the form /s, /r, etc.";
		usage += "\n    [pattern]* are one or more pattern strings used for matching";
		usage += "\n    [integer] is the maximum number of items to process";
		usage += "\n";
		return usage;
	}
	class ProcessCmdLine
	{
	public:
		using Usage = std::string;
		using Path = std::string;
		using Option = int;
		using Options = std::vector<Option>;
		using Pattern = std::string;
		using Patterns = std::vector<Pattern>;
		using Regexes = std::vector<std::string>;
		using Number = long int;

		ProcessCmdLine(int argc, char** argv, std::ostream& out = std::cout);
		ProcessCmdLine(const ProcessCmdLine&) = delete;
		ProcessCmdLine& operator=(const ProcessCmdLine&) = delete;

		bool parseError();
		Path path();
		void path(const Path& path);
		Options options();
		void option(Option op);
		bool hasOption(Option op);
		Patterns patterns();
		Regexes regexes();
		void pattern(const Pattern& patt);
		Number maxItems();
		void maxItems(Number number);
		void usage(const Usage& usage);
		void usage();
		void showCmdLine(int argc, char** argv);
		void showCmdLine();
		void showPath();
		void showOptions();
		void showPatterns();
		void showRegexes();
		void showMaxItems();

	private:
		bool isValidRegex(const std::string& regex);
	private:
		Usage usage_;
		Path path_;
		Patterns patterns_;
		Regexes regexes_;
		Options options_;
		int maxItems_ = 0;
		bool parseError_ = false;
		std::ostream& out_;
	};

	/*----< path operations >------------------------------------------*/

	inline void ProcessCmdLine::path(const Path& path)
	{
		path_ = path;
	}

	inline ProcessCmdLine::Path ProcessCmdLine::path()
	{
		return path_;
	}

	inline void ProcessCmdLine::showPath()
	{
		out_ << path_ << " ";
	}

	/*----< options operations >---------------------------------------*/

	inline void ProcessCmdLine::option(Option option)
	{
		options_.push_back(option);
	}

	inline ProcessCmdLine::Options ProcessCmdLine::options()
	{
		return options_;
	}

	inline bool ProcessCmdLine::hasOption(Option op)
	{
		for (auto item : options_)
		{
			if (item == op)
			{
				return true;
			}
		}
		return false;
	}

	inline void ProcessCmdLine::showOptions()
	{
		for (auto opt : options_)
		{
			out_ << '/' << char(opt) << " ";
		}
	}

	/*----< patterns operations >--------------------------------------*/

	inline void ProcessCmdLine::pattern(const Pattern& pattern)
	{
		patterns_.push_back(pattern);
	}

	inline ProcessCmdLine::Patterns ProcessCmdLine::patterns()
	{
		return patterns_;
	}

	inline ProcessCmdLine::Regexes ProcessCmdLine::regexes()
	{
		return regexes_;
	}

	inline void ProcessCmdLine::showPatterns()
	{
		for (auto patt : patterns_)
		{
			out_ << patt << " ";
		}
	}

	inline void ProcessCmdLine::showRegexes()
	{
		for (auto regex : regexes_)
		{
			out_ << regex << " ";
		}
	}

	/*----< maxItems operations >--------------------------------------*/

	inline void ProcessCmdLine::maxItems(Number maxItems)
	{
		maxItems_ = maxItems;
	}

	inline ProcessCmdLine::Number ProcessCmdLine::maxItems()
	{
		return maxItems_;
	}

	inline void ProcessCmdLine::showMaxItems()
	{
		if (maxItems_ != 0)
			out_ << maxItems_ << " ";
	}

	inline bool ProcessCmdLine::isValidRegex(const std::string & regex)
	{
		bool res = true;
		try
		{
			std::regex tmp(regex);
		}
		catch (const std::regex_error& e)
		{
			(e);
			res = false;
		}
		return res;
	}

	/*----< parseError operation >-------------------------------------*/

	inline bool ProcessCmdLine::parseError()
	{
		return parseError_;
	}

	/*----< command line operations >----------------------------------*/

	inline ProcessCmdLine::ProcessCmdLine(int argc, char** argv, std::ostream& out) : out_(out)
	{
		//usage_ = defaultUsage();
		if (argc < 2)
		{
			out << "\n  command line parse error";
			//usage();
			parseError_ = true;
			return;
		}

		path_ = argv[1];

		for (int i = 2; i < argc; ++i)
		{
			if (argv[i][0] == '/')
			{
				if (strlen(argv[i]) > 2)
					continue;
				options_.push_back(argv[i][1]);
			}
			else
			{
				int number = atoi(argv[i]);
				if (number > 0)
				{
					maxItems_ = number;
				}
				else if (isValidRegex(argv[i]))
				{
					regexes_.push_back(argv[i]);
				}
				else
				{
					patterns_.push_back(argv[i]);
				}
			}
		}
	}

	/*inline void ProcessCmdLine::showCmdLine(int argc, char** argv)
	{
		if (argc > 2)
		{
			out_ << "  empty";
			return;
		}
		for (int i = 0; i < argc; ++i)
		{
			if (argv[i] == "0")
				continue;
			if (argv[i][0] == '/')
			{
				out_ << "/" + char(argv[i][1]) << " ";
			}
			else
			{
				out_ << argv[i] << " ";
			}
		}
	}*/

	inline void ProcessCmdLine::showCmdLine()
	{
		if (path_ == "" && options_.size() == 0 && patterns_.size() == 0)
		{
			out_ << "empty";
			return;
		}
		showPath();
		preface("", false);
		showOptions();
		preface("", false);
		showPatterns();
		preface("", false);
		showMaxItems();
	}

	inline void ProcessCmdLine::usage(const Usage& usage)
	{
		usage_ = usage;
	}

	inline void ProcessCmdLine::usage()
	{
		out_ << usage_;
	}

  class test
  {
  public:
    std::vector<std::string> publicVector;
  };

  class StringHelper
  {
  public:
    static std::vector<std::string> split(const std::string& src);
    static void Title(std::string src, std::ostream&  out = std::cout);
    static void Title(std::string src, std::ostringstream& out);
    static void title(std::string src, std::ostream& out = std::cout);
    static void title(std::string src, std::ostringstream& out);
    static void sTitle(
      std::string src, size_t offset, size_t width, std::ostream& out = std::cout, char underline = '-'
    );
    static void sTitle(
      std::string src, size_t offset, size_t width, std::ostringstream& out, char underline = '-'
    );
  };

  void putline();

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }

  template<typename T>
  T Converter<T>::toValue(const std::string& src)
  {
    std::istringstream in(src);
    T t;
    in >> t;
    return t;
  }
}
#endif
