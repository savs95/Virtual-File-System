//VfsFolderInfo.h

#ifndef VFSFOLDERINFO_H
#define VFSFOLDERINFO_H
#include<iostream>
#include<vector>
#include"VfsNodeInfo.h"

class VfsFileInfo;

class VfsFolderInfo : public VfsNodeInfo {
	std::string node_path;
	std::vector<VfsFileInfo *>file_info;
	std::vector<VfsFolderInfo *>sub_folders;
	VfsNodeInfo * parent;
public:
	//VfsFolderInfo();
	VfsFolderInfo(std::string dir_name_const, std::string path_name_const);
	void getHeader(HeaderRecord & header_record);
	void addChildFolder(VfsFolderInfo * childFolder);
	void setParentFolder(VfsFolderInfo * parentFolder);
	void showFiles(std::vector<std::string> & contents);
	void showFolders(std::vector<std::string> & contents);
	std::string getPath();
	void addFileInfo(VfsFileInfo * file_info_arg);
};

#endif
