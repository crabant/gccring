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
#include <sstream>

#include "env.h"
#include "public.h"
#include "utils.h"
#include "defines.h"

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
int CEnv::kickExePath(std::list<std::string>& paths,std::string arg0)
{
	std::string curDir=get_current_dir_name();
		

	std::list<std::string>::iterator iter;
	std::list<std::string> newPaths;
	bool link;
	for(iter=paths.begin();paths.end()!=iter;++iter)
	{
		std::string path=*iter;
		if(path.empty())
			continue;
		std::stringstream ss;
		ss<<path<<"/"<<arg0;
		if(0==utils::isLink(GCCRING_BIN_FILE,ss.str().c_str(),link))
		{
			if(link)
				continue;
		}
		newPaths.push_back(path);
	}
	paths=newPaths;
	return 0;
}
int CEnv::init(int argc,char* argv[])
{
	int res=0;
	cond_check_r(0<argc,"invalid argc",-1);
	std::list<std::string> paths;
	res=getPaths(paths);
	cond_check_r(0==res,"getPaths failed",-2);
	res=kickExePath(paths,argv[0]);
	cond_check_r(0==res,"kickExePath failed",-3);
	res=setPaths(paths);
	cond_check_r(0==res,"setPaths failed",-4);

	return 0;
}
