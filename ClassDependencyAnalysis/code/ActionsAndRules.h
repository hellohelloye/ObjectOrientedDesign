#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
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

#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "TypeAnal.h"
#include "RelationshipAnal.h"


////////////////////////Reo///////////////////////////////////////
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

    if(elem.type == "function")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
      //std::cout << "\n  Function " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
    }
	else if(elem.type == "class")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
      //std::cout << "\n  class " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
    }
	else if(elem.type == "struct")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
      //std::cout << "\n  struct " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
    }
	else if(elem.type == "enum")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
     // std::cout << "\n  enum " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
    }

	else if(elem.type == "typedef")
    {
     // std::cout << "\nHandlePop";
     // std::cout << "\n--popping at line count = " << p_Repos->lineCount();
     // std::cout << "\n  typedef " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
    }
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
// rule to detect class/struct/enum definitions

class ClassStructEnumDefinition : public IRule
{
public:
 
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;

	 if((tc[tc.length()-1] == "{")&& pTc->find("class") < pTc->length())
	 {
        doActions(pTc);
        return true;
	 }

	  if(pTc->find("struct") < pTc->length())
      {
        doActions(pTc);
        return true;
      }

	  if(pTc->find("enum") < pTc->length())
      {
        doActions(pTc);
        return true;
      }
	  
	  if(pTc->find("typedef") < pTc->length())
      {
        doActions(pTc);
        return true;
      }
  }
};

///////////////////////////////////////////////////////////////
// rule to detect class/struct/enum definitions

class ClassForRepo : public IRule
{
public:
 
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;

	 if((tc[tc.length()-1] == "{")&& pTc->find("class") < pTc->length())
	 {
        doActions(pTc);
        return true;
	 }

	  if(pTc->find("struct") < pTc->length())
      {
        doActions(pTc);
        return true;
      }

	  if(pTc->find("enum") < pTc->length())
      {
        doActions(pTc);
        return true;
      }
	  
	  if(pTc->find("typedef") < pTc->length())
      {
        doActions(pTc);
        return true;
      }
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
// action to push class name onto ScopeStack

class PushClass : public IAction
{
  Repository* p_Repos;
private:
	TypeAnal* p_TypeAnal;

public:
	PushClass(Repository* pRepos, TypeAnal* pTypeAnal)
  {
    p_Repos = pRepos;
	p_TypeAnal = pTypeAnal;
  }
	

  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    //p_Repos->scopeStack().pop();
	 ITokCollection& tc = *pTc;
	if(tc[tc.length()-1] == "{"){
		if(tc[tc.length()-4] == "class"){
			std::string name = tc[tc.length()-3];
			element elem;
			elem.type = "class";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_TypeAnal->KeepToVector(elem.name);
			p_Repos->scopeStack().push(elem);		
		}

		 else if(pTc->find("class") < pTc->length()){
			std::string name = (*pTc)[pTc->find("class")+1];
			element elem;
			elem.type = "class";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);
			p_TypeAnal->KeepToVector(elem.name);			
		}

		else if(tc[tc.length()-4] == "struct"){
			std::string name = tc[tc.length()-3];
			element elem;
			elem.type = "struct";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);
			p_TypeAnal->KeepToVector(elem.name);
		}

		else if(tc[tc.length()-4] == "enum"){
			std::string name = tc[tc.length()-3];
			element elem;
			elem.type = "enum";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);
			p_TypeAnal->KeepToVector(elem.name);			
		}
	}

		if(pTc->find("typedef") < pTc->length()){
			std::string name = (*pTc)[pTc->find(";")-1];
			element elem;
			elem.type = "typedef";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();	
			p_Repos->scopeStack().push(elem);
			p_TypeAnal->KeepToVector(elem.name);
		}
  }
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack. This is used as 
// another rule aside from PushClass, so as to be used in the 
// second parse specifically.

class PushClassForRepo : public IAction
{
  Repository* p_Repos;
private:
	TypeAnal* p_TypeAnal;

public:
	PushClassForRepo(Repository* pRepos, TypeAnal* pTypeAnal)
  {
    p_Repos = pRepos;
	p_TypeAnal = pTypeAnal;
  }
	

