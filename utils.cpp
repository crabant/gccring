//Gccring - utills.cpp          
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

#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <map>
#include <string>
#include <vector>
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

#include "utils.h"
#include "public.h"

namespace utils
{

char* dirname(const char* path)
{
	cond_check_r(NULL!=path,"",NULL);
	static char buf[PATH_MAX];
	memset(buf,0,PATH_MAX);
	strncpy(buf,path,PATH_MAX);
	return ::dirname(buf);
}
char* basename(const char* path)
{
	cond_check_r(NULL!=path,"",NULL);
	static char buf[PATH_MAX];
	memset(buf,0,PATH_MAX);
	strncpy(buf,path,PATH_MAX);
	return ::basename(buf);
}

int cp(const char* src, const char* dst)
{
	cond_check_r(NULL!=src,"src is NULL",-1);
	cond_check_r(NULL!=dst,"dst is NULL",-2);

	char* args[4];
	memset(args,0,sizeof(args));
	args[0]=(char*)"cp";
	args[1]=(char*)src;
	args[2]=(char*)dst;
	args[3]=NULL;
	int pid=fork();
	cond_check_r(0<=pid,"fork failed",-3);
	if(0==pid)
	{
		execvp(args[0],args);
		printf("child process execvp failed\n");
		exit(-1);
	}
	int status;
	waitpid(pid,&status,0);
	int childRetCode=WEXITSTATUS(status);
	cond_check_r(0==childRetCode,"cp files failed",-4);
	return 0;
}
int rm(const char* src)
{
	cond_check_r(NULL!=src,"src is NULL",-1);

	char* args[3];
	memset(args,0,sizeof(args));
	args[0]=(char*)"rm";
	args[1]=(char*)src;
	args[2]=NULL;
	int pid=fork();
	cond_check_r(0<=pid,"fork failed",-2);
	if(0==pid)
	{
		execvp(args[0],args);
		printf("child process execvp failed\n");
		exit(-1);
	}
	int status;
	waitpid(pid,&status,0);
	int childRetCode=WEXITSTATUS(status);
	cond_check_r(0==childRetCode,"rm file failed",-3);
	return 0;
}
int createPathByFileName(std::string fileName)
{
	char* dirName=utils::dirname(fileName.c_str());
	cond_check_r(NULL!=dirName,"utils::dirname failed",-1);
	int res=createPathByDirName(dirName);
	cond_check_r(0==res,"createPathByDirName failed",-2);
	return 0;
}
int createPathByDirName(std::string dirName)
{
	struct stat st;
	int res;

	res=stat(dirName.c_str(),&st);
	if(0!=res)
	{
		std::stringstream ss;
		ss<<"mkdir -p ";
		ss<<dirName;
		res=system(ss.str().c_str());
		cond_check_r(0==res,"mkdir dirName failed",-1);
	}
	else
	{
		cond_check_r(S_ISDIR(st.st_mode),"dirName is not a directory",-2);
	}
	return 0;
}
}
