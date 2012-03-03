//Gccring - trace.cpp
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

#include "cfg.h"
#include "utils.h"
#include "public.h"
#include "trace.h"

int CTrace::add(int argc,char* argv[])
{
	if(!CCfg::_traceFileName.empty())
	{
		int res=utils::createPathByFileName(CCfg::_traceFileName);
		cond_check_r(0==res,"create trace file path failed",-1);
		int fd=open(CCfg::_traceFileName.c_str(),O_WRONLY|O_APPEND|O_CREAT,0666);
		cond_check_r(0<fd,"open trace file failed\n",-2);
		for(int i=0;argc>i;++i)
		{
			std::stringstream ss;
			ss<<"argv["<<i<<"]:"<<argv[i]<<"\n";
			write(fd,ss.str().c_str(),ss.str().length());
		}
		close(fd);
	}
	return 0;
}
int CTrace::add(std::string str)
{
	if(!CCfg::_traceFileName.empty())
	{
		int res=utils::createPathByFileName(CCfg::_traceFileName);
		cond_check_r(0==res,"create trace file path failed",-1);
		int fd=open(CCfg::_traceFileName.c_str(),O_WRONLY|O_APPEND|O_CREAT,0666);
		cond_check_r(0<fd,"open trace file failed\n",-2);
		std::stringstream ss;
		ss<<str<<"\n";
		write(fd,ss.str().c_str(),ss.str().length());
		close(fd);
	}
	return 0;
}
