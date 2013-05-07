/////////////////////////////////////////////////////////////////////
//classVector.cpp function to test the vector which records  all   //
//              class name,global function name and global variable//
//              name, along with all their fileNames               //
// Ver 1.1                                                         //
// Language:    Visual C++ 2012                                    //
// Platform:    MacPro     virtual Machine Windows 8               //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_CLASSVECTOR

#include <queue>
#include <string>
#include <iostream>
#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FileSystemManagement.h"

std::string getFileName(std::string s)
{
	unsigned found=s.find_last_of("\\");
	s=s.substr(found+1);
	unsigned last=s.find_last_of(".");
	s=s.substr(0,last);
	return s;
}
int main(int argc, char* argv[])
{
  FileSystemManagement *fileManager = new FileSystemManagement();
  classVector* pClass = new classVector; 
  std::vector<std::string>fileList = fileManager->getfileList(argv[1]);
 for (size_t i=0;i<fileList.size();++i) {
	std::string fileName=getFileName(fileList.at(i).c_str());
	ConfigParseToConsole configure(pClass,fileName);

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
	pClass->testClassVector();
	system("pause");
}
#endif
