#ifndef METRICANALYZER_H
#define METRICANALYZER_H
//////////////////////////////////////////////////////////////////////
//  MatricAnalyzer.h - Computes complexity after analyzing the file //
//  ver 1.0                                                         //
//  Language:     C++, Visual Studio 2105                           //
//  Application:  Code Parser with Abstract Syntax Tree		        //
//				  CSE687 - Object Oriented Design                   //
//  Author:       Varun Jindal - vjindal@syr.edu                    //
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This package computes complexity after analyzing the file

Public Functions:
 getComplexity(element* node)
 displayComplexity(element* node)
 setRoot(element* root)
 getRoot()

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp, AST_tree.h,
FileManager.h, FileManager.cpp
Build commands
- devenv MetricAnalyzer.sln

Maintenance History:
====================
ver 1.0 : 17 Mar 16 - first release

*/
//
#include <iostream>
#include <iosfwd>
#include <string>
#include "../ASTree/AST_node.h"

class MetricAnalyzer
{
public:
	void getComplexity(element* node);
	void displayComplexity(element* node);
	void setRoot(element* root);
	element* getRoot();	
private:
	element* node;
};

#endif
