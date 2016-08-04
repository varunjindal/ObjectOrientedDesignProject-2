//////////////////////////////////////////////////////////////////////////////////
//  FileManager.cpp - This package locates files in a directory				    //
//					  with a specific pattern									//
//  ver 1.0                                                                     //
//  Language:          C++, Visual Studio 2105							        //
//  Application:       Code Parser with Abstract Syntax Tree					//
//				       CSE687 - Object Oriented Design						    //
//  Source:            Jim Fawcett, Syracuse University, CST 4-187			    //
//                     jfawcett@twcny.rr.com									//
//  Author:			   Varun Jindal - vjindal@syr.edu                           //
//////////////////////////////////////////////////////////////////////////////////

#include "FileManager.h"
#include <iostream>
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

using namespace FileSystem;



//----< Constructor >-------------
FileManager::FileManager() {

}

// -------< Destructor > ---------
FileManager :: ~FileManager(){}

//----< Adds path of the file >-----------
void FileManager::addDir(std::string path) {
	dir = path;
}

//----< gets paths of all the files matching a particular pattern >----------
std::vector<std::string> FileManager::getFiles(std::string path, std::vector<std::string> patterns_) {
	filesInDir = {};
	return find(path, patterns_);
}

//----< finds all files matching a pattern and give them to getFiles >
std::vector<std::string> FileManager::find(const std::string& path, std::vector<std::string> patterns_)
{
	std::string fpath = FileSystem::Path::getFullFileSpec(path);
	for (auto patt : patterns_)  
	{                            
		std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
		for (auto f : files)
		{
			std::string fileName = fpath + "\\" + f;
			filesInDir.push_back(fpath + "\\" + f);
		}
	}
	std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);

	for (auto d : dirs)
	{
		if (d == "." || d == "..")
			continue;
		std::string dpath = fpath + "\\" + d;  // here's the fix
		find(dpath, patterns_);
	}
	return filesInDir;
}

std::vector<std::string> FileManager::filesInDir = {};

#ifdef TEST_FILEMANAGER
int main(int argc, char* argv[]) {
	string a = Directory::getCurrentDirectory();
	cout << a;
	FileManager fileManager;
	vector<string> files = fileManager.getFiles(a, "*.h");
	for (int i = 0; i < files.size(); i++)
		cout << endl << files[i];
	return 0;
}
#endif