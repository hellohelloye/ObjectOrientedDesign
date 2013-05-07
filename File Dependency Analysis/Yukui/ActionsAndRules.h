#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//     detect globalFunctionDetection and GlobalVariableDetection  //
//     whenever found package relationship add it into             //
//     packageRelationVector                                       //
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
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "classVector.h"
#include "relationVector.h"
//#include "ConfigureParser.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

struct element
{
  std::string type;
  std::string name;
  size_t lineCount;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << lineCount;
    temp << ")"; 
    return temp.str();
  }
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
  ScopeStack<element> stack;
  Toker* p_Toker;
public:
  Repository(Toker* pToker)
  {
    p_Toker = pToker;
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->lines());
  }
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }

  void doAction(ITokCollection*& pTc)
  {
    if(p_Repos->scopeStack().size() == 0)
      return;
    element elem = p_Repos->scopeStack().pop();
/*    if(elem.type == "function")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
      std::cout << "\n  Function " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
    }*/
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--PreprocStatement rule";
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        //std::cout << "\n--FunctionDefinition rule";
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};



///////////////////////////////////////////////////////////////
// rule to detect class definitions
///////////////////////////////////////////////////////////////
class ClassDefinition : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      if((tc.find("class")<tc.length()) || tc.find("struct")<tc.length() ||tc.find("enum")<tc.length())
      {	 
        doActions(pTc);
        return false;
      }
    }
	if(tc[tc.length()-1]==";")
	{
		if(tc.find("typedef")<tc.length())
		{
			doActions(pTc);
			return false;
		}
	}	
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack 
//as well as onto classVector at the first Build
class PushClassForVector : public IAction
{
  Repository* p_Repos;
  classVector* p_classV;
  std::string pfile;
public:
  PushClassForVector(Repository* pRepos,classVector* pClassV,std::string _file)
  {
    p_Repos = pRepos;
	p_classV = pClassV;
	pfile = _file;
  }
  void doAction(ITokCollection*& pTc)
  {
	ITokCollection& tc = *pTc;
	elemClass classelem;
	if(tc[tc.length()-1] == "{" )
	{ 
		if( tc.find("class") < tc.length())
		{
			std::string name = tc[tc.find("class")+1];
			element elem;			
			classelem.elemName=name;
			classelem.elemType = "class";
			classelem.packageName= pfile;
			elem.type = "class";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);
			p_classV->storeClassToVector(classelem);
		}
		if( tc.find("template")<tc.length() )
		{
			if( tc.find(">") < tc.find("class") )
			{
			std::string name = tc[tc.find("class")+1];
			element elem;
			classelem.elemName=name;
			classelem.elemType = "class";
			classelem.packageName= pfile;
			elem.type = "class";		
			elem.name = name;		
			elem.lineCount = p_Repos->lineCount();	
			p_Repos->scopeStack().push(elem);
			p_classV->storeClassToVector(classelem);
			}
		}
	}
	if(tc[tc.length()-1] == "{" && tc.find("struct")<tc.length())
	{ 
		std::string name = tc[tc.find("struct") + 1];
		element elem;
		classelem.elemName=name;
		classelem.elemType = "class";
		classelem.packageName= pfile;
		elem.type = "struct";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		p_classV->storeClassToVector(classelem);
	}

	if(tc[tc.length()-1] == "{" && tc.find("enum")<tc.length())
	{ 
		std::string name = tc[tc.find("enum") + 1];
		element elem;
		classelem.elemName=name;
		classelem.elemType = "class";
		classelem.packageName= pfile;
		elem.type = "enum";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		p_classV->storeClassToVector(classelem);
	}
	if(tc[tc.length()-1] == ";" && tc.find("typedef")<tc.length())
	{  
		std::string name = tc[tc.find(";") -1];
		element elem;
		classelem.elemName=name;
		classelem.elemType = "class";
		classelem.packageName= pfile;
		elem.type = "typedef";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		p_classV->storeClassToVector(classelem);
	}
  }
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack for second build
class PushClass : public IAction
{
  Repository* p_Repos;
public:
  PushClass(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
	ITokCollection& tc = *pTc;
	if(tc[tc.length()-1] == "{" )
	{ 
		if( tc.find("class") < tc.length())
		{
			std::string name = tc[tc.find("class")+1];
			element elem;			
			elem.type = "class";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);
		}
		if( tc.find("template")<tc.length() )
		{
			if( tc.find(">") < tc.find("class") )
			{
			std::string name = tc[tc.find("class")+1];
			element elem;
			elem.type = "class";		
			elem.name = name;		
			elem.lineCount = p_Repos->lineCount();	
			p_Repos->scopeStack().push(elem);
			}
		}
	}
	if(tc[tc.length()-1] == "{" && tc.find("struct")<tc.length())
	{ 
		std::string name = tc[tc.find("struct") + 1];
		element elem;
		elem.type = "struct";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
	}

