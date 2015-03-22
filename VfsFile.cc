//VfsFile.cc

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<sys/stat.h>
#include"VfsFileInfo.h"
#include"VfsFile.h"
#include"vfs_errors.h"
using namespace std;

void VfsFile::exportFile(std::fstream & repository, int & offset_arg, long & file_bytes_arg, std::string host_path)  {
	repository.seekg(offset_arg,ios::beg);
	long chunks = file_bytes_arg/BUFFER_SIZE;
	long left = file_bytes_arg-(chunks*BUFFER_SIZE);
	fstream hostFile;
	hostFile.open(host_path, ios::out|ios::binary);
	if(!hostFile.good())
		throw VFS_COPYOUT_02;
	hostFile.close();
	hostFile.open(host_path,ios::out|ios::in|ios::binary);
	if(!hostFile.good())
		throw VFS_COPYOUT_02;
	for(long i=0;i<chunks;i++) { 
		repository.read((char *)data_bytes, BUFFER_SIZE);
		hostFile.write((char *)data_bytes, BUFFER_SIZE);
	}
	repository.read((char *)data_bytes, left);
	hostFile.write((char *)data_bytes, left);
	hostFile.clear();
	hostFile.close();
	repository.clear();
}

void VfsFile::saveInVFS(std::fstream & repository, string external_path, int &file_offset, long &file_bytes) {
	fstream realFile;
	realFile.open(external_path,  ios::in | ios::binary);
	struct stat buf;
	stat(external_path.c_str(), &buf);
	file_bytes = buf.st_size; 

	long chunks = file_bytes/BUFFER_SIZE;
	long left = file_bytes-(chunks*BUFFER_SIZE);

	repository.seekp(0,ios::end);
	file_offset=repository.tellp();

	for(long i=0;i<chunks;i++) {
		realFile.read((char *)data_bytes, BUFFER_SIZE);
		repository.write((char *)data_bytes, BUFFER_SIZE);
	}
	realFile.read((char *)data_bytes, left);
	repository.write((char *)data_bytes, left);

	realFile.clear();
	realFile.close();
	repository.clear();
}



