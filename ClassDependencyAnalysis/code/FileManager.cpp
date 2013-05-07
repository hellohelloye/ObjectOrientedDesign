/////////////////////////////////////////////////////////////////////
// FileManager.cpp - Support file and directory operations         //
// ----------------------------------------------------------------//
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////

#include "FileManager.h"

std::vector<std::string> FileManager::GetFiles(std::string path){
	_FindFiles(path);
	return _AllFiles;
} 

void FileManager::_FindFiles(std::string path){
	std::string newPath ="";
	FileSystem::Directory::setCurrentDirectory(path);
	std::cout << "\n\n  Directory name:  "<<path;
	std::cout << "\n  it contains files:";
	//Get all .h files
	std::vector<std::string> currfiles_h = FileSystem::Directory::getFiles(path,"*.h");
	if(currfiles_h.size() == 0)
		std::cout << "\n    No .h file exists in this directory";
	else{
		for(size_t i=0; i<currfiles_h.size(); ++i) {
		std::cout << "\n    " +path +"\\"+currfiles_h[i].c_str();
		_AllFiles.push_back(path +"\\"+currfiles_h[i].c_str());
		}
	}
	//Getl all .cpp files
	std::vector<std::string> currfiles_cpp = FileSystem::Directory::getFiles(path,"*.cpp");
	if(currfiles_cpp.size() == 0)
		std::cout << "\n    No .cpp file exists in this directory";
	else{
		for(size_t i=0; i<currfiles_cpp.size(); ++i) {
			std::cout << "\n    " +path +"\\"+currfiles_cpp[i].c_str();
			_AllFiles.push_back(path +"\\"+currfiles_cpp[i].c_str());
		}
	}
	std::vector<std::string> currdirs = FileSystem::Directory::getDirectories(path);

	for(size_t i=0; i<currdirs.size(); ++i){
		if(currdirs[i]!="." && currdirs[i]!=".."){
			newPath = path+"\\"+currdirs[i];
			_FindFiles(newPath);
		}
	}
}


//----< test stub >--------------------------------------------------------

#define TEST_FILEMANAGER
#ifdef TEST_FILEMANAGE
int main(int argc, char* argv[])
{
	FileManager f;
	std::string InputPath;
	std::cout<<"Enter path:";
	std::getline(std::cin, InputPath);
	
	std::vector<std::string>files = f.GetFiles(InputPath);

}
#endif