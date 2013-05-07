/////////////////////////////////////////////////////////////////////
//  RelationshipAnal.cpp - Analyzes C++ language constructs        //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include "RelationshipAnal.h"


//----< test stub >--------------------------------------------------------
#define TEST_RELATIONSHIPANAL
#ifndef TEST_RELATIONSHIPANAL
int main(int argc, char* argv[])
{
std::vector<vertex> v;
	std::vector<std::string> ClassName = VectorFromFirstParse->ReturnVector();
	RelationshipAnal* pRelation = new RelationshipAnal(VectorFromFirstParse);
	for(int i=0; i<files.size(); ++i)
	{
		ConfigParseToConsole configure(VectorFromFirstParse,pRelation);
		Parser* pParser = configure.BuildForRule();
		try
		{
		  if(pParser)
		  {
			if(!configure.Attach(files.at(i).c_str()))
			{
			  std::cout << "\n  could not open file " << files.at(i).c_str() << std::endl;
			  continue;
			}
		  }
		  else
		  {
			std::cout << "\n\n  Parser not built\n\n";
		  }
		  // now that parser is built, use it

		   while(pParser->next())
			pParser->parse();
		}
		catch(std::exception& ex)
		{
		  std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		std::cout << "\n\n";
	
   }
	std::cout<<"  Second parse : Findig relationship among the files\n";
	std::cout<<"  --------------------------------------------------\n";
	pRelation->PrintToCheck();
	return pRelation;

}
#endif