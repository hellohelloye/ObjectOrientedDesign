#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 2.0                                                        //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv Project1HelpS06.sln
    - cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
         ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/

//
#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "FoldingRules.h"
#include "classVector.h"
#include "relationVector.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
protected:
	std::string currentFileName;
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  ConfigParseToConsole(classVector* pC,std::string file) 
  { 
	  pClass = pC; 
	  currentFileName = file;
  };
  ConfigParseToConsole(classVector* pC,relationVector* pR,std::string file)
  {
	  pClass = pC;
	  pRelation = pR;
	  currentFileName = file;
  }; 
  bool Attach(const std::string& name, bool isFile=true);
  Parser* firstParserBuild();
  Parser* Build();

private:
  // Builder must hold onto all the pieces

  Toker* pToker;
  SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;
  classVector* pClass;     //store all the className
  relationVector* pRelation;

  // add folding rules

  FoldingRules* pFR;

  // add Rules and Actions

  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;

  ClassDefinition* pClassDefinition;
  PushClassForVector* pPushClassForVector;
  PushClass* pPushClass;

  GlobalFunctionDefinition* pGlobalFunctionDefinition;
  pushGlobalFunctionForVector* pGlobalFunctionForVector;

  GlobalVariableDefinition* pGlobalVariableDefinition;
  pushGlobalVariableForVector* pGlobalVariableForVector;
  
  CallGlobalFunction* pCallGlobalFunction;
  CallGlobalFunctionAction* pCallGlobalFunctionAction;
  
  CallGlobalVariable* pCallGlobalVariable;
  CallGlobalVariableAction* pCallGlobalVariableAction;

  InheritanceFound* pInheritanceFound;
  InheritanceAction* pInheritanceAction;

  CompositionFound* pCompositionFound;
  CompositionAction* pCompositionAction;

  AggregationFound* pAggregationFound;
  AggregationAction* pAggregationAction;

  UsingFound* pUsingFound;
  UsingAction* pUsingAction;
  // prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&);
  ConfigParseToConsole& operator=(const ConfigParseToConsole&);
};
#endif
