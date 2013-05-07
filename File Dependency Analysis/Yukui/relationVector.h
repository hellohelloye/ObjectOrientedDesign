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

#ifndef RELATIONVECTOR_H
#define RELATIONVECTOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include "classVector.h"

struct elemRelation
{
  std::pair<std::string,std::string> parentClass;  //first is className ,second is its packageName
  std::pair<std::string,std::string> childClass;
  std::string relationship;
};
struct packageRelation
{
	std::string firstPackageName;
	std::string secondPackageName;
	std::string packageRelationship;
};

class relationVector
{
public:
	relationVector(classVector* pCV)
	{
		p_CV = pCV;
	}
	void storeRelationToVector(elemRelation item )
	{
		relationVct.push_back(item);
	}
	elemRelation popRelationFromVector()
	{
		relationVct.pop_back();
	}
	std::vector<elemRelation> getRelationVector()
	{
		return relationVct;
	}
	void pushPackageRelationVector(packageRelation item2)
	{
		packageRelationVct.push_back(item2);
	}
	packageRelation popPackageRelationFromVector()
	{
		packageRelationVct.pop_back();
	}
	
	void  connectPackageRelationVectorFromProject1()	
	{
		packageRelation pkgRelation;
		for(size_t i=0; i<relationVct.size();++i)
		{
			if(relationVct[i].parentClass.second !=  relationVct[i].childClass.second)
			{
				pkgRelation.firstPackageName = relationVct[i].childClass.second;
				pkgRelation.secondPackageName = relationVct[i].parentClass.second;
				pkgRelation.packageRelationship ="Depends";
				pushPackageRelationVector(pkgRelation);
			}
		}
	}
	std::vector<packageRelation> getPackageRelationVector()
	{
		connectPackageRelationVectorFromProject1();
		return packageRelationVct;
	}
	void testRelationVector()
	{
		for(size_t i=0; i<relationVct.size();++i)
		{
			std::cout<<"\n ParentClass name:" << relationVct[i].parentClass.first;
			std::cout<<"\n ParentClass from which packege: "<<relationVct[i].parentClass.second;
			std::cout<<"\n ChildClass name:" << relationVct[i].childClass.first;
			std::cout<<"\n ChildClass from which packege: "<<relationVct[i].childClass.second;
			std::cout<<"\n Relationship:" << relationVct[i].relationship;
		}
	}	
	void testPackageRelationVector()
	{
		for(size_t i=0; i<packageRelationVct.size();++i)
		{
			std::cout<<"\n\n firstPackage:" << packageRelationVct[i].firstPackageName;
			std::cout<<"\n secondPackage: "<<packageRelationVct[i].secondPackageName;
			std::cout<<"\n Relationship:" << packageRelationVct[i].packageRelationship;
		}
	}
private:
	std::vector<elemRelation> relationVct;
	std::vector<packageRelation> packageRelationVct;
	classVector* p_CV;
};
#endif