  void doAction(ITokCollection*& pTc)
  {
	 ITokCollection& tc = *pTc;
	if(tc[tc.length()-1] == "{"){
		if(tc[tc.length()-4] == "class"){
			std::string name = tc[tc.length()-3];
			element elem;
			elem.type = "class";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);		
		}

		 else if(pTc->find("class") < pTc->length()){
			std::string name = (*pTc)[pTc->find("class")+1];
			element elem;
			elem.type = "class";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);		
		}

		else if(tc[tc.length()-4] == "struct"){
			std::string name = tc[tc.length()-3];
			element elem;
			elem.type = "struct";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);
		}

		else if(tc[tc.length()-4] == "enum"){
			std::string name = tc[tc.length()-3];
			element elem;
			elem.type = "enum";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(elem);		
		}
	}

		if(pTc->find("typedef") < pTc->length()){
			std::string name = (*pTc)[pTc->find(";")-1];
			element elem;
			elem.type = "typedef";
			elem.name = name;
			elem.lineCount = p_Repos->lineCount();	
			p_Repos->scopeStack().push(elem);
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

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a Class def
// to console

class PrintClass : public IAction
{
public:
	PrintClass(std::vector <std::string> *p){
		classlist = p;
	}
	std::vector <std::string> *classlist;
  void doAction(ITokCollection*& pTc)
  {
	  classlist->push_back(pTc->show().c_str());
	  std::cout << "\n\n  Class Stmt  : " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect Inheritance 

class InheritanceDetection: public IRule
{
	bool doTest(ITokCollection*& pTc)
	{	
		if((pTc->find("class")< pTc->length()) && (pTc->find(":")< pTc->length()))
		{
			doActions(pTc);
			return true;
		}	 
	}
};


///////////////////////////////////////////////////////////////
// rule to detect Composition
class CompositionDetection: public IRule
{
	Repository* p_Repos;
private:
	TypeAnal* p_TypeAnal;
public:
	class CompositionDetection(TypeAnal* pT, Repository* r)
	{
		p_TypeAnal = pT;
		p_Repos=r;
	
	}
	bool doTest(ITokCollection*& pTc)
	{	
		ITokCollection& tc = *pTc;
		std::vector<std::string> temp=p_TypeAnal->ReturnVector();
		for(int i=0; i<p_TypeAnal->ReturnVector().size();i++)
		{
			if(tc[0]==temp[i])//Check whether the class is stored in vector before
			{
				if(p_Repos->scopeStack().size()>0)//check repository
				{
					element elem = p_Repos->scopeStack().pop();
					if(elem.type == "class")//what is checked has to be in "class"
					{
						p_Repos->scopeStack().push(elem);
						doActions(pTc);						
						return true;
					}
					p_Repos->scopeStack().push(elem);
				}
			}
		}
	return true;
	}
};


///////////////////////////////////////////////////////////////
// rule to detect Aggregation
class AggregationDetection: public IRule
{
private:
	TypeAnal* p_TypeAnal;
public:
	class AggregationDetection(TypeAnal* pT)
	{
		p_TypeAnal = pT;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if(pTc->find("new")< pTc->length())
		{
			std::vector<std::string> temp=p_TypeAnal->ReturnVector();
			for(int i=0; i<p_TypeAnal->ReturnVector().size();i++)
			{
				if(tc[0]==temp[i])//Check whether the class is stored in vector before
				{
				doActions(pTc);
				}
			}
		}
		return true;
	}
};


///////////////////////////////////////////////////////////////
// rule to detect Using
class UsingDetection: public IRule
{
	Repository* p_Repos;
private:
	TypeAnal* p_TypeAnal;
	RelationshipAnal* p_RelationshipAnal;
public:
	class UsingDetection(TypeAnal* pT, RelationshipAnal* pR,Repository* r)
	{
		p_TypeAnal = pT;
		p_RelationshipAnal = pR;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		FunctionDefinition fCheck;
		if(fCheck.doTest(pTc)==true)
		{
			int FirstParen, SecondParen;
			FirstParen = pTc->find("(");
			SecondParen = pTc->find(")");
			std::vector<std::string> temp=p_TypeAnal->ReturnVector();
			for(int i=0; i<p_TypeAnal->ReturnVector().size();i++)
			{
				if(tc[FirstParen+1]==temp[i])//Check whether the class is stored in vector before
				{
					if((pTc->find("*")< pTc->length())||(pTc->find("&")< pTc->length()))
					{
						doActions(pTc);
					}
				}
			}
		}
		return true;
	}
};


///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a Inheritance def 
// to console

class InheritanceAction : public IAction
{
private:
	TypeAnal* p_TypeAnal;
	RelationshipAnal* p_RelationshipAnal;
public:
	class InheritanceAction(TypeAnal* pT, RelationshipAnal* pR)
	{
		p_TypeAnal = pT;
		p_RelationshipAnal = pR;
	}

  void doAction(ITokCollection*& pTc)
	 {	  
	  ITokCollection& tc = *pTc;
	  size_t len = pTc->find(":");
	  elementRelationship elem;
	  
	  std::vector<std::string> temp = p_TypeAnal->ReturnVector();
	  bool checkFirstClass;
	  bool checkSecondClass;

			for(int i=0;i<temp.size();i++)
			{
				if(temp[i]==tc[len-1])
					checkFirstClass=true;
			}

			for(int i=0;i<temp.size();i++)
			{
				if(temp[i]==tc[len+2])
					checkSecondClass=true;
			}

			if((checkFirstClass=true)&&(checkSecondClass=true))
			{
				elem.class1 = tc[len-1];
				elem.class2 = tc[len+2];
				elem.Relation = "Inheritance";
				p_RelationshipAnal->AddRelationship(elem);
			}							
		}
};


///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a Composition def 
// to console
class CompositionAction : public IAction
{
	Repository* p_Repos;
private:
	RelationshipAnal* p_RelationshipAnal;
public:
	class CompositionAction(RelationshipAnal* pR, Repository* pRepo)
	{
		p_RelationshipAnal = pR;
		p_Repos = pRepo;
	}

	void doAction(ITokCollection*& pTc)
	{	 
		ITokCollection& tc = *pTc;
		elementRelationship elem;
		element getClassName = p_Repos->scopeStack().pop();
		elem.class1 = getClassName.name;
		elem.class2 = tc[0];
		elem.Relation = "Composition";
		p_RelationshipAnal->AddRelationship(elem);
		p_Repos->scopeStack().push(getClassName);
	}
};


///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a Aggregation def 
// to console
class AggregationAction : public IAction
{
	Repository* p_Repos;
private:
	RelationshipAnal* p_RelationshipAnal;
public:
	class AggregationAction(RelationshipAnal* pR, Repository* pRepo)
	{
		p_RelationshipAnal = pR;
		p_Repos = pRepo;
	}

	void doAction(ITokCollection*& pTc)
	{	
		ITokCollection& tc = *pTc;
		elementRelationship elem;
		element getClassName;

		std::vector<element>temp ;
		getClassName = p_Repos->scopeStack().pop();
		temp.push_back(getClassName);

		while(getClassName.type!="class")
		{
			getClassName = p_Repos->scopeStack().pop();
			temp.push_back(getClassName);
		}
		
		if(getClassName.type=="class")
		{
			elem.class1 = getClassName.name;
			elem.class2 = tc[0];
			elem.Relation = "Aggregation";
			p_RelationshipAnal->AddRelationship(elem);
		}
		
		for(int i=0;i<temp.size();i++)
		{
			getClassName = temp.at(i);
			p_Repos->scopeStack().push(getClassName);
		}

	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a Using def 
// to console
class UsingAction : public IAction
{
	Repository* p_Repos;
private:
	RelationshipAnal* p_RelationshipAnal;
public:
	class UsingAction(RelationshipAnal* pR, Repository* pRepo)
	{
		p_RelationshipAnal = pR;
		p_Repos = pRepo;
	}

	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		elementRelationship elem;
		element getClassName;
		int FirstParen;
		FirstParen = pTc->find("(");

		std::vector<element>temp ;
		getClassName = p_Repos->scopeStack().pop();
		temp.push_back(getClassName);

		while(getClassName.type!="class")
		{
			getClassName = p_Repos->scopeStack().pop();
			temp.push_back(getClassName);
		}
		
		if(getClassName.type=="class")
		{
			elem.class1 = getClassName.name;
			elem.class2 = tc[FirstParen+1];
			elem.Relation = "Using";
			p_RelationshipAnal->AddRelationship(elem);
		}
		
		for(int i=0;i<temp.size();i++)
		{
			getClassName = temp.at(i);
			p_Repos->scopeStack().push(getClassName);
		}
	}
};
#endif
