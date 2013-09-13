//Gccring - args.cpp          
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

#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
#include "ringargs.h"
#include "public.h"
#include "cfg.h"
#include "trace.h"
#include "defines.h"
#include "utils.h"
#include <list>
#include "refine.h"

const CRingArgs::CInfo CRingArgs::_infos[]=
{
	{"--help",	"",			2,	&CRingArgs::help,			"help\n\t\t\t:show this help\n"},
	{"help",	"",			2,	&CRingArgs::help,			"help\n\t\t\t:show this help\n"},
	{"enable",	"",			2,	&CRingArgs::enable,			"enable\n\t\t\t:enable gccring redirect output function\n"},
	{"disable",	"",			2,	&CRingArgs::disable,		"disable\n\t\t\t:disable gccring redirect output function\n"},
	{"make",	"",			2,	&CRingArgs::make,			"make\n\t\t\t:do make with gccring\n"},
	{"compiler","add",		4,	&CRingArgs::compiler_add,	"compiler add COMPILER_COMMAND\n\t\t\t:add a compiler alias\n"},
	{"compiler","del",		4,	&CRingArgs::compiler_del,	"compiler del COMPILER_COMMAND\n\t\t\t:delete a compiler alias\n"},
	{"compiler","list",		3,	&CRingArgs::compiler_list,	"compiler list\n\t\t\t:list current compiler aliases\n"},
	{"output",	"list",		3,	&CRingArgs::output_list,	"output list\n\t\t\t:list output files\n"},
	{"output",	"refine",	3,	&CRingArgs::output_refine,	"output refine\n\t\t\t:refine the output list file\n"},
	{"output",	"clear",	3,	&CRingArgs::output_clear,	"output clear\n\t\t\t:clear all output files\n"},
};


CRingArgs::CRingArgs()
{
}
CRingArgs::~CRingArgs()
{
}
int CRingArgs::check(int argc,char* argv[])
{
	for(int i=0;argc>i;++i)
		_args.push_back(argv[i]);
	if(2>_args.size())
		return 0;

	const std::string& arg0=_args[0];
	if(arg0.npos==arg0.find("gccring"))
		return 0;
	funcArgAction func=NULL;
	for(size_t index=0;sizeof(_infos)/sizeof(CInfo)>index;++index)
	{
		const CInfo& info=_infos[index];
		if(_args[1]==info._tag1)
		{
			if(0==*info._tag2)
			{
				if(info._argCount!=_args.size())
				{
					printf("invalid arg count\n");
					exit(1);
				}
				func=info._func;
				break;
			}
			else 
			{
				if(_args[2]==info._tag2)
				{					
					if(info._argCount!=_args.size())
					{
						printf("invalid arg count\n");
						exit(1);
					}
					func=info._func;
					break;
				}
			}
		}
	}
	if(NULL!=func)
	{
		(this->*func)();
		exit(0);
	}
	return 0;
}
int CRingArgs::help()
{
	cond_check_r(1<=_args.size(),"arg[0] not found",-1);
	std::string str;
	for(size_t index=0;sizeof(_infos)/sizeof(CInfo)>index;++index)
	{
	str+=_args[0];
	str+=" ";
	str+=_infos[index]._help;
	}
	printf("%s",str.c_str());
	return 0;
}
int CRingArgs::enable()
{
	std::list<std::string> paths;
	char* env=getenv("PATH");
	if(NULL==env)
	{
		printf("could not get PATH environment variable\n");
		return -1;
	}
	std::vector<char> strEnv;
	int len=strlen(env);
	strEnv.resize(len+1);
	memcpy(&strEnv[0],env,len);
	char* path=strtok(&strEnv[0],":");
	while(NULL!=path)
	{
		char* absPath=realpath(path,NULL);
		if(NULL!=absPath)
		{
			if(0!=strcmp(CFG_CMD_PATH,absPath))
				paths.push_back(path);
			else
			{
				if(paths.empty())//CFG_CMD_PATH is at frist position
				{
					printf("already enabled\n");
					return 0;
				}
			}
		}
		else
			paths.push_back(path);
		path=strtok(NULL,":");
	}
	std::list<std::string>::iterator iter;
	std::stringstream ss;
	ss<<"please run folling command manually:\n";
	ss<<"export PATH=\""<<CFG_CMD_PATH;
	for(iter=paths.begin();paths.end()!=iter;++iter)
	{
		ss<<":"<<*iter;
	}
	ss<<"\"";
	printf("%s\n",ss.str().c_str());
	return 0;
}
int CRingArgs::disable()
{
	std::list<std::string> paths;
	char* env=getenv("PATH");
	if(NULL==env)
	{
		printf("could not get PATH environment variable\n");
		return -1;
	}
	std::vector<char> strEnv;
	int len=strlen(env);
	strEnv.resize(len+1);
	memcpy(&strEnv[0],env,len);
	char* path=strtok(&strEnv[0],":");
	bool found=false;
	while(NULL!=path)
	{
		char* absPath=realpath(path,NULL);
		if(NULL!=absPath)
		{
			if(0!=strcmp(CFG_CMD_PATH,absPath))
				paths.push_back(path);
			else
			{
				found=true;
			}
		}
		else
			paths.push_back(path);
		path=strtok(NULL,":");
	}
	if(!found)
	{
		printf("already disabled\n");
		return 0;
	}
	std::list<std::string>::iterator iter;
	std::stringstream ss;
	ss<<"please run folling command manually:\n";
	ss<<"export PATH=\"";
	for(iter=paths.begin();paths.end()!=iter;++iter)
	{
		if(paths.begin()!=iter)
			ss<<":";
		ss<<*iter;
	}
	ss<<"\"";
	printf("%s\n",ss.str().c_str());
	return 0;
}
int CRingArgs::make()
{
	std::string path;
	int ret=get_paths_enable_gccring(path);
	if(0!=ret)
	{
		fprintf(stderr,"get PATH with gccring enabled failed,ret=%d",ret);
		return -1;
	}
	ret=setenv("PATH",(char*)path.c_str(),1);
	if(0!=ret)
	{
		fprintf(stderr,"putenv failed,ret=%d\n",ret);
		return -1;
	}
	fprintf(stdout,"new env is %s",getenv("PATH"));
	system("make");
	return 0;
}

