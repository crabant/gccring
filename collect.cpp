//Gccring - collect.cpp          
//                                         
//Copyright (C) 2012 Crabant Yang
//                                         
//This file is part of Gccring.                                                       
//                                                                                   
//    Gccring is free software: you can redistribute it and/or modify                 
//    it under the terms of the GNU General Public License as published by           
//    the Free Software Foundation, either version 3 of the License, or              
//    (at your option) any later version.                                            
//                                                                                   
//    Gccring is distributed in the hope that it will be useful,                      
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                 
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  
//    GNU General Public License for more details.                                   
//                                                                                   
//    You should have received a copy of the GNU General Public License              
//    along with Gccring.  If not, see <http://www.gnu.org/licenses/>.

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "collect.h"
#include "public.h"
#include "utils.h"
#include "cfg.h"

CCollect::CCollect()
{
}
CCollect::~CCollect()
{
}

int CCollect::init(int argc,char* argv[])
{
	int res;

	res=utils::createPathByDirName(CCfg::_rootPath);
	cond_check_r(0==res,"createPathByDirName failed",-1);
	return 0;
}
int CCollect::prepareObjFile(std::string oldFile,std::string& newFile)
{
	//解析出路径和文件名
	char* baseName=utils::basename((char*)oldFile.c_str());
	char* baseDir=utils::dirname((char*)oldFile.c_str());
	cond_check_r(NULL!=baseName,"baseName is NULL",-1);
	cond_check_r(NULL!=baseDir,"baseDir is NULL",-1);

	
	//生成新的文件路径，基于_rootPath
	char tmp[PATH_MAX]={0};
	cond_check_r(realpath(baseDir,tmp)==tmp,"realpath failed",-2);
	newFile=CCfg::_rootPath;
	newFile+=tmp;
	newFile+="/";
	newFile+=baseName;

	//生成文件目录
	std::string tmp1=newFile;
	int res=utils::createPathByFileName(tmp1);
	cond_check_r(0==res,"createPathByFileName failed",-3);

	return 0;
}
int CCollect::copyFiles(std::string newObjFile)
{
	struct stat st;
	int res=stat(newObjFile.c_str(),&st);
	cond_check_r(0==res,"copyFiles stat newObjFile failed",-3);
	if(0>=st.st_size)
	{
		printf("newObjFile is %s\n",newObjFile.c_str());
	}
	cond_check_r(0<st.st_size,"copyFiles newObjFile invalid st_size",-4);
	int len=st.st_size;
	std::vector<char> buf;
	buf.resize(1+len);

	int fd=open(newObjFile.c_str(),O_RDONLY);
	cond_check_r(0<fd,"copyFiles,open newObjFile failed",-5);
	void* mbuf=mmap(NULL,len,PROT_READ,MAP_SHARED,fd,0);	
	close(fd);
	cond_check_r(NULL!=mbuf,"copyFiles mmap failed",-6);

	char* src=(char*)mbuf;
	int pos=0;
	for(int index=0;len>index;++index)
	{
		if('\\'!=src[index] && '\r'!=src[index] && '\n'!=src[index])
			buf[pos++]=src[index];
	}
	munmap(mbuf,len);
	len=pos;
	char* fileName=strtok(&buf[0]," \t");
	char tmp[PATH_MAX];
	while(NULL!=fileName)
	{
		memset(tmp,0,sizeof(tmp));
		if(realpath(fileName,tmp)==tmp)
			copyFile(tmp);
		fileName=strtok(NULL," \t");
	}
	return 0;
}
int CCollect::copyFile(std::string fileName)
{
	if(0<fileName.length())
		if(':'==fileName[fileName.length()-1])
			return 0;
	cond_check_r(!fileName.empty(),"fileName is empty",-1);

	struct stat st;
	int res;
	res=stat(fileName.c_str(),&st);
	cond_check_r(0==res,"stat fileName failed",-2);
	if((!S_ISREG(st.st_mode)) && (!S_ISLNK(st.st_mode)))
		return 0;

	std::string newFile=CCfg::_rootPath;
	newFile+="/";
	newFile+=fileName;
	
	res=utils::createPathByFileName(newFile);
	cond_check_r(0==res,"create path for new copyFile failed",-3);
	res=utils::cp(fileName.c_str(),newFile.c_str());
	cond_check_r(0==res,"cp file failed",-4);
	
	res=addOriListFile(fileName);
	cond_check_r(0==res,"addOriListFile failed",-5);

	res=addNewListFile(newFile);
	cond_check_r(0==res,"addNewListFile failed",-6);
	return 0;
}
int CCollect::addMacros(std::map<std::string,std::string>& macros)
{
	int fd=open(CCfg::_macroFileName.c_str(),O_WRONLY|O_CREAT|O_APPEND,0666);
	cond_check_r(0<fd,"open macro files failed",-1);
	std::map<std::string,std::string>::iterator iter=macros.begin();
	std::vector<char> buf;
	int len;
	const std::string format="#define %s \"%s\"\n";
	for(;macros.end()!=iter;++iter)
	{
		const std::string& key=iter->first;
		const std::string& value=iter->second;
		len=format.length()+key.length()+value.length()+2;
		buf.resize(len);
		len=snprintf(&buf[0],buf.size(),format.c_str(),key.c_str(),value.c_str());
		if(0>=len)
		{
			printf("snprintf failed\n");
			close(fd);
			return -2;
		}
		cond_check_r(0<len,"snprintf failed",-2);
		if(write(fd,&buf[0],len)!=len)
		{
			printf("write failed\n");
			close(fd);
			return -3;
		}
	}
	close(fd);
	return 0;
}
int CCollect::addOriListFile(std::string fileName)
{
	int fd=open(CCfg::_oriListFileName.c_str(),O_WRONLY|O_APPEND|O_CREAT,0666);
	cond_check_r(0<fd,"open list file failed\n",-1);
	std::stringstream ss;
	if(0==fileName.find(CCfg::_oriListFrontRemove))
		fileName=fileName.substr(CCfg::_oriListFrontRemove.length(),fileName.length());
	ss<<CCfg::_oriListPrefix<<fileName<<"\n";
	fileName=ss.str();
	if(CCfg::_changeDirChar)
	{
		for(std::string::size_type pos=0;fileName.length()>pos;++pos)
		{
			if('/'==fileName[pos])
				fileName[pos]='\\';
		}
	}
	size_t writeLen=write(fd,fileName.c_str(),fileName.length());
	close(fd);
	cond_check_r(writeLen==ss.str().length(),"addOriListFile write failed",-2);
	return 0;
}
int CCollect::addNewListFile(std::string fileName)
{
	int fd=open(CCfg::_newListFileName.c_str(),O_WRONLY|O_APPEND|O_CREAT,0666);
	cond_check_r(0<fd,"open list file failed\n",-1);
	std::stringstream ss;
	if(0==fileName.find(CCfg::_newListFrontRemove))
		fileName=fileName.substr(CCfg::_newListFrontRemove.length(),fileName.length());
	ss<<CCfg::_newListPrefix<<fileName<<"\n";
	fileName=ss.str();
	if(CCfg::_changeDirChar)
	{
		for(std::string::size_type pos=0;fileName.length()>pos;++pos)
		{
			if('/'==fileName[pos])
				fileName[pos]='\\';
		}
	}
	size_t writeLen=write(fd,fileName.c_str(),fileName.length());
	close(fd);
	cond_check_r(writeLen==ss.str().length(),"addNewListFile write failed",-2);
	return 0;
}

