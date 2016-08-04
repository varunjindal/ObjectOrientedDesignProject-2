#ifndef FILEMANAGER_H
#define FILEMANAGER_H
/////////////////////////////////////////////////////////////////////
//  FileManager.h  - This package locates files in a directory     //
//					  with a specific pattern					   //
//  ver 1.0                                                        //
//  Language:     C++, Visual Studio 2105                          //
//  Application:  Code Parser with Abstract Syntax Tree		       //
//				  CSE687 - Object Oriented Design				   //
//  Source:         Jim Fawcett, Syracuse University, CST 4 - 187  //
//                jfawcett@twcny.rr.com							   //
//  Author:       Varun Jindal - vjindal@syr.edu                   //
/////////////////////////////////////////////////////////////////////
/*
	Package Operations:
	==================
	This package locates files in a directory with a specific pattern in a specified path

	Build Process:
	==============
	Required files
	- FileManager.h, FileManager.cpp

	Maintenance History:
	====================
	ver 1.0 : 17 Mar 16 - first release

	Public Functions:
	=================
	 addDir(std::string)
	 getFiles(std::string, std::vector<std::string>)

*/
//

#include<iostream>
#include<string>
#include<vector>
#include"../FileSystem-Windows/FileSystemDemo/FileSystem.h"

class FileManager {
public:
	FileManager();
	void addDir(std::string);
	std::vector<std::string> getFiles(std::string, std::vector<std::string>);
	~FileManager();
private:
	std::string dir;
	static std::vector<std::string> filesInDir;
	std::vector<std::string> find(const std::string&, std::vector<std::string>);
};

#endif