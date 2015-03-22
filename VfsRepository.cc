//VfsRepository.cc

#include<sys/stat.h>
#include<iostream>
#include<fstream>
#include"PathString.h"
#include"VfsFileInfo.h"
#include"VfsFolderInfo.h"
#include"VfsNodeInfo.h"
#include"VfsRepository.h"
#include"vfs_errors.h"
#include"vfs_errors_def.h"

using namespace std;

bool fileExists(const std::string& filename) {
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1) {
		return true;
	}
	return false;
}



//VfsRepository::VfsRepository(string name, string external_path) : repo_name(name), repo_file_path(external_path), num_nodes(0) { }

void VfsRepository::create(string name, string external_path) {
	fstream repoFile;
	num_nodes=0;
	repo_name=name;
	repo_file_path=external_path;
	if(fileExists(repo_file_path))
		repoFile.open(repo_file_path, ios::in | ios::out | ios::binary);
	else {
		repoFile.open(repo_file_path, ios::out|ios::binary);
		repoFile.close();
		repoFile.open(repo_file_path, ios::in | ios::out | ios::binary);
	}
	if(!repoFile.good()) {
		throw VFS_CREATE_01;
	}
	repoFile.write((char*)&num_nodes,sizeof(int));
	HeaderRecord header_record;
	repoFile.seekp(0,ios::end);
	repoFile.write ((char*) &header_record , sizeof(HeaderRecord)*MAX_NODES);
	repoFile.clear();
	repoFile.close();
}

void VfsRepository::open(string name, string external_path) {
	repo_status = OPEN;
	fstream repoFile;
	repo_name=name;
	repo_file_path=external_path;
	if(!fileExists(repo_file_path)) {
		throw VFS_OPEN_01;
	}
	repoFile.open(repo_file_path, ios::in | ios::out | ios::binary);
	repoFile.read((char*) &num_nodes, sizeof (int));
	HeaderRecord header_record;
	for(int i=0;i<num_nodes;i++) {
		repoFile.read((char*) &header_record, sizeof(HeaderRecord));
		if(header_record.node_type==0) {  			 //it is a file
			string node_name_str(header_record.node_name);
			string folder_path_str(header_record.folder_path);
			string full_name_str=folder_path_str+"/"+node_name_str;
			if(folder_path_str=="/")
				full_name_str=folder_path_str+node_name_str;
		//	cout<<"full name "<<full_name_str<<endl;
			int temp_offset;
			long temp_size;
			temp_offset=header_record.offset;
			temp_size=header_record.size;
			VfsFileInfo * fileItIs = new VfsFileInfo(node_name_str,folder_path_str);
			fileItIs->setInfo(temp_offset, temp_size);   
			fileItIs->setParent(dynamic_cast <VfsFolderInfo*> (nodes[folder_path_str]));
			dynamic_cast<VfsFolderInfo*>(nodes[folder_path_str])->addFileInfo(fileItIs);
			nodes[full_name_str]= fileItIs;	
		}
		else { 							 //it is a folder
			string node_name_str(header_record.node_name);
			string folder_path_str(header_record.folder_path);
			string full_name_str;
			if(folder_path_str=="") {
				makeDir("",""); 

			}
			else if (folder_path_str=="/" ) {
				makeDir("",node_name_str);
			}
			else 
				makeDir(folder_path_str,node_name_str);
			}
	}

	repoFile.clear();
	repoFile.close();
}

void VfsRepository::close() {
	if(repo_status==CLOSED) {
		throw VFS_GEN_01;
	}
	fstream repoFile;
	if(!fileExists(repo_file_path)) {
		throw VFS_CLOSE_01;
	}
	repoFile.open(repo_file_path, ios::in | ios::out | ios::binary);
	if(!repoFile.good()) {
		throw VFS_CLOSE_01;
	}
	num_nodes=nodes.size();
	repoFile.write((char*)&num_nodes,sizeof(int));
	HeaderRecord header_record;
	for(map<string,VfsNodeInfo *>::iterator im=nodes.begin(); im!=nodes.end(); im++) {
		 (im->second)->getHeader(header_record); //virtual function to convert class to structure
		 repoFile.write ((char*) &header_record , sizeof(HeaderRecord));
	}
	nodes.clear();
	repoFile.clear();
	repoFile.close();
	repo_status=CLOSED;
}

