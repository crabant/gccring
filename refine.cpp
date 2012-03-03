//Gccring - refine.cpp
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
#include <set>
#include <string>

#include "cfg.h"
#include "utils.h"
#include "public.h"
#include "refine.h"

CRefine::CRefine()
{
}
CRefine::~CRefine()
{
}
int CRefine::refine()
{
	int res;
	res=refineFile(CCfg::_macroFileName.c_str());
	cond_check_r(0==res,"refine macro file failed",-1);
	res=refineFile(CCfg::_oriListFileName.c_str());
	cond_check_r(0==res,"refine original list file failed",-2);
	res=refineFile(CCfg::_newListFileName.c_str());
	cond_check_r(0==res,"refine new list file failed",-3);
	return 0;
}
int CRefine::refineMacroFile()
{
	return -1;
}
int CRefine::refineListFile()
{
	return -1;
}
int CRefine::refineFile(const char* fileName)
{
	cond_check_r(NULL!=fileName,"refineFile failed,fileName is NULL",-1);
	int fd=open(fileName,O_RDONLY);
	cond_check_r(0<fd,"refineFile failed,open for read failed",-2);
	struct stat st;
	int res=fstat(fd,&st);
	cond_check_r(0==res,"refineFile failed,stat failed",-3);
	const int len=st.st_size;

	void* hBuf=mmap(NULL,len,PROT_READ,MAP_SHARED,fd,0);	
	close(fd);
	cond_check_r(NULL!=hBuf,"refineFile failed,mmap failed",-4);

	std::set<std::string> items;
	char* begin=(char*)hBuf;
	char* end=(char*)hBuf;
	bool inEnd=false;
	for(int i=0;len>i;++i)
	{
		if('\r'==*end || '\n'==*end)
		{
			inEnd=true;
		}
		else if(inEnd)
		{
			inEnd=false;
			if(begin!=end)
			{
				std::string str;
				str.assign(begin,end-begin);
				if(items.end()==items.find(str))
					items.insert(str);
			}
			begin=end;
		}
		++end;
	}
	if(end>begin)
	{
		std::string str;
		str.assign(begin,end-begin);
		if(items.end()==items.find(str))
			items.insert(str);
	}
	munmap(hBuf,len);

	fd=open(fileName,O_WRONLY|O_CREAT|O_TRUNC,0666);
	cond_check_r(0<fd,"refineFile failed,open for write failed",-5);

	std::set<std::string>::iterator iter=items.begin();
	for(;items.end()!=iter;++iter)
	{
		std::string item=*iter;
		size_t writeLen=write(fd,item.c_str(),item.length());
		cond_check_r(item.length()==writeLen,"refineFile failed,write failed",-6);
	}
	close(fd);

	return 0;
}
