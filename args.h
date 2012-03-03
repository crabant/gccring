//Gccring - args.h
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

#ifndef _ARGS_H_INCLUDED_
#define _ARGS_H_INCLUDED_

#include <map>
#include <string>
#include <vector>

class CArgs
{
public:
	CArgs();
	virtual ~CArgs();
public:
	int init(int argc,char* argv[]);
	int needCollect(bool& nc);
	int getOriCmd(char**& args);
	std::string getObjFileName();
	int createNewCmd(char**& args,std::string newObjFileName);
	int getMacros(std::map<std::string,std::string>& macros);
protected:
	int checkMacros(const char* arg);
protected:
	char**			_oriArgs;
	char**			_newArgs;
	int				_objFilePos;
	std::map<std::string,std::string> _macros;
	bool			_needCollect;//是否需要收集，根据参数中是否有-c决定
};

#endif//_ARGS_H_INCLUDED_
