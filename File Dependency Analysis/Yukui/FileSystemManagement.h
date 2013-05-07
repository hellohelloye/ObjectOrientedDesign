/////////////////////////////////////////////////////////////////////
// FileSystemManagement.h                                          //
// Ver 1.1                                                         //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////
#ifndef FILESYSEMMANAGER_H
#define FILESYSEMMANAGER_H
#include "FileSystem.h"

class FileSystemManagement 
{
public:
	FileSystemManagement();
	void getAllFile(const std::string& path);
	//void setfileList(std::vector<std::string>& fl);
    std::vector<std::string> FileSystemManagement::getfileList(const std::string path);
private:
	std::vector <std::string> FileList;
   
};
#endif