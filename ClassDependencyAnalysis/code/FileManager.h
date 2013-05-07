#ifndef FILEMANAGER_H
#define FILEMANAGER_H
/////////////////////////////////////////////////////////////////////
// FileManager.h - Support file and directory operations           //
// ver 2.1                                                         //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * This module provides classes, Files finder, a path taker, and a vector for 
 * keeping all found files.
 *
 * Files finder (_FindFiles) accepts the input path parse from function named
 * Getfiles. Then, it find all .h and .cpp files and print out to console. 
 * All files can be found regardless being put in directories with the recursive
 * method. The function firstly traverses in the current directory and get all 
 * files in there. If there are subdirectories, the function concatenate "\\" 
 * in order to traverse inside and get more files. This process recursively keeps
 * going until no related files have left.
 *
 * Path taker (GetFiles) allows the user to give a path that all .h/.cpp files 
 * needed to be searched for. After the function receive the input from keyboard,
 * it then parse input to _FindFiles function for further process.
 *
 * Vector of Files (_AllFiles) is used to keep all results from the previous 
 * functions. It is declared as a vector of string so as to serve the flexible
 * growing size of results.
 *
 * Required Files:
 * ===============
 * FileManager.h, FileManager.cpp
 *
 */

#include "FileSystem.h"
#include <iostream>

class FileManager
{
	private:
		void _FindFiles(std::string path);
		std::vector<std::string> _AllFiles;
	public:
		std::vector<std::string> GetFiles(std::string path);
};

#endif
