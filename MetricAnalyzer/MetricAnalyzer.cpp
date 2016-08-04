//////////////////////////////////////////////////////////////////////////////////
//  MetricAnalyzer.cpp - Computes complexity after analyzing the file           //
//  ver 1.0                                                                     //
//  Language:          C++, Visual Studio 2105							        //
//  Application:       Code Parser with Abstract Syntax Tree					//
//				       CSE687 - Object Oriented Design						    //
//  Author:            Varun Jindal - vjindal@syr.edu		     		        //
//								                                                //
//////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "MetricAnalyzer.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../FileManager/FileManager.h"

using namespace Scanner;
using namespace Utilities;


//----< return the root >--------
element* MetricAnalyzer::getRoot() {
	return node;
}

//----< set the root on which analysis is to be performed >------
void MetricAnalyzer ::setRoot(element* root) {
	node = root;
}


//----< Compute complexity of the node in the AST >------
void MetricAnalyzer::getComplexity(element* node) {
	static size_t complexity = 0;
	node->complexity = complexity;
	auto iterator = node->child.begin();
	++complexity;
	while (iterator != node->child.end())
	{
		getComplexity(*iterator);
		++iterator;
	}
	node->complexity = complexity - node->complexity;
}

//----< Prints the line count and complexity of a function >-------
void MetricAnalyzer::displayComplexity(element* node) {
	
	if (node->type == "function") {
		std::cout << "\n   " << node->name;
		size_t n = 34 - node->name.length();
		for (int i = 0; i < n; i++)
		{
			std::cout << "-";
		}
		std::cout << "    ";
		std::cout << node->endLineCount - node->startLineCount;
		int a;
		if ((node->endLineCount - node->startLineCount) < 10)
			a = 0;
		else if ((node->endLineCount - node->startLineCount) < 100)
			a = 1;
		else if ((node->endLineCount - node->startLineCount) < 1000)
			a = 2;
		std::cout << "  ";
		for (a;a < 10; a++)
			std::cout << "-";
		std::cout << "  ";
		std::cout <<  node->complexity;
		 if (node->complexity < 10)
			std::cout << "   " ;
		else
			std::cout << "  " ;
	}
	auto iter = node->child.begin();
	while (iter != node->child.end())
	{
		displayComplexity(*iter);
		++iter;
	}
}
	


//Test Stub
#ifdef TEST_METRICANALYZER

#include <queue>
#include <string>
#define Util StringHelper

int main(int argc, char* argv[])
{
	//collecting tokens from files, named on the command line
	if (argc < 2)
	{
		std::cout<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	FileManager fileManager;
	std::vector<std::string> patterns = { "*.h" , "*.cpp" };
	std::vector<std::string> files = fileManager.getFiles("..//", patterns);
	for (int i = 1; i<argc; ++i)
	{
		std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
		std::string msg = "Processing file" + fileSpec;		
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		MetricAnalyzer *test = new MetricAnalyzer;
		try		{
			if (pParser)			{
				if (!configure.Attach(argv[i]))
				{
					std::cout << "\n  could not open file " << fileSpec << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			while (pParser->next())
			{
				pParser->parse();

			}
			std::cout << "\n";
			configure.printTree();
			test->setRoot(configure.getRoot());
			test->getComplexity(test->getRoot());
			test->getComplexity(test->getRoot());
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
}

#endif