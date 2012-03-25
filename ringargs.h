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

#ifndef _RINGARGS_H_INCLUDED_
#define _RINGARGS_H_INCLUDED_

#include <map>
#include <string>
#include <vector>

class CRingArgs
{
public:
	CRingArgs();
	virtual ~CRingArgs();	
public:
	int check(int argc,char* argv[]);
protected:
	typedef int (CRingArgs::*funcArgAction)();
	struct CInfo
	{
		const char* 	_tag1;//first argument passed to gccring
		const char* 	_tag2;//send argument if needed
		size_t		    _argCount;//include the argv[0]
		funcArgAction 	_func;//function according to the args
		const char* 	_help;//help string
	};
protected:
	int help();
	int enable();
	int disable();
	int compiler_add();
	int compiler_del();
	int compiler_list();
	int output_list();
	int output_refine();
	int output_clear();
protected:
	const static CInfo _infos[];
	std::vector<std::string> _args;
};

#endif//_RINGARGS_H_INCLUDED_