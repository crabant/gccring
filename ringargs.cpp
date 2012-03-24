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
#include "ringargs.h"
#include "public.h"
#include "cfg.h"
#include "trace.h"

const CRingArgs::CInfo CRingArgs::_infos[]=
{
	{"help","",&CRingArgs::help,"help\n\t\t\t:show this help\n"},
	{"enable","",&CRingArgs::enable,"enable\n\t\t\t:enable gccring redirect output function\n"},
	{"disable","",&CRingArgs::disable,"disable\n\t\t\t:disable gccring redirect output function\n"},
	{"compiler","add",&CRingArgs::compiler_add,"compiler add COMPILER_COMMAND\n\t\t\t:add a compiler alias\n"},
	{"compiler","del",&CRingArgs::compiler_del,"compiler del COMPILER_COMMAND\n\t\t\t:delete a compiler alias\n"},
	{"compiler","list",&CRingArgs::compiler_list,"compiler list\n\t\t\t:list current compiler aliases\n"},
	{"output","list",&CRingArgs::output_list,"output list\n\t\t\t:list output files\n"},
	{"output","refine",&CRingArgs::output_refine,"output refine\n\t\t\t:refine the output list file\n"},
	{"output","clear",&CRingArgs::output_clear,"output clear\n\t\t\t:clear all output files\n"},
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
			printf("%s:%d\n",__FILE__,__LINE__);
			if(0==*info._tag2)
			{
				func=info._func;
				break;
			}
			else 
			{
				if(_args[2]==info._tag2)
				{
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
	return 0;
}
int CRingArgs::disable()
{
	return 0;
}
int CRingArgs::compiler_add()
{
	return 0;
}
int CRingArgs::compiler_del()
{
	return 0;
}
int CRingArgs::compiler_list()
{
	return 0;
}
int CRingArgs::output_list()
{
	return 0;
}
int CRingArgs::output_refine()
{
	return 0;
}
int CRingArgs::output_clear()
{
	return 0;
}


