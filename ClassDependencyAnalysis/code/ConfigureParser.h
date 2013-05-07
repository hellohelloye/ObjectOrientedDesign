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

#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "FoldingRules.h"
//#include "TypeAnal.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  ConfigParseToConsole(TypeAnal* p) {pType=p;};
  ConfigParseToConsole(TypeAnal* p,RelationshipAnal* r) //JUST ADDED
  {
	  pType=p;
	  pRelationship = r;
  };
  bool Attach(const std::string& name, bool isFile=true);
  Parser* Build();
  Parser* BuildForRule();

private:
  // Builder must hold onto all the pieces

  Toker* pToker;
  SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;
  TypeAnal* pType;
  RelationshipAnal* pRelationship;//ADD
  

  // add folding rules

  FoldingRules* pFR;

  // add Rules and Actions

  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;
  ClassStructEnumDefinition* pClassDefinition;
  PushClass* pPushClass;
  ClassForRepo* pClassForRepo;
  PushClassForRepo* pPushClassForRepo;
  InheritanceDetection* pInheritanceDetection;
  InheritanceAction* pInheritanceAction;
  CompositionDetection* pCompositionDetection;
  CompositionAction* pCompositionAction;
  AggregationDetection* pAggregationDetection;
  AggregationAction* pAggregationAction;
  UsingDetection* pUsingDetection;
  UsingAction* pUsingAction;

  // prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&);
  ConfigParseToConsole& operator=(const ConfigParseToConsole&);
 // ConfigParseToConsole& operator=(TypeAnal&);

};


#endif
