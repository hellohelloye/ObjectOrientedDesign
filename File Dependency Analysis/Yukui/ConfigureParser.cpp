/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.0                                                        //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////

#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "classVector.h"
#include "relationVector.h"

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pFR;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
	  return false;
  return pToker->attach(name, isFile);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::firstParserBuild()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker();
    pToker->returnComments();
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);

    // add code folding rules

    pFR = new codeFoldingRules;
    pParser->addFoldingRules(pFR);

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);

	pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);  // no action
    pFunctionDefinition->addAction(pPushFunction);
    pParser->addRule(pFunctionDefinition);
	
	pClassDefinition = new ClassDefinition;
	pPushClassForVector = new PushClassForVector(pRepo,pClass,currentFileName);  
    pClassDefinition->addAction(pPushClassForVector);
    pParser->addRule(pClassDefinition);

	pGlobalFunctionDefinition = new GlobalFunctionDefinition;
	pGlobalFunctionForVector = new pushGlobalFunctionForVector(pRepo,pClass,pRelation,currentFileName);
	pGlobalFunctionDefinition->addAction(pGlobalFunctionForVector);
	pParser->addRule(pGlobalFunctionDefinition);
	
	pGlobalVariableDefinition = new GlobalVariableDefinition;
	pGlobalVariableForVector = new pushGlobalVariableForVector(pRepo,pClass,pRelation,currentFileName);
	pGlobalVariableDefinition->addAction(pGlobalVariableForVector);
	pParser->addRule(pGlobalVariableDefinition);

    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}


Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComments();
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);

    // add code folding rules

    pFR = new codeFoldingRules;
    pParser->addFoldingRules(pFR);

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);

    // configure to detect and act on function definitions
    // these will stop further rule checking by returning false
	pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);  // no action
    pFunctionDefinition->addAction(pPushFunction);
    pParser->addRule(pFunctionDefinition);

	pClassDefinition = new ClassDefinition;
    pPushClass = new PushClass(pRepo);  
    pClassDefinition->addAction(pPushClass);
    pParser->addRule(pClassDefinition);
	/*
    pGlobalFunctionDefinition = new GlobalFunctionDefinition;
	pGlobalFunctionForVector = new pushGlobalFunctionForVector(pRepo,pClass,pRelation,currentFileName);
	pGlobalFunctionDefinition->addAction(pGlobalFunctionForVector);
	pParser->addRule(pGlobalFunctionDefinition);
	
	pGlobalVariableDefinition = new GlobalVariableDefinition;
	pGlobalVariableForVector = new pushGlobalVariableForVector(pRepo,pClass,pRelation,currentFileName);
	pGlobalVariableDefinition->addAction(pGlobalVariableForVector);
	pParser->addRule(pGlobalVariableDefinition);
	*/
	
	pCallGlobalFunction = new CallGlobalFunction;
	pCallGlobalFunctionAction = new CallGlobalFunctionAction(pRepo,pClass,pRelation,currentFileName);
	pCallGlobalFunction->addAction(pCallGlobalFunctionAction);
	pParser->addRule(pCallGlobalFunction);
	
	pCallGlobalVariable = new CallGlobalVariable;
	pCallGlobalVariableAction = new CallGlobalVariableAction(pRepo,pClass,pRelation,currentFileName);
	pCallGlobalVariable->addAction(pCallGlobalVariableAction);
	pParser->addRule(pCallGlobalVariable);
	
	pInheritanceFound = new InheritanceFound(pRepo);
	pInheritanceAction = new InheritanceAction(pClass, pRelation);
	pInheritanceFound->addAction(pInheritanceAction);
	pParser->addRule(pInheritanceFound);
	
	pCompositionFound = new CompositionFound(pRepo);
	pCompositionAction = new CompositionAction(pRepo,pClass,pRelation);
	pCompositionFound->addAction(pCompositionAction);
	pParser->addRule(pCompositionFound);
	
	pAggregationFound = new AggregationFound(pRepo);
	pAggregationAction = new AggregationAction(pRepo,pClass,pRelation);
	pAggregationFound->addAction(pAggregationAction);
	pParser->addRule(pAggregationFound);

	pUsingFound = new UsingFound(pRepo);
	pUsingAction = new UsingAction(pRepo,pClass,pRelation);
	pUsingFound->addAction(pUsingAction);
	pParser->addRule(pUsingFound);
	
    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}


#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

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
