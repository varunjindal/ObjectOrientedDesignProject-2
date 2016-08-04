//////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers             //
//  ver 1.0                                                         //
//  Language:     C++, Visual Studio 2105                           //
//  Application:  Code Parser with Abstract Syntax Tree		        //
//				  CSE687 - Object Oriented Design                   //
//  Author:      Varun Jindal - vjindal@syr.edu                     //
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pLambdaDefinition;
  delete pPushLambda;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  pIn->close();
  delete pIn;

}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}
//----< Sets the Print Variable >------------

void ConfigParseToConsole::setDisplayFilter(bool temp)
{
	discardDisplay = temp;
}
//----< Here's where all the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try  {
    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);    
	pBeginningOfScope = new BeginningOfScope();// configure to manage scopec-- these must come first - they return true on match-- so rule checking continues
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);	
	pFunctionDefinition = new FunctionDefinition;// configure to detect and act on function definitions -- these will stop further rule checking by returning false
    pPushFunction = new PushFunction(pRepo);  
	pPrintFunction = new PrintFunction(pRepo);
    pFunctionDefinition->addAction(pPushFunction);
	if (!discardDisplay)
		pFunctionDefinition->addAction(pPrintFunction);
    pParser->addRule(pFunctionDefinition);
	pLambdaDefinition = new LambdaDefinition;
	pPushLambda = new PushLambda(pRepo);
	pPrintLambda = new PrintLambda(pRepo);
	pLambdaDefinition->addAction(pPushLambda);
	if (!discardDisplay)
		pLambdaDefinition->addAction(pPrintLambda);
	pParser->addRule(pLambdaDefinition);   
    pDeclaration = new Declaration; // configure to detect and act on declarations and Executables
    pShowDeclaration = new ShowDeclaration;
	if (!discardDisplay)
		pDeclaration->addAction(pShowDeclaration);
    pParser->addRule(pDeclaration);
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
	if (!discardDisplay)
		pExecutable->addAction(pShowExecutable);
    pParser->addRule(pExecutable);
    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}
//Calls Treewalk function of ASTree of the Repository
void ConfigParseToConsole::printTree()
{
	pRepo->asTree->TreeWalk(pRepo->asTree->getRoot());
}

//Returns root of the ASTree
element* ConfigParseToConsole::getRoot()
{
	return pRepo->asTree->getRoot();
}


#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>
//Test Stub
int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
