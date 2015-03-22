//VfsFile.h

#ifndef VFSFILE_H
#define VFSFILE_H
#include<iostream>
#include<fstream>

class VfsFileInfo;
const int BUFFER_SIZE=1024;

class VfsFile {
	char data_bytes[BUFFER_SIZE];
	VfsFileInfo * data_file;
public:
	void exportFile(std::string external_path);
	void saveInVFS(std::fstream & repository, std::string external_path, int &file_offset, long &file_bytes);
	void exportFile(std::fstream & repository, int & offset_arg, long & file_bytes_arg, std::string host_path);	
};

#endif