	if(tc[tc.length()-1] == "{" && tc.find("enum")<tc.length())
	{ 
		std::string name = tc[tc.find("enum") + 1];
		element elem;
		elem.type = "enum";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
	}
	if(tc[tc.length()-1] == ";" && tc.find("typedef")<tc.length())
	{  
		std::string name = tc[tc.find(";") -1];
		element elem;
		elem.type = "typedef";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
	}
  }
};


//Detect global function
class GlobalFunctionDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		FunctionDefinition funcDetect;
		if(funcDetect.doTest(pTc) == true)
		{
			doActions(pTc);
			return false;
		}
		return true;
	}
};
//global function action for store global function name into vector 
class pushGlobalFunctionForVector :  public IAction
{
	Repository* p_Repos;
	std::string pfile;
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
  pushGlobalFunctionForVector(Repository* pRepos,classVector* pC,relationVector* pR,std::string _file)
  {
    p_Repos = pRepos;
	pfile = _file;
	p_ClassV = pC;
	p_RelationV = pR;
  }
  void doAction(ITokCollection*& pTc)
  {
    elemClass classelem;
	element preElemName;
	bool check=true;
	if(p_Repos->scopeStack().size() == 0)return;
	element currentElem = p_Repos->scopeStack().pop();
    if(p_Repos->scopeStack().size() == 0)return;
	preElemName =  p_Repos->scopeStack().pop();
	if(preElemName.type == "class")
	{
		check = false;
		p_Repos->scopeStack().push(preElemName);
		p_Repos->scopeStack().push(currentElem);
	}
	if(check == true)
    {if(p_Repos->scopeStack().size() == 0)
      return;
		p_Repos->scopeStack().push(preElemName);
		    if(p_Repos->scopeStack().size() == 0)
      return;
		p_Repos->scopeStack().push(currentElem);	
		std::string name= (*pTc)[pTc->find("(") - 1];
		classelem.elemType = "globalFunction";
		classelem.elemName = name;
		classelem.packageName = pfile;
		p_ClassV->storeClassToVector(classelem);		
	}
  }
};
//calling global function 
class CallGlobalFunction : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if((tc.find("(")< tc.length())&& tc.find(";") < tc.length() )
		{
			if( !(tc.find("void") < tc.length()) &&!(tc.find("->") < tc.length() )
				&& !(tc.find(".") < tc.length()) && !(tc.find("new")<tc.length()))
			{
				doActions(pTc); 
				return false;
			}
		}	 
		return false;
	}
};
//calling global function action for package relationship
class CallGlobalFunctionAction :  public IAction
{
private:
	Repository* p_Repos;
	std::string pfile;
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
  CallGlobalFunctionAction(Repository* pRepos,classVector* pC,relationVector* pR,std::string _file)
  {
    p_Repos = pRepos;
	pfile = _file;
	p_ClassV = pC;
	p_RelationV = pR;
  }
  void doAction(ITokCollection*& pTc)
  {
	  ITokCollection& tc = *pTc;
	  packageRelation pkgRelation;
	  for(size_t i=0;i<tc.length();i++)
		for(size_t j=0;j<p_ClassV->getClassVector().size();j++){		
			if(tc[i] == p_ClassV->getClassVector()[j].elemName){
						pkgRelation.firstPackageName = p_ClassV->getClassVector()[j].packageName;	
						pkgRelation.secondPackageName = pfile ; 
						pkgRelation.packageRelationship="Depends";
						p_RelationV->pushPackageRelationVector(pkgRelation); 
				}
			}
	  }
};

//calling global function 
class CallGlobalVariable : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if(tc.find(";") < tc.length() )
		{
			if( !(tc.find("(") < tc.length()) &&!(tc.find("void") < tc.length()) &&!(tc.find("->") < tc.length() )
				&& !(tc.find(".") < tc.length()) && !(tc.find("new")<tc.length()))
			{
				doActions(pTc); 
				return false;
			}
		}	 
		return false;
	}
};

//calling global function action for package relationship
class CallGlobalVariableAction :  public IAction
{
private:
	Repository* p_Repos;
	std::string p_file;
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
  CallGlobalVariableAction(Repository* pRepos,classVector* pC,relationVector* pR,std::string _file)
  {
    p_Repos = pRepos;
	p_file = _file;
	p_ClassV = pC;
	p_RelationV = pR;
  }
  void doAction(ITokCollection*& pTc)
  {
	  ITokCollection& tc = *pTc;
	  packageRelation pkgRelation;
	  for(size_t i=0;i<tc.length();i++)
		for(size_t j=0;j<p_ClassV->getClassVector().size();j++){		
			if(tc[i] == p_ClassV->getClassVector()[j].elemName){
						pkgRelation.firstPackageName = p_ClassV->getClassVector()[j].packageName;
						pkgRelation.secondPackageName = p_file ; 
						pkgRelation.packageRelationship="Depends";
						p_RelationV->pushPackageRelationVector(pkgRelation); 					
				}
			}
	  }
};
//detect global variable
class GlobalVariableDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "int", "double", "size_t", "float", "std::" ,"short"};
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
	if( isSpecialKeyWord(tc[0]))
	{
		doActions(pTc);
		return true;
	}
	return false;
  } 
};

