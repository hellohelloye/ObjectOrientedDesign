#ifndef CLASSVECTOR_H
#define CLASSVECTOR_H
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

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include  <map>

struct elemClass
{
  std::string elemType;
  std::string elemName;
  std::string packageName;
};

class classVector
{
public:
	void storeClassToVector(elemClass item )
	{
		/*for(int check=1;check< classVct.size();check++)
			if(item.elemName != classVct[check].elemName &&
				item.elemType != classVct[check].elemType &&
				item.packageName!= classVct[check].packageName)*/
				classVct.push_back(item);
	}
	elemClass popClassFromVector()
	{
		classVct.pop_back();
	}
	std::vector<elemClass> getClassVector()
	{
		return classVct;
	}
	void testClassVector()
	{
		for(size_t i=0; i<classVct.size();++i)
		{
			std::cout<<"\n type :" << classVct[i].elemType;
			std::cout<<"\n name:"<<classVct[i].elemName;
			std::cout<<"\n Package name:"<<classVct[i].packageName;
			std::cout<<"\n\n";
		}
	}
	
private:
	std::vector<elemClass> classVct;
    //std::map<int,std::vector<elemClass>> mymap;
};
#endif