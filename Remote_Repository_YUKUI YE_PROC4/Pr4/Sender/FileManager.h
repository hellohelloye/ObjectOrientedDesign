#ifndef FILEMANAGER_H
#define FILEMANAGER_H
///////////////////////////////////////////////////////////////
// FileManager.cpp                                           //
// Ver 1.1                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Lenova ThinkPad E420s, Windows 8             //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013     //
// Author:      YUKUI YE,            Syracuse University     //
//              (315) 751-0156, yye@syr.edu                  //
///////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "..\FileSystem\FileSystem.h"

class FileManager
{
public:
	FileManager();
	void getAllFile(const std::string& path);
	std::vector<std::string> getFileList(const std::string& path);
private:
	std::vector<std::string> fileList;
};
#endif

