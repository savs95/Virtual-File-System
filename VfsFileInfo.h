//VfsFileInfo.h

#ifndef VFSFILEINFO_H
#define VFSFILEINFO_H
#include<iostream>
#include<fstream>
#include"VfsNodeInfo.h"

class VfsFile;
class VfsFolderInfo;


class VfsFileInfo : public VfsNodeInfo {
	int file_offset;
	long file_bytes;
	std::string node_path;
	VfsFile * metadata;
	VfsFolderInfo * folder_info;
public:
	VfsFileInfo(std::string file_name_const, std::string folder_path_const);
	void getHeader(HeaderRecord & header_record);
	void save(std::string fully_qualified_name_in_host_object, std::string repo_file_path_arg);
	void setParent(VfsFolderInfo * folder_info_arg);
	void dExport(std::string host_path, std::string repo_path_arg);
	void setInfo(int offset_srg, long size_arg);
	std::string getPath();
};

#endif



