//Gccring - cfg.cpp          
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

#define INCLUDE_SYSHEADER_ENV "GCCRING_INCLUDE_SYSHEADER"
#define INCLUDE_SYSHEADER_DEFAULT "1"

#define ROOT_PATH_ENV  "GCCRING_ROOTPATH"
#define ROOT_PATH_DEFAULT "/tmp/ring"

#define MACRO_FILENAME_ENV "GCCRING_MACRO_FILENAME"
#define MACRO_FILENAME_DEFAULT "compile_macros.h"


#define ORI_LIST_FILENAME_ENV "GCCRING_ORI_LIST_FILENAME"
#define ORI_LIST_FILENAME_DEFAULT "compile_ori_list.txt"

#define ORI_LIST_PREFIX_ENV "GCCRING_ORI_LIST_PREFIX"
#define ORI_LIST_PREFIX_DEFAULT ""

#define ORI_LIST_FRONT_REMOVE_ENV "GCCRING_ORI_LIST_FRONT_REMOVE"
#define ORI_LIST_FRONT_REMOVE_DEFAULT ""

#define NEW_LIST_FILENAME_ENV "GCCRING_NEW_LIST_FILENAME"
#define NEW_LIST_FILENAME_DEFAULT "compile_new_list.txt"

#define NEW_LIST_PREFIX_ENV "GCCRING_NEW_LIST_PREFIX"
#define NEW_LIST_PREFIX_DEFAULT ""

#define NEW_LIST_FRONT_REMOVE_ENV "GCCRING_NEW_LIST_FRONT_REMOVE"
#define NEW_LIST_FRONT_REMOVE_DEFAULT ""

#define TRACE_FILE_ENV "GCCRING_TRACE_FILE"
#define TRACE_FILE_DEFAULT ""

#define CHANGE_DIR_CHAR_ENV "GCCRING_CHANGE_DIR_CHAR"
#define CHANGE_DIR_CHAR_DEFAULT "0"

bool CCfg::_includeSysHeader=true;
std::string CCfg::_rootPath=ROOT_PATH_DEFAULT;
std::string CCfg::_macroFileName=MACRO_FILENAME_DEFAULT;
std::string CCfg::_oriListFileName=ORI_LIST_FILENAME_DEFAULT;
std::string CCfg::_oriListPrefix=ORI_LIST_PREFIX_DEFAULT;
std::string CCfg::_oriListFrontRemove=ORI_LIST_FRONT_REMOVE_DEFAULT;
std::string CCfg::_newListFileName=NEW_LIST_FILENAME_DEFAULT;
std::string CCfg::_newListPrefix=NEW_LIST_PREFIX_DEFAULT;
std::string CCfg::_newListFrontRemove=NEW_LIST_FRONT_REMOVE_DEFAULT;
std::string CCfg::_traceFileName=TRACE_FILE_DEFAULT;
bool CCfg::_changeDirChar=false;
void CCfg::init()
{
	char* tmp;

	tmp=getenv(INCLUDE_SYSHEADER_ENV);
	if(NULL!=tmp)
	{
		if(0==strcmp("0",tmp))
			_includeSysHeader=false;
		else if(0==strcmp("1",tmp))
			_includeSysHeader=true;
	}

	tmp=getenv(ROOT_PATH_ENV);
	if(NULL!=tmp)
		_rootPath=tmp;

	tmp=getenv(MACRO_FILENAME_ENV);
	if(NULL!=tmp)
		_macroFileName=tmp;
	_macroFileName=_rootPath+"/"+_macroFileName;

	
	tmp=getenv(ORI_LIST_FILENAME_ENV);
	if(NULL!=tmp)
		_oriListFileName=tmp;
	_oriListFileName=_rootPath+"/"+_oriListFileName;

	tmp=getenv(ORI_LIST_PREFIX_ENV);
	if(NULL!=tmp)
		_oriListPrefix=tmp;

	
	tmp=getenv(ORI_LIST_FRONT_REMOVE_ENV);
	if(NULL!=tmp)
		_oriListFrontRemove=tmp;

	tmp=getenv(NEW_LIST_FILENAME_ENV);
	if(NULL!=tmp)
		_newListFileName=tmp;
	_newListFileName=_rootPath+"/"+_newListFileName;

	tmp=getenv(NEW_LIST_PREFIX_ENV);
	if(NULL!=tmp)
		_newListPrefix=tmp;
	
	tmp=getenv(NEW_LIST_FRONT_REMOVE_ENV);
	if(NULL!=tmp)
		_newListFrontRemove=tmp;
	
	tmp=getenv(TRACE_FILE_ENV);
	if(NULL!=tmp)
		_traceFileName=tmp;

	tmp=getenv(CHANGE_DIR_CHAR_ENV);
	if(NULL!=tmp)
	{
		if(0==strcmp("0",tmp))
			_changeDirChar=false;
		else if(0==strcmp("1",tmp))
			_changeDirChar=true;
	}
}
std::string CCfg::help()
{
	std::string ret;
	ret="Environment Settings:\n"
		INCLUDE_SYSHEADER_ENV":whether include system header files\n"
		"      default:"INCLUDE_SYSHEADER_DEFAULT"\n"
		"            0:do not include system header files\n"
		"            1:include system header files\n"
		"       others:use default\n"
		ROOT_PATH_ENV":path to store the gccring result\n"
		"     default:"ROOT_PATH_DEFAULT"\n"
		MACRO_FILENAME_ENV":contain the macro passed to compile tool\n"
		"     default:"MACRO_FILENAME_DEFAULT"\n"
		ORI_LIST_FILENAME_ENV":contains all files that are built,include macro file\n"
		"     default:"ORI_LIST_FILENAME_DEFAULT"\n"
		ORI_LIST_PREFIX_ENV":add the prefix string to each line of original list file\n"
		"     default:"ORI_LIST_PREFIX_DEFAULT"\n"
		ORI_LIST_FRONT_REMOVE_ENV":remove the the string if existence in front of the file name\n"
		"     default:"ORI_LIST_FRONT_REMOVE_DEFAULT"\n"
		NEW_LIST_FILENAME_ENV":contains all files copied to new locations,include macro file\n"
		"     default:"NEW_LIST_FILENAME_DEFAULT"\n"
		NEW_LIST_PREFIX_ENV":add the prefix string to each line of new list file\n"
		"     default:"NEW_LIST_PREFIX_DEFAULT"\n"	
		NEW_LIST_FRONT_REMOVE_ENV":remove the the string if existence in front of the file name\n"
		"     default:"NEW_LIST_FRONT_REMOVE_DEFAULT"\n"	
		TRACE_FILE_ENV":trace to the file,do not trace if filename is NULL\n"
		"     default:"TRACE_FILE_DEFAULT"\n"
		CHANGE_DIR_CHAR_ENV":whether replace the dir charcter from '/' to '\\' \n"
		"      default:"CHANGE_DIR_CHAR_DEFAULT"\n"
		"            0:do not change\n"
		"            1:change\n"
		"       others:use default\n"	
		;
	return ret;
}
