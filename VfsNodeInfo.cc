//VfsNodeInfo.cc

#include<iostream>
#include"VfsRepository.h"
#include"VfsNodeInfo.h"
using namespace std;

VfsNodeInfo::VfsNodeInfo(string node_name_const, int a): node_name_string(node_name_const) {
	if(a==0)
		node_type=FILE_NAME;
	else
		node_type=FOLDER;
	
}

void VfsNodeInfo::getHeader(HeaderRecord & header_record) { }

string VfsNodeInfo::getName() {
	return node_name_string;
}
