//VfsFileInfo.cc
#include<sys/stat.h>
#include<iostream>
#include<cstring>
#include"VfsRepository.h"
#include"VfsFile.h"
#include"VfsFolderInfo.h"
#include"VfsFileInfo.h"
#include"vfs_errors.h"
using namespace std;

long GetFileSize(std::string filename) {
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

bool fileExistsr(const std::string& filename) {
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1) {
		return true;
	}
	return false;
}



VfsFileInfo::VfsFileInfo(string file_name_const, string folder_path_const) :VfsNodeInfo(file_name_const,0), node_path(folder_path_const) { }

void VfsFileInfo::getHeader(HeaderRecord & header_record) {
	header_record.node_type=0;
	header_record.offset=file_offset;
	header_record.size=file_bytes;
	strcpy(header_record.folder_path,node_path.c_str());
	strcpy(header_record.node_name,node_name_string.c_str());
}

void VfsFileInfo::setParent(VfsFolderInfo * folder_info_arg) {
	folder_info=folder_info_arg;
}

void VfsFileInfo::save(string fully_qualified_name_in_host_object, string repo_file_path_arg) { 
	if (!fileExistsr(fully_qualified_name_in_host_object))
		throw VFS_COPYIN_03; //error
	//else {
		//if(!fileExistsr(repo_file_path_arg)){
		//	cout<<"chacha2"; //repo does not exist
		//}
		fstream repoFile;
		repoFile.open(repo_file_path_arg,ios::in|ios::out|ios::binary);
		file_bytes=GetFileSize(fully_qualified_name_in_host_object);
		repoFile.seekp(0,ios::end);
		metadata = new VfsFile();
		metadata->saveInVFS(repoFile,fully_qualified_name_in_host_object,file_offset,file_bytes); 
		delete metadata;
		repoFile.clear();
		repoFile.close();
	//}
}

void VfsFileInfo::dExport(string host_path, string repo_path_arg) {
	if(fileExistsr(host_path))
		throw VFS_COPYOUT_02;
	//if(!fileExistsr(repo_path_arg))
	//	cout<<"repo not found"<<endl;	
	metadata = new VfsFile();
	fstream repoFile;
	repoFile.open(repo_path_arg, ios::in|ios::out|ios::binary);
	metadata->exportFile(repoFile, file_offset, file_bytes, host_path);
	delete metadata;
	repoFile.clear();
	repoFile.close();
}

void VfsFileInfo::setInfo(int offset_srg, long size_arg) {
	file_offset=offset_srg;
	file_bytes=size_arg;
}

string VfsFileInfo::getPath() {
	return node_path;
}
	 
