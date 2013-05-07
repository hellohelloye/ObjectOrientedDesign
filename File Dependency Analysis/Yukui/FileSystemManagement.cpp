/////////////////////////////////////////////////////////////////////
// FileSystemManagement.cpp                                        //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////


#include <iostream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "FileSystem.h"
#include "FileSystemManagement.h"

FileSystemManagement::FileSystemManagement()
{
	FileList.clear();
}
std::vector<std::string> FileSystemManagement::getfileList(const std::string path)
{
	getAllFile(path);
	return FileList;
}
//modified code by myself: extract folder to get all files
void FileSystemManagement::getAllFile(const std::string& path)
{
	//FileSystem::Directory::setCurrentDirectory(path);
	std::cout<<"Latested Directory Name:"<<path;
	std::cout << "\nIt contains the following files :";
	std::string newPath;
	//get current "*.h" file

	std::vector<std::string> hfilecurr = FileSystem::Directory::getFiles(path,"*.h");
	for(size_t i=0; i<hfilecurr.size(); ++i)
	{
		std::cout << "\n    " << hfilecurr[i].c_str();
		FileList.push_back(hfilecurr[i].c_str());
		//newPath=path+"\\"+hfilecurr[i];
	}
	std::cout << "\n";

	//get current "*.cpp" file
    std::vector<std::string> cppfilecurr = FileSystem::Directory::getFiles(path,"*.cpp");
	std::cout << "\n  :";
	for(size_t i=0; i<cppfilecurr.size(); ++i)
	{
		std::cout << "\n    " << cppfilecurr[i].c_str();
		FileList.push_back(cppfilecurr[i].c_str());
		//path+"\\"+cppfilecurr[i];
	}
	std::cout << "\n";
	
	//extract folder

	std::cout<<"It also contains the following folder:";
	std::vector<std::string> currDir = FileSystem::Directory::getDirectories(path);
	for(size_t i=0;i<currDir.size();i++)
	{
		std::cout << "\n    " << currDir[i].c_str() <<std::endl;
	}
	for(size_t i=2;i<currDir.size();i++)
	{
			//newPath=path+"\\"+currDir[i];
			getAllFile(currDir[i]);
	}
}

#ifdef  TEST_FILESYSTEMMANAGEMENT
int main(int argc, char* argv[])
{
	/*std::cout <<" Number of parameters " << argc << "\n";
	std::cout <<" The program is called " << argv[0] << std::endl;
	std::cout <<" first pameter " << argv[1] << "\n";
	*/
	std::string path;
	std::cout << "input the path:\n";
//	std::cin >> path;
//	std::cout << path;
	FileSystemManagement fileManager;
	std::getline(std::cin,path);
    std::vector<std::string> filevector=fileManager.getfileList(path);
/*	for(size_t i=0; i<filevector.size(); ++i)
		std::cout << "\n    " <<filevector[i];
	std::cout << "\n";
*/	
};
#endif