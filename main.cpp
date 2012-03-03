//Gccring - main.cpp
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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "public.h"
#include "env.h"
#include "args.h"
#include "collect.h"
#include "utils.h"
#include "cfg.h"
#include "refine.h"
#include "trace.h"

int main_show_help()
{
	std::string str;
	str+=
		"gccring --help\n"
		"                :show this help\n"
		"gccring --refine\n"
		"                :refine the list file\n"
		"gcc --gccring-help\n"
		"                :show this help\n"
		"gcc --gccring-refine\n"
		"                :refine the list file\n"
		"\n";
	str+=CCfg::help();
	printf("%s",str.c_str());
	return 0;
}

int main_do_refine()
{
	CRefine refine;
	refine.refine();
	return 0;
}

void help(int argc,char* argv[])
{
	bool gccring= 0<=argc && NULL!=strstr(argv[0],"gccring");
	bool show_help=false;
	bool do_refine=false;
	for(int i=1;argc>i;++i)
	{
		if(gccring)
		{
			if(0==strcmp("--help",argv[i]))
			{
				show_help=true;
				break;
			}
			if(0==strcmp("--refine",argv[i]))
			{
				do_refine=true;
				break;
			}
		}
		if(0==strcmp("--gccring-help",argv[i]))
		{
			show_help=true;
			break;
			
		}
		if(0==strcmp("--gccring-refine",argv[i]))
		{
			do_refine=true;
			break;
			
		}
	}
	if(show_help)
	{
		main_show_help();
		exit(0);
	}
	if(do_refine)
	{
		main_do_refine();
		exit(0);
	}
	if(gccring)
	{
		main_show_help();
		exit(0);
	}
}

int main(int argc,char* argv[])
{	
	int res=0;
	CCfg::init();

	help(argc,argv);

	CTrace::add("new request");
	CTrace::add(argc,argv);

	CEnv env;
	env.init(argc,argv);

	CArgs args;
	args.init(argc,argv);

	CCollect collect;
	collect.init(argc,argv);

	bool needCollect;
	args.needCollect(needCollect);
	
	//是编译操作
	if(needCollect)
	{
		//获取obj文件名称
		std::string oldFile=args.getObjFileName();
		std::string newFile;

		//生成收集信息目录
		res=collect.prepareObjFile(oldFile,newFile);
		cond_check_r(0==res,"createObjFilePath failed",-4);
		char** newArgs=NULL;

		//拿到新的gcc命令，进行临时编译，获取obj文件对应的所有头文件，源文件信息
		res=args.createNewCmd(newArgs,newFile);
		cond_check_r(0==res,"getOriCmd failed",-1);
		cond_check_r(NULL!=newArgs,"oriArgs is NULL",-2);
		cond_check_r(NULL!=newArgs[0],"args[0] is NULL",-3);

		//执行新的gcc命令
		int pid=fork();
		cond_check_r(0<=pid,"fork failed",-4);
		if(0==pid)
		{
			execvp(newArgs[0],newArgs);
			printf("child process execvp failed\n");
			exit(-1);
		}
		int status;
		waitpid(pid,&status,0);
		int childRetCode=WEXITSTATUS(status);
		if(0!=childRetCode)
			return childRetCode;

		//复制obj文件依赖的所有文件
		res=collect.copyFiles(newFile);
		cond_check_r(0==res,"collect copy files failed",-4);

		utils::rm(newFile.c_str());
		
		std::map<std::string,std::string> macros;
		res=args.getMacros(macros);
		cond_check_r(0==res,"getMacros failed",-5);
		res=collect.addMacros(macros);
		cond_check_r(0==res,"collect addMacros failed",-6);
	}

	//执行原来的编译操作
	{
		char** oriArgs=NULL;
		res=args.getOriCmd(oriArgs);
		cond_check_r(0==res,"getOriCmd failed",-1);
		cond_check_r(NULL!=oriArgs,"oriArgs is NULL",-2);
		cond_check_r(NULL!=oriArgs[0],"args[0] is NULL",-3);
		execvp(oriArgs[0],oriArgs);
		printf("execvp failed\n");
		exit(-1);
	}
	return 0;
}
