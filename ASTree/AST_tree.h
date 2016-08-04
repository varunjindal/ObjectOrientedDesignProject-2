#ifndef ASTree_H
#define ASTree_H
/////////////////////////////////////////////////////////////////////////////
//  AST_tree.h - Constructs Abstax Syntax Tree							   //
//  ver 1.0                                                                //
//  Language:     C++, Visual Studio 2105								   //
//  Application:  Code Parser with Abstract Syntax Tree					   //
//				  CSE687 - Object Oriented Design						   //
//  Source:       Jim Fawcett, Syracuse University, CST 4-187			   //
//                jfawcett@twcny.rr.com									   //
//  Author:       Varun Jindal - vjindal@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package provides abstract syntax tree for the parser
*
* Public Interface:
* =================
* ScopeStack<element> stack;
* // define some element type
* element elem;
* stack.push(elem);
* element popped = stack.pop();
*
* Required Files:
* ===============
* AST_tree.h, AST_node.h
*
* Build Command:
* ==============
* devenv AST_tree.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 13 Mar 16
* - first release
*/
#include <list>

// element is a application specific type.  It must provide a function:
// std::string show() that returns a string display of its parts. see
// test stub for an example.

#include "AST_node.h"
#include<iostream>
class AST
{
public:
	element* getRoot() { return root; }
	void setRoot(element  *elem) { root = elem; }

	element *root = nullptr;

//------- < This function traverses the tree >-----------

	void TreeWalk(element* pItem)
	{
		static size_t indentLevel = 0;
		std::cout << "\n  " << std::string(2 * indentLevel, ' ') << pItem->show();
		auto iter = pItem->child.begin();
		++indentLevel;
		while (iter != pItem->child.end())
		{
			TreeWalk(*iter);
			++iter;
		}
		--indentLevel;
	}
};
#endif