//VfsNodeInfo.h

#ifndef VFSNODEINFO_H
#define VFSNODEINFO_H
#include<iostream>

struct HeaderRecord;
class VfsRepository;

class VfsNodeInfo {
	enum {FILE_NAME,FOLDER} node_type;
	VfsRepository * repository;
protected:	
	std::string node_name_string;
public:
	VfsNodeInfo(std::string node_name_const, int a);
	virtual void getHeader(HeaderRecord & header_record)=0;
	std::string getName();

};

#endif
