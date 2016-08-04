#ifndef METRICEXECUTIVE_H
#define METRICEXECUTIVE_H
//////////////////////////////////////////////////////////////////////
//  MetricExecutive.h - Test Suite to demostrate all requirements   //
//  ver 1.1                                                         //
//  Language:     C++, Visual Studio 2105                           //
//  Application:  Code Parser with Abstract Syntax Tree		        //
//				  CSE687 - Object Oriented Design                   //
//  Author:      Varun Jindal - vjindal@syr.edu                     //
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This package contains a test suite that demonstrates all the requirements
of project 2. The matric analyzer and file manager is called in this package.

Public Interface functions:
  Title(std::string str);
  Req1()
  Req2()
  Req3()
  Req4()
  Req5(std::vector<std::string>)
  Req7(std::vector<std::string>)
  Req9(std::vector<std::string>)
  Req10()

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp,
MetricAnalyzer.h, MetricAnalyzer.cpp, ASTree.h, ASTree.cpp, FileManager.h, FileManager.cpp
Build commands (either one)
- devenv MetricExecutive.sln

Maintenance History:
====================
ver 1.0 : 14 Mar 16
- first release, demonstrated 1,2,3 and 4 requirement
ver 1.1 : 17 Mar 16
- demonstrated 5,6,7,8,9,10 requirement


*/
//
#include <iosfwd>
#include <string>
using namespace std;
class MetricExecutive
{
public:
	static void Title(std::string str);
	static void Req1();
	static void Req2();
	static void Req3();
	static void Req4();
	static int Req5(std::vector<std::string>);	
	static void Req7(std::vector<std::string>);
	static void Req9(std::vector<std::string>);
	static void Req10();
};
#endif