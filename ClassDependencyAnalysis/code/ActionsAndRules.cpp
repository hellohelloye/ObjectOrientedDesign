/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.cpp - implements new parsing rules and actions //
//  ver 2.0                                                        //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////

//----< test stub >--------------------------------------------------------
#ifdef TEST_ACTIONSANDRULES

#include <iostream>
#include "ActionsAndRules.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "RelationshipAnal.h"
#include "TypeAnal.h"

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ActionsAndRules class\n "
            << std::string(30,'=') << std::endl;

  RelationshipAnal* p = new RelationshipAnal;
	TypeAnal* q = new TypeAnal;

  try
  {
    PrintPreproc pp;
    PreprocStatement pps;
    pps.addAction(&pp);

    FunctionDefinition fnd;
    PrintFunction prt;
    fnd.addAction(&prt);

	ClassStructEnumDefinition cls1;
	std::vector<std::string> classResult;
	PrintClass cls2(&classResult);
	cls1.addAction(&cls2);

	
	InheritanceDetection* cls3 = new InheritanceDetection; 
	InheritanceAction* cls4 = new InheritanceAction(q, p);
	cls3->addAction(cls4);


    Toker toker("../ActionsAndRules.h");
    SemiExp se(&toker);
    Parser parser(&se);
    parser.addRule(&pps);
    parser.addRule(&fnd);
	parser.addRule(&cls1);
	parser.addRule(cls3);
    while(se.get())
      parser.parse();
    std::cout << "\n\n";
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
}
#endif
