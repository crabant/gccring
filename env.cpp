//Gccring - env.cpp          
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <vector>
#include <limits.h>

#include "env.h"
#include "public.h"
#include "utils.h"


int CEnv::getExeFullPath(std::string& path)
{
	char tmp[256]={0};
	snprintf(tmp,sizeof(tmp),"/proc/%d/exe",getpid());
	char link[256]={0};
	int len=readlink(tmp,link,sizeof(link)-1);
	if(0>=len)
	{
		printf("readlink falied\n");
		return -2;
	}
	std::string dir=utils::dirname(link);
	path=dir;
	return 0;
}

int CEnv::getPaths(std::list<std::string>& paths)
{	
	std::string path=getenv("PATH");
	std::vector<char> buf;
	buf.resize(path.length()+1);
	memcpy(&buf[0],path.c_str(),path.length());
	char* str=strtok(&buf[0],":");
	while(NULL!=str)
	{
		paths.push_back(str);
		str=strtok(NULL,":");
	}
	return 0;
}
int CEnv::setPaths(std::list<std::string>& paths)
{
	std::string path;
	for(std::list<std::string>::iterator iter=paths.begin();paths.end()!=iter;++iter)
	{
		if(!path.empty())
			path+=":";
		path+=*iter;
	}
	setenv("PATH",path.c_str(),1);
	return 0;
}
int CEnv::kickExePath(std::list<std::string>& paths,std::string exeFullPath)
{
	char tmp[PATH_MAX]={0};
	std::string curDir=get_current_dir_name();
	
	if(tmp!=realpath(exeFullPath.c_str(),tmp))
	{
		printf("realpath(%s) failed\n",exeFullPath.c_str());
		return -1;
	}
	const std::string destPath=tmp;

	std::list<std::string>::iterator iter=paths.begin();
	std::list<std::string> newPaths;
	for(;paths.end()!=iter;++iter)
	{
		std::string path=*iter;
		if(path.empty())
			continue;
		if('/'!=path[0])
		{
			path=curDir+path;
		}		
		if(tmp!=realpath(path.c_str(),tmp))
		{
			newPaths.push_back(*iter);
			continue;
		}
		if(destPath!=tmp)
			newPaths.push_back(*iter);
	}
	paths=newPaths;
	return 0;
}
int CEnv::init(int argc,char* argv[])
{
	int res=0;
	std::string exeFullPath;
	res=getExeFullPath(exeFullPath);
	cond_check_r(0==res,"getExeFullPath failed",-1);	
	std::list<std::string> paths;
	res=getPaths(paths);
	cond_check_r(0==res,"getPaths failed",-1);
	res=kickExePath(paths,exeFullPath);
	cond_check_r(0==res,"kickExePath failed",-1);
	res=setPaths(paths);
	cond_check_r(0==res,"setPaths failed",-1);

	return 0;
}
