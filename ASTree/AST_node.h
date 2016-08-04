#ifndef ASTNODE_H
#define ASTNODE_H

/////////////////////////////////////////////////////////////////////
//  AST_node.h  - Defines node for AST - Abstract Syntax tree      //
//  ver 1.0                                                        //
//  Language:     C++, Visual Studio 2105                          //
//  Application:  Code Parser with Abstract Syntax Tree		       //
//				  CSE687 - Object Oriented Design                  //
//  Author:       Varun Jindal - vjindal@syr.edu                   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Defines node for abstract syntax tree to be used in Parser
*
*	Build Process:
*	==============
*	Build commands (either one)
*	- devenv AST_node.sln /rebuild debug
*	 Maintenance History:
*	--------------------
*	ver 1.0 : 17 March 2016
*	 - first release
*
*/
#include<vector>
#include <sstream>

//Defining the structure for node
struct element
{
	std::string type;  
	std::string name;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << startLineCount;
		temp << ", ";
		temp << endLineCount;
		temp << ")";
		return temp.str();
	}

	size_t startLineCount = 0;
	size_t endLineCount = 0;
	size_t complexity = -1;
	std::vector<element*> child;
};
#endif
