//VfsRepository.h

#ifndef VFSREPOSITORY_H
#define VFSREPOSITORY_H
#include<iostream>
#include<vector>
#include<map>

class VfsNodeInfo;

const int MAX_NAME_LENGTH=200;
const int MAX_PATH_LEN=200;
const int MAX_NODES=200;


struct HeaderRecord {
	char node_name[MAX_NAME_LENGTH];
	int node_type;
	int offset;
	long size;
	char folder_path[MAX_PATH_LEN];
};

class VfsRepository {
	std::string repo_name;
	enum {CLOSED,OPEN} repo_status;
	std::string repo_file_path;
	std::vector<VfsNodeInfo *> node_info;
	std::map<std::string, VfsNodeInfo *> nodes;
	int num_nodes;
	void loadHeader();
	void saveHeader();
public:
	//VfsRepository(std::string name, std::string external_path);
	void create (std::string name, std::string external_path);
	void open(std::string name, std::string external_path);
	void close();
	void makeDir(std::string dir_path, std::string dir_name);
	void list(std::string dir_name_arg, std::vector<std::string> & content);
	void copyIn(std::string host_path, std::string vfs_path);
	void copyOut(std::string vfs_path, std::string host_path);

};

#endif
