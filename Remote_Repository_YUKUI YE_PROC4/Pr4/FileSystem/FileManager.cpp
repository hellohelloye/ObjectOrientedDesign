///////////////////////////////////////////////////////////////
// FileManager.cpp                                           //
// Ver 1.1                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    MACPRO, Windows 8                            //
// Application:  CSE687 - OOD, Pr#1, Spring 2013             //
// Author:      YUKUI YE,            Syracuse University     //
//              (315) 751-0156, yye@syr.edu                  //
///////////////////////////////////////////////////////////////
#include "FileManager.h"
#include <iostream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "../FileSystem/FileSystem.h"

FileManager::FileManager()
{
	fileList.clear();
}
std::vector<std::string> FileManager::getFileList(const std::string& path)
{
	getAllFile(path);
	return fileList;
}
void FileManager::getAllFile(const std::string& Path)
{
	std::cout<<"\nLatest Directory FileFolder Path:"<<Path;
	std::cout<<"\nIt contains following files:";
	std::string newPath ;
	std::vector<std::string> currFile = FileSystem::Directory::getFiles(Path,"*.*");
	for(size_t i = 0;  i< currFile.size();i++)
	{
		std::cout<<"\n"<<currFile[i].c_str();
		fileList.push_back(currFile[i]);
	}
	//if the folder contains another folder, do extract folder
	std::vector<std::string> currDir = FileSystem::Directory::getDirectories(Path);
	for(size_t i=0;i<currDir.size();i++)
	{
		std::cout << "\n    " << currDir[i].c_str() <<std::endl;
	}
	for(size_t i=0 ;i<currDir.size();i++)
	{
		if(currDir[i] !="." && currDir[i] !=".."){
			newPath=Path+"\\"+currDir[i];
			getAllFile(newPath);
		}
	}
}
//#endif