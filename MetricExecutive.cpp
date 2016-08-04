//////////////////////////////////////////////////////////////////////////////////
//  MetricExecutive.cpp - Demonstrates all the requirements                     //
//  ver 1.2                                                                     //
//  Language:          C++, Visual Studio 2105							        //
//  Application:       Code Parser with Abstract Syntax Tree					//
//				       CSE687 - Object Oriented Design						    //
//  Source:            Jim Fawcett, Syracuse University, CST 4-187			    //
//                     jfawcett@twcny.rr.com									//
//  Author:            Varun Jindal - vjindal@syr.edu                           //
//////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../MetricAnalyzer/MetricAnalyzer.h"
#include "../FileManager/FileManager.h"
#include "MetricExecutive.h"

using namespace Scanner;
using namespace Utilities;
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"



#ifdef TEST_METRICEXECUTIVE

#include <queue>
#include <string>
#define Util StringHelper

void MetricExecutive::Title(std::string str)
{
	std::cout << std::endl << "================================================================================" << std::endl;
	std::cout << str << std::endl;
	std::cout << "================================================================================" << std::endl;
}
void MetricExecutive::Req1()
{
	std::cout << "This project is developed using Visual Studio 2015" << std::endl;
	std::cout << std::endl;
}

void MetricExecutive::Req2()
{
	std::cout << "C++ standard library's is used for streams for all I/O and new and delete for all heap-based memory management" << std::endl;
	std::cout << std::endl;
}

void MetricExecutive::Req3()
{
	std::cout << "Project provides Matrix Analyser Package for analyzing function size and complexity metrics for a set of specified packages. Corrosponding results are displayed in later requirements" << std::endl;
	std::cout << std::endl;
}

void MetricExecutive::Req4()
{
	std::cout << "Project provides a Parser package with a Parser class that is a container for Rules and that provides the interfaces IRule and IAction for rules contained in the Parser and actions contained in each rule." << std::endl;
	std::cout << std::endl;
}

void MetricExecutive::Req5(std::vector<std::string> files1)
{
	std::cout << "Project provides a an associated RulesAndActions package that has rules to detect:" << std::endl;
	std::cout << "		" << "- global functions and static and non-static member function definitions" << std::endl;
	std::cout << "		" << "- beginning and end of all C++ scopes" << std::endl;
	std::cout << "		" << "- actions for each rule that support building the Abstract Syntax Tree (AST) that represents a single file's scope structure and identifies the type and line number extend of each scope." << std::endl;
	std::cout << std::endl;
	vector<string> files = files1;	
	std::cout << "The scopes, functions, Decalrations and Executables are handled and displayed below:\n";
	std::string fileSpec = "MetricExecutive.cpp";
	std::string msg = "Processing file  " + fileSpec;	
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	MetricAnalyzer *mAnalyzer = new MetricAnalyzer;
	try	{		
		if (pParser)// the parser is build here
		{
			if (!configure.Attach(fileSpec))
				std::cout << "\n  could not open file " << fileSpec << std::endl;
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}		
		while (pParser->next())// the parser is used here
		{
			pParser->parse();
		}
		MetricExecutive::Title("Demonstrating Requirement 6");
		std::cout << "The abstract syntax tree structure that provides an interface built for the above procesed file is as follows:\n";
		configure.printTree();		
		MetricExecutive::Req7(files);
		MetricExecutive::Title("Demonstrating Req 8");
		std::string msg = "Processing file  " + fileSpec;
		Util::title(msg);
		std::cout << std::endl;
		mAnalyzer->setRoot(configure.getRoot());
		mAnalyzer->getComplexity(mAnalyzer->getRoot());
		std::cout << "Function \t\t\t" << "       " << "Line Count" << "    " << "Complexity";
		std::cout << "\n ***********************************************************";
		mAnalyzer->displayComplexity(mAnalyzer->getRoot());
		std::cout << "\n ***********************************************************";
	}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
}

void MetricExecutive::Req7(std::vector<std::string> files)
{
	MetricExecutive::Title("Demonstrating Req 7");
	std::cout << "\n Searching for the pattern on the directory: " << FileSystem::Path::getFullFileSpec("..//") << std::endl;
	std::cout << std::endl << "File names : " << std::endl;
	for (int i = 0; i < files.size(); ++i)
	{
		std::cout << "\n" << files[i];
	}
}

void MetricExecutive::Req9(std::vector<std::string> files)
{
	for (int i = 0; i < files.size(); ++i)
	{
		std::string fileSpec = FileSystem::Path::getFullFileSpec(files[i]);
		std::cout << std::endl;
		std::string msg = "Processing file  " + fileSpec;
		Util::title(msg);
		std::cout << std::endl;
		ConfigParseToConsole configure;
		configure.setDisplayFilter(true);
		Parser* pParser = configure.Build();

		MetricAnalyzer *mAnal = new MetricAnalyzer;
		try
		{
			if (pParser)
			{
				if (!configure.Attach(files[i]))
				{
					std::cout << "\n  could not open file " << fileSpec << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return;
			}
			while (pParser->next())
			{
				pParser->parse();

			}
			mAnal->setRoot(configure.getRoot());
			mAnal->getComplexity(mAnal->getRoot());
			std::cout << "Function \t\t\t" << "       " << "Line Count" << "    " << "Complexity";
			std::cout << "\n ***********************************************************";
			mAnal->displayComplexity(mAnal->getRoot());
			std::cout << "\n ***********************************************************";

		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}

}

void MetricExecutive::Req10()
{
	std::cout << "This is an automated unit test suite that exercises all of the packages provided in my submission and demonstrates all the requirements" << std::endl;
	std::cout << std::endl;
}


int main(int argc, char* argv[])
{
	MetricExecutive::Title("Demonstrating Req 1");
	MetricExecutive::Req1();
	MetricExecutive::Title("Demonstrating Req 2");
	MetricExecutive::Req2();
	MetricExecutive::Title("Demonstrating Req 3");
	MetricExecutive::Req3();
	MetricExecutive::Title("Demonstrating Req 4");
	MetricExecutive::Req4();
	if (argc < 3 || argv[1] == "" ||argv[2] == "")
	{
		// tokens are collected from the files passed on the commandline 
		std::cout
			<< "\n  please enter file patterns and root directory on command line\n\n";
		return 1;
	}	
	FileManager fileManager;
	vector<string> patterns;
	patterns.push_back(argv[1]);
	vector<string> files = fileManager.getFiles(argv[2], patterns);
	MetricExecutive::Title("Demonstrating Req 5");
	MetricExecutive::Req5(files);	
	MetricExecutive::Title("Demonstrating Req 9");
	MetricExecutive::Req9(files);
	MetricExecutive::Title("Demonstrating Req 10");
	MetricExecutive::Req10();
}

#endif