#ifndef RELATIONSHIPANAL_H
#define RELATIONSHIPANAL_H
/////////////////////////////////////////////////////////////////////
// RelationshipAnal.h - Support file and directory operations      //
// ver 2.1                                                         //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * This module defines a Relationship analysis class of functions  used for 
 * 1. Keeping structs which consist of two class names and their relationship
 * This makes it easy to utilize later one when doing a graph and need to
 * pick out only specific element from the struct.
 * 2. Keeping all relationships in the vector string. This vector will be reused
 * when analyzing the dependencies between classes. That is, all stored relationships
 * will be compared to each collected token whether it has been created
 * earlier or not. This has a high effect to the final result.
 * 2. Printing out all stored relationships to console.
 * 3.Returning the vector of relationships. Since the function which is directly
 * used to store the relationships are declared as a private. It is unable to access
 * by any other function outside the class. The function for returning vector
 * is therefore created to specifically take the stored relationships out.
 *
 *  Required files
 *  RelationshipAnal.h, RelationshipAnal.cpp
 *
 * ==================*/

#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include "TypeAnal.h"

struct elementRelationship
{
  std::string class1;
  std::string class2;
  std::string Relation;
};

class RelationshipAnal
{
private:
	std::vector<elementRelationship> KeepRelation;
	TypeAnal* p_TypeAnal;
public:
	RelationshipAnal(TypeAnal* pType)
	{
		p_TypeAnal = pType; 
	}

	void AddRelationship(elementRelationship ToKeep)
	{
		KeepRelation.push_back(ToKeep);
	}

	void PrintToCheck()
	{
		for(int i=0;i<KeepRelation.size();i++)
		{
			std::cout<<"\n  Class1 : "<< KeepRelation[i].class1;
			std::cout<<"\n  Class2 : "<< KeepRelation[i].class2;
			std::cout<<"\n  Relationship : "<< KeepRelation[i].Relation;
		}	
	}

	std::vector<elementRelationship>ReturnVector()
	{
		return KeepRelation;
	}
};
#endif