//global variable action: store global variable into vector
class pushGlobalVariableForVector :  public IAction
{
	Repository* p_Repos;
	std::string pfile;
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
  pushGlobalVariableForVector(Repository* pRepos,classVector* pC,relationVector* pR,std::string _file)
  {
    p_Repos = pRepos;
	pfile = _file;
	p_ClassV = pC;
	p_RelationV = pR;
  }
  void doAction(ITokCollection*& pTc)
  {
	elemClass classelem;
	if(p_Repos->scopeStack().size() == 0)return;

	element currentElem = p_Repos->scopeStack().pop();
	    if(p_Repos->scopeStack().size() == 0)return;
	element preElemName = p_Repos->scopeStack().pop(); 
	bool check = true;
	if(preElemName.type == "class" || preElemName.type == "function" )
	{
		p_Repos->scopeStack().push(preElemName);//push back previous pop element in stack
		p_Repos->scopeStack().push(currentElem);
		check = false;
	}
	if(check == true)
	{
		p_Repos->scopeStack().push(preElemName);//push back previous pop element in stack
		p_Repos->scopeStack().push(currentElem);
		std::string name= (*pTc)[pTc->find(";") - 1];
		classelem.elemName = name;
		classelem.elemType = "globalVariable";
		classelem.packageName = pfile;
		p_ClassV->storeClassToVector(classelem);
	}   
  }
};
///////////////////////////////////////////////////////////////
//rules to detect Inheritance between classes, 
class InheritanceFound : public IRule
{
public:
  Repository* p_Repos;
public:
  InheritanceFound(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  bool doTest(ITokCollection*& pTc)
  {
	ITokCollection& tc = *pTc;
    if((tc.find("class") < tc.length()) && (tc.find(":")< tc.length()))
    {
		   doActions(pTc);
		   return false;
    }
	return false;
  }
};
//InheritanceAction
class InheritanceAction : public IAction
{
private:
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
	class InheritanceAction(classVector* pC,relationVector* pR)
	{
		p_ClassV =  pC;
		p_RelationV = pR;
	}
  void doAction(ITokCollection*& pTc)
  {
	ITokCollection& tc = *pTc;
	elemRelation elemR;

		  for(size_t i=0;i<p_ClassV->getClassVector().size();i++)
		  {
			  if(tc[tc.find("class")+1]==p_ClassV->getClassVector()[i].elemName)
			  {
				  elemR.parentClass.first = tc[tc.find("class")+1];
				  elemR.parentClass.second = p_ClassV->getClassVector()[i].packageName;
			  }
		  }
		  for(size_t j=0;j<p_ClassV->getClassVector().size();j++)
		  {
			  if(tc[tc.find(":")+2]==p_ClassV->getClassVector()[j].elemName)
			  { 
				  elemR.childClass.first = tc[tc.find(":")+2];	
				  elemR.childClass.second = p_ClassV->getClassVector()[j].packageName;
			  }		  
		  }
		  elemR.relationship = "Inheritance";
		  p_RelationV->storeRelationToVector(elemR);
  }	
};
///////////////////////////////////////////////////////////////
//rules to detect Composition between classes, 
//and add class name and relationship at the same time.
class CompositionFound:public IRule
{
  Repository* p_Repos;
public:
	CompositionFound(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
  bool doTest(ITokCollection*& pTc)
  {
	ITokCollection& tc = *pTc;
	elemRelation elemR;
	 if( tc.find(";")< tc.length() )
	 {
		if( !(tc.find("*") < tc.length() ) && !(tc.find("&") < tc.length()) && !(tc.find("new")<tc.length()))
		{
			doActions(pTc); 
			return false;
		}
	 }	 
	return false;
  }
};

class CompositionAction : public IAction
{
private:
	Repository* p_Repos;
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
	CompositionAction(Repository* pRepos,classVector* pC,relationVector* pR)
	{
		p_Repos = pRepos;
		p_ClassV = pC;
		p_RelationV = pR;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		elemRelation elemR;
		for(size_t i=0;i<tc.length();i++){
			for(size_t j=0;j<p_ClassV->getClassVector().size();j++){		
				if(tc[i]==p_ClassV->getClassVector()[j].elemName){
				  if(p_Repos->scopeStack().size()>0){
				     element elem= p_Repos->scopeStack().pop();
					 if(elem.type=="class"){
						 elemR.parentClass.first = elem.name;
						 for(size_t k=0;k<p_ClassV->getClassVector().size();k++){
							 if(elem.name == p_ClassV->getClassVector()[k].elemName)
								 elemR.parentClass.second = p_ClassV->getClassVector()[k].packageName;
						 }
						 elemR.childClass.first = tc[i];
						 elemR.childClass.second = p_ClassV->getClassVector()[j].packageName;
						 elemR.relationship="Composition";
						 p_RelationV->storeRelationToVector(elemR);
						 p_Repos->scopeStack().push(elem);
					 }
				  }
				}
			}
		}
	}
};
///////////////////////////////////////////////////////////////
//rules to detect Aggregation between classes, 
//and add class name and relationship at the same time.
class AggregationFound:public IRule
{
  Repository* p_Repos;
public:
  AggregationFound(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  bool doTest(ITokCollection*& pTc)
  {
	ITokCollection& tc = *pTc;
	  if( tc.find("new") < tc.length() )
	  {
		  doActions(pTc);
		  return false;
	   }
	  return false;
  }
};

class AggregationAction : public IAction
{
private:
	Repository* p_Repos;
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
	AggregationAction(Repository* pRepos,classVector* pC,relationVector* pR)
	{
		p_Repos = pRepos;
		p_ClassV = pC;
		p_RelationV = pR;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		elemRelation elemR;
		for(size_t i=0;i<tc.length();i++){
			for(size_t j=0;j<p_ClassV->getClassVector().size();j++){		
				if(tc[i]==p_ClassV->getClassVector()[j].elemName){
					if(p_Repos->scopeStack().size() == 0)return;
				     element elempopfunction = p_Repos->scopeStack().pop();
					 if(elempopfunction.type == "function"){
						 if(p_Repos->scopeStack().size() == 0)return;
						 element elempopClass = p_Repos->scopeStack().pop();
						 for(size_t k=0;k<p_ClassV->getClassVector().size();k++){
							 if(elempopClass.name == p_ClassV->getClassVector()[k].elemName)
								 elemR.parentClass.second = p_ClassV->getClassVector()[k].packageName;
						 }
						 elemR.parentClass.first = elempopClass.name;
						 elemR.childClass.first = tc[i];	
						 elemR.childClass.second = p_ClassV->getClassVector()[j].packageName;
						 elemR.relationship="aggregation";
						 p_RelationV->storeRelationToVector(elemR);
						 p_Repos->scopeStack().push(elempopfunction);
						 p_Repos->scopeStack().push(elempopClass);
					 }
				}
			}
		}
	}
};


///////////////////////////////////////////////////////////////
//rules to detect Using between classes, 
//and add class name and relationship at the same time.
class UsingFound:public IRule
{
	  Repository* p_Repos;
public:
  UsingFound(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
  	element elempopClass;
    ITokCollection& tc = *pTc;
	element elemParent,elemChild,elempopfunction;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1])&&  (tc[len+2]=="&"||tc[len+2]=="*"))
      {
		  doActions(pTc);
		  return false;

      }
    }
    return false;
  }
};


