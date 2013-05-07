#ifndef TYPEANAL_H
#define TYPEANAL_H
/////////////////////////////////////////////////////////////////////////////
// TypeAnal.h - implement an analysis of determining a "class" which all   //
//				these are used afterwards in later processes.			   //
//  Language:       Visual C++, ver 6.0                                    //
//  Platform:       MacBook Pro, Windows7 Pro				               //
//  Application:    CSE687 project #3	                                   //
//  Author:         YUKUI YE, Syracuse University	     		           //
//                  (315) 751-0156, mollysmile.ye@gmail.com                //
/////////////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==================
 * This module defines a Type analysis class of functions which are used for 
 * 1. Keeping all class names in the vector string. This vector will be reused
 * when analyzing the dependencies between classes. That is, all stored class
 * names will be compared to each collected token whether it has been created
 * earlier or not. This has a high effect to the final result.
 * 2. Printing out all stored class names to console.
 * 3.Returning the vector of class names. Since the function which is directly
 * used to store the names are declared as a private. It is unable to access
 * by any other function outside the class. The function for returning vector
 * is therefore created to specifically take the stored class names out.
 *
 *  Required files
 *  TypeAnal.h, TypeAnal.cpp
 *
 * ==================*/

#include <queue>
#include <string>
#include <sstream>
#include <iostream>


class TypeAnal
{
private:
	std::vector<std::string> ClassName;
public:
	TypeAnal(){}
	void KeepToVector(std::string ToKeep)
	{
		ClassName.push_back(ToKeep);
	}

	void PrintToCheck()
	{
		for(int i=0;i<ClassName.size();i++)
		{
			std::cout<<"\n\  Class names are: "<<ClassName[i];
		}	
	}

	std::vector<std::string>ReturnVector()
	{
		return ClassName;
	}
};

#endif