void VfsRepository::makeDir(string dir_path,string dir_name) {
	if(repo_status==CLOSED) {
		throw VFS_GEN_01;
	}
	int temp_count=0;
	string full_path_str = dir_path+"/"+dir_name;

	for(map<string,VfsNodeInfo *>::iterator im=nodes.begin(); im!=nodes.end(); im++) 
		if(full_path_str==im->first)
			throw VFS_MAKEDIR_02;

	if(dir_path=="" && dir_name=="") {
		temp_count=1;
		VfsFolderInfo * newFolder = new VfsFolderInfo("/","");
		nodes["/"]=newFolder;
	}
	else if (dir_path=="" && dir_name!="") {
		temp_count=1;
		dir_path="/";
		for(map<string,VfsNodeInfo *>::iterator im=nodes.begin(); im!=nodes.end(); im++) {
			if(dir_path==(im->first)) {
				VfsFolderInfo * newFolder = new VfsFolderInfo(dir_name,"/");
				newFolder->setParentFolder(dynamic_cast <VfsFolderInfo*>(im->second));
				dynamic_cast <VfsFolderInfo*>(im->second)->addChildFolder(newFolder);
				nodes[dir_path+dir_name]=newFolder;
				break;
			}
		}
	}
	else {
		for(map<string,VfsNodeInfo *>::iterator im=nodes.begin(); im!=nodes.end(); im++) {
			if(dir_path==(im->first)) {
				temp_count=1;
				VfsFolderInfo * newFolder = new VfsFolderInfo(dir_name,dir_path); 
				newFolder->setParentFolder(dynamic_cast <VfsFolderInfo*>(im->second)); 
				dynamic_cast <VfsFolderInfo*>(im->second)->addChildFolder(newFolder);
				nodes[full_path_str]=newFolder;
				break;
			}
		}
	}
	if(temp_count==0){
		throw VFS_MAKEDIR_01;
	}
}

void VfsRepository::list(string dir_name_arg, vector<string> & content) {
	if(repo_status==CLOSED) {
		throw VFS_GEN_01;
	}
	int temp=0;
	for(map<string,VfsNodeInfo *>::iterator im=nodes.begin(); im!=nodes.end(); im++) {
		if(dir_name_arg==(im->first)) {
			temp=1;
			dynamic_cast <VfsFolderInfo*> (im->second)->showFiles(content);
			dynamic_cast <VfsFolderInfo*> (im->second)->showFolders(content);
			break;
		}
	}
	if(temp == 0)
		throw VFS_LISTDIR_01;
}


void VfsRepository::copyIn(string host_path, string vfs_path) {
	if(repo_status==CLOSED) {
		throw VFS_GEN_01;
	}
	PathString path (vfs_path,"/");
	string folder_path=path.excludeLast();
	string file_name=path.getLast();
	if(folder_path=="")
		folder_path="/";
	if(!nodes[folder_path])
		throw VFS_COPYIN_01;
	if(nodes[vfs_path])
		throw VFS_COPYIN_02;	
	VfsFileInfo * file_info = new VfsFileInfo(file_name, folder_path);
	file_info->save(host_path,repo_file_path);
	file_info->setParent( dynamic_cast <VfsFolderInfo*> (nodes[folder_path]));
	dynamic_cast <VfsFolderInfo*> (nodes[folder_path])->addFileInfo(file_info);
	nodes[vfs_path]=file_info;
}

void VfsRepository::copyOut(string vfs_path, string host_path) {
	if(repo_status==CLOSED) {
		throw VFS_GEN_01;
	}
	int temp=0;
	for(map<string,VfsNodeInfo *>::iterator im=nodes.begin(); im!=nodes.end(); im++) {
		if(vfs_path==(im->first)) {
			temp=1;
			dynamic_cast <VfsFileInfo *>(im->second)->dExport(host_path,repo_file_path);
			break;
		}
	}
	if(temp==0)
		throw VFS_COPYOUT_01;
}
