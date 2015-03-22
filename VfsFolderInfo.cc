//VfsFolderInfo.cc

#include<iostream>
#include<cstring>
#include<string>
#include"VfsRepository.h"
#include"VfsFileInfo.h"
#include"VfsFolderInfo.h"
using namespace std;

//VfsFolderInfo::VfsFolderInfo() : VfsNodeInfo("/",1) {
//}

VfsFolderInfo::VfsFolderInfo(string dir_name_const, string path_name_const): VfsNodeInfo(dir_name_const,1), node_path(path_name_const)  { }

void VfsFolderInfo::getHeader(HeaderRecord & header_record) {
	header_record.node_type=1;
	header_record.offset=0;
	header_record.size=0;
	strcpy(header_record.folder_path,node_path.c_str());
	strcpy(header_record.node_name,node_name_string.c_str());

}

void VfsFolderInfo::addChildFolder(VfsFolderInfo * childFolder) {
	sub_folders.push_back(childFolder);
}
void VfsFolderInfo::setParentFolder(VfsFolderInfo * parentFolder) {
	parent = parentFolder;
}

void VfsFolderInfo::showFiles(vector<string> & contents) {
	for(int i=0; i<file_info.size() ; i++) {
		string temp_path=file_info[i]->getPath();
		string temp_name=file_info[i]->getName();
		string full_name_str;
		if(temp_path=="/")
			full_name_str=temp_path+temp_name;
		else
			full_name_str=temp_path+"/"+temp_name;
		contents.push_back(full_name_str);



	}
}

void VfsFolderInfo::showFolders(vector<string> & contents) {
	for(int i=0; i<sub_folders.size(); i++) {
		string temp_path = sub_folders[i]->getPath();
		string temp_name = sub_folders[i]->getName();
		string full_name_str;
		if(temp_path!="/") 
			full_name_str = temp_path+"/"+temp_name+"/";
		else
			full_name_str = temp_path+temp_name+"/";
		contents.push_back(full_name_str);
	}
}
	
string VfsFolderInfo::getPath() {
	return node_path;
}
void VfsFolderInfo::addFileInfo(VfsFileInfo * file_info_arg) {
	file_info.push_back(file_info_arg);
}
