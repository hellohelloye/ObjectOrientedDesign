/////////////////////////////////////////////////////////////////////
//exeucte.cpp  main function to generate graph and get the xml file//
// Ver 1.1                                                         //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////
//#ifndef EXECUTE_H
//#define EXECUTE_H

#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FileSystemManagement.h"
#include "XmlWriter.h"
#include "XmlReader.h"
#include "classVector.h"
#include "relationVector.h"
#include "FileSystem.h"
#include "Graph.h"
#include <queue>
#include <string>
#include <iostream>

#ifndef TEST_EXECUTE

typedef GraphLib::Graph<std::string, std::string> graph;
typedef GraphLib::Vertex<std::string,std::string> vertex;
typedef GraphLib::Display<std::string,std::string> display;
typedef GraphLib::Utility<std::string,std::string> utils;

classVector* FirstParserBuild(std::vector<std::string>fileList);
relationVector* SecondParserBuild(std::vector<std::string>fileList,classVector* pCV);
void createGraph(relationVector* pRelationV);

void main(int argc, char* argv[])
{
   FileSystemManagement *fileManager = new FileSystemManagement();
   std::vector<std::string> fileList=fileManager->getfileList(argv[1]);
   classVector* pCV = FirstParserBuild(fileList);
   relationVector* pRV = SecondParserBuild(fileList,pCV);
   createGraph(pRV);
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
  } 
	pClassVector->getClassVector();
	return pClassVector;
}

relationVector* SecondParserBuild(std::vector<std::string>fileList,classVector* pCV)
{
	relationVector* pkgRelationVector = new relationVector(pCV);
    for (size_t i=0;i<fileList.size();++i) {	
		std::string fileName=getFileName(fileList.at(i).c_str());;
	ConfigParseToConsole configure(pCV,pkgRelationVector,fileName);
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
  } 
	pkgRelationVector->getPackageRelationVector();
	return pkgRelationVector;
}

void createGraph(relationVector* pRelationV)
{
		graph g;
		std::unordered_map<std::string,int> map;
		std::unordered_map<std::string,int>::const_iterator itr;
		std::vector<packageRelation> pkgRelationship=pRelationV->getPackageRelationVector();
		for(size_t i=0;i<pkgRelationship.size();++i)
		{
			itr = map.find(pkgRelationship[i].secondPackageName);
			if(itr == map.end() )
			{
				vertex v(pkgRelationship[i].secondPackageName);
				g.addVertex(v);
				map[pkgRelationship[i].secondPackageName] = v.id();
			}
			itr = map.find(pkgRelationship[i].firstPackageName);
			if(itr == map.end() )
			{
				vertex v(pkgRelationship[i].firstPackageName);
				g.addVertex(v);
				map[pkgRelationship[i].firstPackageName] = v.id();
			}
			g.addEdge(pkgRelationship[i].packageRelationship,
				g[g.findVertexIndexById(map[pkgRelationship[i].secondPackageName])],
				g[g.findVertexIndexById(map[pkgRelationship[i].firstPackageName])]);	
		}
		std::cout<<"\n\n vertex and edges exit in Graph\n ";
		display::show(g);
		utils::displayStrongConnectedComponents(g);
}
#endif