class UsingAction : public IAction
{
private:
	Repository* p_Repos;
	classVector* p_ClassV;
	relationVector* p_RelationV;
public:
	UsingAction(Repository* pRepos,classVector* pC,relationVector* pR)
	{
		p_Repos = pRepos;
		p_ClassV = pC;
		p_RelationV = pR;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		elemRelation elemR;
		for(size_t i=0;i<tc.length();i++){
			for(size_t j=0;j<p_ClassV->getClassVector().size();j++){		
				if(tc[i]==p_ClassV->getClassVector()[j].elemName){
					    if(p_Repos->scopeStack().size() == 0)return;
				     element elempopfunction = p_Repos->scopeStack().pop();
					 if(elempopfunction.type == "function"){
						 if(p_Repos->scopeStack().size() == 0)return;
						 element elempopClass=p_Repos->scopeStack().pop();
						 for(size_t k=0;k<p_ClassV->getClassVector().size();k++){
							 if(elempopClass.name == p_ClassV->getClassVector()[k].elemName)
								 elemR.parentClass.second = p_ClassV->getClassVector()[k].packageName;
						 }
						 elemR.parentClass.first = elempopClass.name;
						 elemR.childClass.first = tc[i];
						 elemR.childClass.second = p_ClassV->getClassVector()[j].packageName;
						 elemR.relationship="Using";
						 p_RelationV->storeRelationToVector(elemR);
						 p_Repos->scopeStack().push(elempopClass);
						 p_Repos->scopeStack().push(elempopfunction);
					 }
				}
			}
		}
	}
};
///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
  }
};

#endif
