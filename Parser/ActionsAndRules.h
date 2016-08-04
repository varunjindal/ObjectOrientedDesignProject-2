#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
//////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions      //
//  ver 1.2                                                         //
//  Language:     C++, Visual Studio 2105                           //
//  Application:  Code Parser with Abstract Syntax Tree		        //
//				  CSE687 - Object Oriented Design                   //
//  Author:      Varun Jindal - vjindal@syr.edu                     //
//////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 1.0 : 14 Mar 16
  - first release
  ver 1.1 : 15 Mar 16
  - added Abstract Syntax Tree 
  ver 1.2 : 16 Mar 16
  - added lambda function

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../ASTree/AST_tree.h"
///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
class Repository  // application specific
{
  ScopeStack<element*> stack;
  Scanner::Toker* p_Toker;
  
public:
	AST *asTree = new AST();
//----< Repository Constructor >------
  Repository(Scanner::Toker* pToker)
  {
    p_Toker = pToker;
  }
  //----< This function returns Stack of Elements >------
  ScopeStack<element*>& scopeStack()
  {
    return stack;
  }
  //-----< This funtion returns Toker pointer >------
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  //----< This funtion returns Line Count >------
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }

};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	//----< Tests for Beginning of the Scope >------
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
	//----< HandlePush Class Constructor >------
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  //----< This function returns boolean for Special Key word >------ 
  bool isSpecialKeyWord(const std::string& tok)
  {
	  const static std::string keys[]
		  = { "for", "while", "switch", "if", "catch" };
	  for (int i = 0; i<5; ++i)
		  if (tok == keys[i])
			  return true;
	  return false;
  }
  //----< Does Action for the rule for pushing the element into the stack >------
  void doAction(ITokCollection*& pTc)
  {
	  if (p_Repos->asTree->getRoot() == nullptr)
	  {
		  element *_elem = new element;
		  _elem->type = "GNS";
		  _elem->name = "anonymous";
		  _elem->startLineCount = 0;
		  p_Repos->asTree->setRoot(_elem);
		  p_Repos->scopeStack().push(_elem);
	  }
	  ITokCollection& tc = *pTc;
    element *elem = new element;
	size_t len = tc.find("(") - 1;
	if (isSpecialKeyWord(tc[len]))
	{
		elem->type = tc[len];
	}
	else if (tc.length() > 3 && tc[tc.length() - 3] == "try")
	{
		elem->type = "try";
		elem->name = "Try Block";
		elem->startLineCount = p_Repos->lineCount();
	}
	else if (tc.length() > 3 &&  tc[tc.length() - 3] == "else")
	{
		elem->type = "else";
		elem->name = "Else Block";
		elem->startLineCount = p_Repos->lineCount();
	}
	else
	{
		elem->type = "unknown";
		elem->name = "anonymous";
		elem->startLineCount = p_Repos->lineCount();
	}
	element  * elem2 = p_Repos->scopeStack().top();
	elem2->child.push_back(elem);
	p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	//----< tests end of the scope >------
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
	//----< HandlePop class Constructor >------
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  //----< Handles action for popping element from stack >------
  void doAction(ITokCollection*& pTc)
  {
    if(p_Repos->scopeStack().size() == 0)
      return;
    element *elem = p_Repos->scopeStack().pop();
	elem->endLineCount = p_Repos->lineCount() + 1;    
	if (p_Repos->scopeStack().size() == 1)
	{
		p_Repos->scopeStack().top()->endLineCount = elem->endLineCount;
	}
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	//----< tests if the statement is a preprocessor >------
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

//----< Class to print preprocessor statement >------

class PrintPreproc : public IAction
{
public:
	//----< action to print preprocessor statement >------
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preprocessor Statement: " << pTc->show().c_str();
  }
};




///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
	//Checks for special keyword
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  //Tests for Function Definition
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {

      size_t len = tc.find("(");
	  size_t len2 = tc.findEnd("]");
	  bool isLambda = (len2 < tc.length() && tc[len2 + 1] == "(") || tc[tc.length() - 2] == "]";
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]) && !isLambda)
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
	//Constructor
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  //pushes function on to the stack
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();
	element *elem2 =	p_Repos->scopeStack().top();
	elem2->child.pop_back();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element *elem = new element;
    elem->type = "function";
    elem->name = name;
    elem->startLineCount = p_Repos->lineCount();
	elem2 = p_Repos->scopeStack().top();
	elem2->child.push_back(elem);
    p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
	//Constructor
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  //Prints SemiExpression
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	//Prints function definition to the console
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    size_t len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

//----< class to capture Lambda >------

class LambdaDefinition : public IRule
{
public:
	//----< function to test lambda >------
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.findEnd("]");
			if ((len < tc.length() && tc[len + 1] == "(") || tc[tc.length() - 2] == "]")
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

//----< class to push Lambda on to the stack >------

class PushLambda : public IAction
{
	Repository* p_Repos;
public:
	//----< PushLambda Constructor >------
	PushLambda(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	//----< function action to push lambda to the stack >------
	void doAction(ITokCollection*& pTc)
	{
		//----< pop anonymous scope from stack >------
		p_Repos->scopeStack().pop();
		element *elem2 = p_Repos->scopeStack().top();
		elem2->child.pop_back();

		//----< push Lambda scope on the stack >------
		element *elem = new element;
		elem->type = "lambda";
		elem->startLineCount = p_Repos->lineCount();
		elem2 = p_Repos->scopeStack().top();
		elem2->child.push_back(elem);
		p_Repos->scopeStack().push(elem);
	}
};

//----< class to Print Lambda on to the console >------

class PrintLambda : public IAction
{
	Repository* p_Repos;
public:
	//----< PrintLambda constructor >------
	PrintLambda(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	//----< action to print lambda >------
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t len = tc.findEnd("[");
		std::cout << "\n  Executable: \n  ";
		for (size_t i = 0; i < len - 2; i++)
			if (tc[i] != "\n")
				std::cout << tc[i];
		std::cout << "\n";
		std::cout << "\n  Lambda: \n  ";
		for (size_t i = len - 1; i < tc.length(); i++)
			if (tc[i] != "\n")
				std::cout << tc[i];
		std::cout << "\n";
	}
};

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  //Handling Templates
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
  }
  //Checks if token is special keyword
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  //Removes invocation Paranthesis
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }

  //Check for Declaration
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);
      // remove modifiers, comments, newlines, returns, and initializers
      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]))
          continue;
        if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
	//Shows Declaraion
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
    std::cout << "\n  Declaration: " << se.show();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  //Handles Templates
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
  }
  
//Checks for special  Keyword
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  //Removes Invocation Paranthesis
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }
  //Checks for Declaration
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);
      // remove modifiers, comments, newlines, returns, and initializers
      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]))
          continue;
        if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
	//Shows Executable
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    std::cout << "\n  Executable: " << se.show();
  }
};

#endif
