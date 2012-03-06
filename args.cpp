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
#include "args.h"
#include "public.h"
#include "cfg.h"
#include "trace.h"

CArgs::CArgs()
{
	_needCollect=false;
	_oriArgs=NULL;
	_newArgs=NULL;
	_objFilePos=-1;
}
CArgs::~CArgs()
{
	if(NULL!=_oriArgs)
	{
		char** args=_oriArgs;
		while(NULL!=*args)
		{
			delete *args;
			++args;
		}
		delete _oriArgs;
		_oriArgs=NULL;
	}
	if(NULL!=_newArgs)
	{
		char** args=_newArgs;
		while(NULL!=*args)
		{
			delete *args;
			++args;
		}
		delete _newArgs;
		_newArgs=NULL;
	}
}
int CArgs::init(int argc,char* argv[])
{
	cond_check_r(NULL==_oriArgs,"already inited",-1);
	cond_check_r(0<argc,"invalid argc",-1);

	_oriArgs=new char*[argc+1];
	memset(_oriArgs,0,(argc+1)*sizeof(char*));

	
	_newArgs=new char*[argc+2];
	memset(_newArgs,0,(argc+2)*sizeof(char*));
	_newArgs[argc]=new char[PATH_MAX];
	memset(_newArgs[argc],0,PATH_MAX);
	std::string mTag;
	if(CCfg::_includeSysHeader)
		mTag="-M";
	else
		mTag="-MM";
	strcpy(_newArgs[argc],mTag.c_str());

	bool oFound=false;
	for(int i=0;argc>i;++i)
	{
		int len=strlen(argv[i]);
		bool ignore=false;

		checkMacros(argv[i]);
		//忽略类似指令 -Wp,-MD,scripts/mod/.empty.o.d
		ignore=NULL!=strstr(argv[i],"-MD");
		if(!ignore)
		{
			ignore=NULL!=strstr(argv[i],"-MMD");
		}

		_oriArgs[i]=new char[PATH_MAX];
		memset(_oriArgs[i],0,PATH_MAX);
		memcpy(_oriArgs[i],argv[i],len);

		_newArgs[i]=new char[PATH_MAX];
		memset(_newArgs[i],0,PATH_MAX);
		if(!ignore)
			memcpy(_newArgs[i],argv[i],len);
		else
			strcpy(_newArgs[i],mTag.c_str());	

		if(oFound && -1==_objFilePos)
		{
			_objFilePos=i;
			oFound=false;
		}

		if(0==strcmp("-c",_oriArgs[i]))
			_needCollect=true;
		else if(0==strcmp("-o",_oriArgs[i]))
		{
			oFound=true;
		}

	}
	return 0;
}
int CArgs::needCollect(bool& nc)
{
	nc=_needCollect;
	return 0;
}
int CArgs::getOriCmd(char**& args)
{
	args=_oriArgs;
	return 0;
}
std::string CArgs::getObjFileName()
{
	if(NULL==_oriArgs || -1==_objFilePos)
		return NULL;
	return _oriArgs[_objFilePos];
}
int CArgs::createNewCmd(char**& args,std::string newObjFileName)
{
	cond_check_r(NULL!=_newArgs && 0<_objFilePos,"invalid state",-1);
	strncpy(_newArgs[_objFilePos],newObjFileName.c_str(),PATH_MAX);
	args=_newArgs;
	return 0;
}
int CArgs::checkMacros(const char* arg)
{
	cond_check_r(NULL!=arg,"checkMacros failed",-1);
	int len=strlen(arg);
	//必须要有"-D"再加一个字符
	if(3>len)
		return 0;

	if('-'!=arg[0] || 'D'!=arg[1])
		return 0;

	const char* key=arg+2;
	const char* value=strchr(key,'=');//处理-DKEY=VALUE
	cond_check_r(key!=value,"-D= like argument found",-2);

	//-DKEY 这样的定义
	if(NULL==value)
	{	
		_macros[key]="";//后面的覆盖前面的
		return 0;
	}
	//-DKEY=VALUE这样的定义

	std::string strKey;
	strKey.assign(key,value-key);
	++value;
	_macros[strKey]=value;//后面的覆盖前面的
	return 0;
}
int CArgs::getMacros(std::map<std::string,std::string>& macros)
{
	macros=_macros;
	return 0;
}