int CRingArgs::compiler_add()
{	
	std::stringstream ss;
	ss<<CFG_CMD_PATH<<"/"<<_args[3];
	bool link;
	if(0==utils::isLink(GCCRING_BIN_FILE,ss.str().c_str(),link))
	{
		if(link)
		{
			printf("ok,not need created\n");
			return 0;
		}
	}
	if(0==symlink(GCCRING_BIN_FILE,ss.str().c_str()))
	{
		printf("added ok\n");
	}
	else
	{
		printf("added failed,sudo is used?\n");
		return -1;
	}
	return 0;
}
int CRingArgs::compiler_del()
{
	std::stringstream ss;
	ss<<CFG_CMD_PATH<<"/"<<_args[3];
	bool link;
	if(0==utils::isLink(GCCRING_BIN_FILE,ss.str().c_str(),link))
	{
		if(link)
		{
			if(0==unlink(ss.str().c_str()))
			{
				printf("deleted ok\n");				
			}
			else
			{
				printf("deleted failed\n");
				return -1;
			}			
			return 0;
		}
	}
	printf("%s not a gccring compiler,sudo is used?\n",_args[3].c_str());
	return 0;
}
int CRingArgs::compiler_list()
{
	DIR* dir=opendir(CFG_CMD_PATH);
	cond_check_r(NULL!=dir,"open cmd dir failed",-1);
	struct dirent* entry=readdir(dir);

	bool link;
	while(NULL!=entry)
	{
		std::stringstream ss;
		ss<<CFG_CMD_PATH<<"/"<<entry->d_name;
		if(0==utils::isLink(GCCRING_BIN_FILE,ss.str().c_str(),link))
		{
			if(link)
				printf("%s\n",entry->d_name);
		}
		entry=readdir(dir);
	}
	closedir(dir);
	return 0;
}
int CRingArgs::output_list()
{
	DIR* dir=opendir(CCfg::_rootPath.c_str());
	cond_check_r(NULL!=dir,"open output directory failed",-1);
	struct dirent* entry=readdir(dir);
	std::list<std::string> dirs;
	std::list<std::string> files;
	while(NULL!=entry)
	{
		std::stringstream ss;
		ss<<CCfg::_rootPath<<"/"<<entry->d_name;
		if('.'!=entry->d_name[0])
		{
			struct stat st;		
			if(0==stat(ss.str().c_str(),&st))
			{
				if(S_ISREG(st.st_mode))
					files.push_back(entry->d_name);
				else if(S_ISDIR(st.st_mode))
					dirs.push_back(entry->d_name);
			}
		}
		entry=readdir(dir);
	}
	closedir(dir);

	std::list<std::string>::iterator iter;
	printf("%u directories:\n",dirs.size());
	for(iter=dirs.begin();dirs.end()!=iter;++iter)
	{
		printf("\t%s\n",(*iter).c_str());
	}
	printf("%u files:\n",files.size());
	for(iter=files.begin();files.end()!=iter;++iter)
	{
		printf("\t%s\n",(*iter).c_str());
	}
	//
	return 0;
}
int CRingArgs::output_refine()
{	
	CRefine refine;
	refine.refine();
	return 0;
}
int CRingArgs::output_clear()
{
	printf("please rm the directory \"%s\" manually\n",CCfg::_rootPath.c_str());
	return 0;
}

int CRingArgs::get_paths_enable_gccring(std::string& pathStr)
{
	std::list<std::string> paths;
	char* env=getenv("PATH");
	if(NULL==env)
	{
		printf("could not get PATH environment variable\n");
		return -1;
	}
	std::vector<char> strEnv;
	int len=strlen(env);
	strEnv.resize(len+1);
	memcpy(&strEnv[0],env,len);
	char* path=strtok(&strEnv[0],":");
	while(NULL!=path)
	{
		char* absPath=realpath(path,NULL);
		if(NULL!=absPath)
		{
			if(0!=strcmp(CFG_CMD_PATH,absPath))
				paths.push_back(path);
			else
			{
				if(paths.empty())//CFG_CMD_PATH is at frist position
				{
					printf("already enabled\n");
					return 0;
				}
			}
		}
		else
			paths.push_back(path);
		path=strtok(NULL,":");
	}
	std::list<std::string>::iterator iter;
	std::stringstream ss;
	//ss<<"PATH=\"";
	ss<<CFG_CMD_PATH;
	for(iter=paths.begin();paths.end()!=iter;++iter)
	{
		ss<<":"<<*iter;
	}
	ss<<"\"";
	pathStr=ss.str();
	return 0;
}



