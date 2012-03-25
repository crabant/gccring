//Gccring - env.h
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

#ifndef _ENV_H_INCLDED_
#define _ENV_H_INCLDED_

#include <list>
#include <string>

class CEnv
{
public:
	int init(int argc,char* argv[]);
protected:
	int getStartProgramFullPath(char* arg0,std::string& path);
	int kickExePath(std::list<std::string>& paths,std::string arg0);
	int getPaths(std::list<std::string>& paths);//读取环境变量PATH到list
	int setPaths(std::list<std::string>& paths);//使用list中变量设置PATH
};

#endif//_ENV_H_INCLDED_
