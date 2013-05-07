/////////////////////////////////////////////////////////////////////
//relationVector.cpp function to test output PackageRelationVector //
// Ver 1.1      transform the class relationship got from project1 //
//            and also store the globalFunction and globalVariable //
//            into packageRelationVector                           //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_RELATIONVECTOR
#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FileSystemManagement.h"
#include "classVector.h"
#include "relationVector.h"
#include "FileSystem.h"
#include <queue>
#include <string>
#include <iostream>


classVector* FirstParserBuild(std::vector<std::string>fileList);
relationVector* SecondParserBuild(std::vector<std::string>fileList,classVector* pCV);

void main(int argc, char* argv[])
{
   FileSystemManagement *fileManager = new FileSystemManagement();
   std::vector<std::string> fileList=fileManager->getfileList(argv[1]);
   classVector* pCV = FirstParserBuild(fileList);
   relationVector* pRV = SecondParserBuild(fileList,pCV);
   system("pause");
}

std::string getFileName(std::string s)
{
	unsigned found=s.find_last_of("\\");
	s=s.substr(found+1);
	unsigned last=s.find_last_of(".");
	s=s.substr(0,last);
	return s;
}
//build parser for the first time to store all the class name and filename
classVector* FirstParserBuild(std::vector<std::string>fileList)
{
	classVector* pClassVector= new classVector;
	
  for (size_t i=0;i<fileList.size();++i) {
	std::string fileName=getFileName(fileList.at(i).c_str());
	ConfigParseToConsole configure(pClassVector,fileName);

    Parser* pParser = configure.firstParserBuild();
    try{
      if(pParser){
        if(!configure.Attach(fileList.at(i).c_str())){
          std::cout << "\n  could not open file " << fileList.at(i).c_str() << std::endl;
          continue;}
      }else{
        std::cout << "\n\n  Parser not built\n\n";
      }
	  while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }catch(std::exception& ex){
      std::cout << "\n\n    " << ex.what() << "\n\n";}
    std::cout << "\n\n";
  } 
	pClassVector->getClassVector();
	return pClassVector;
}

relationVector* SecondParserBuild(std::vector<std::string>fileList,classVector* pCV)
{
	relationVector* pRelationVector = new relationVector(pCV);
    for (size_t i=0;i<fileList.size();++i) {	
	std::string fileName=getFileName(fileList.at(i).c_str());;
	ConfigParseToConsole configure(pCV,pRelationVector,fileName);
    Parser* pParser = configure.Build();
    try{
      if(pParser){
        if(!configure.Attach(fileList.at(i).c_str())){
          std::cout << "\n  could not open file " << fileList.at(i).c_str() << std::endl;
          continue;}
      }else{
        std::cout << "\n\n  Parser not built\n\n";
      }
	  while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }catch(std::exception& ex){
      std::cout << "\n\n    " << ex.what() << "\n\n";}
    std::cout << "\n\n";
  } 
	pRelationVector->getPackageRelationVector();
	pRelationVector->testPackageRelationVector();
	return pRelationVector;
}
#endif
