/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                  //
//  ver 1.3                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FoldingRules.h"
#include "Graph.h"
#include "TypeAnal.h"
#include "RelationshipAnal.h"
#include "XmlWriter.h"

//----< register parsing rule >--------------------------------



void Parser::addRule(IRule* pRule)
{
	
  rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  pFoldingRules->doFold(pTokColl);
  return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
  if(actions.size() > 0)
    for(size_t i=0; i<actions.size(); ++i)
      actions[i]->doAction(pTokColl);
}



//----< test stub >--------------------------------------------

#ifdef TEST_PARSER

#include <queue>
#include <string>
using namespace GraphLib;
typedef Graph<std::string, std::string> graph;//just added
typedef Vertex<std::string, std::string> vertex;//just added
typedef Display<std::string, std::string> display;//just added

template<typename V, typename E>
void showVert(Vertex<V,E>& v)
{
  std::cout << "\n  " << v.id();
}
void mOut(const std::string& message)
{
  std::cout << "\n  " << message.c_str();
}


int main(int argc, char* argv[])
{
  std::cout << "\n  Testing Parser class\n "
            << std::string(22,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  //---------------------first parse-----------------------------//
  TypeAnal* pType = new TypeAnal;
  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

	ConfigParseToConsole configure(pType);
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach("A:\\Masters\\Spring2013\\OOD\\Parser\\ActionsAndRules.h"))
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
		pType->PrintToCheck();
		
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }

  //---------------------second parse-----------------------------//
	graph g;
	XmlWriter wtr;
	std::vector<std::string> ClassName = pType->ReturnVector();
	std::vector<vertex> v;
	
	for(size_t i=0; i<ClassName.size(); ++i)
	{
		vertex v(ClassName.at(i));
		g.addVertex(v);
	}

    display::show(g);
    std::cout << "\n";

   for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

	RelationshipAnal* pRelation = new RelationshipAnal(pType);
	ConfigParseToConsole configure(pType,pRelation);
	Parser* pParser = configure.BuildForRule();
    try
    {
      if(pParser)
      {
        if(!configure.Attach("A:\\Masters\\Spring2013\\OOD\\Parser\\ActionsAndRules.h"))
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
		pRelation->PrintToCheck();
		std::vector<elementRelationship> GetRelation = pRelation->ReturnVector(); 

	  for(int i=0; i<GetRelation.size();i++)
	  {
		  for(int j=0;j<g.size();j++)
		  {
			  for(int k=0;k<g.size();k++)
			  {
				  if((g[j].value()==GetRelation[i].class1)&&(g[k].value()==GetRelation[i].class2))
				  { 
					  g.addEdge(GetRelation[i].Relation,g[j],g[k]);
				  }
			  }
		  }
	  }
    }

    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
	display::show(g);
	}   	   
}

#endif
