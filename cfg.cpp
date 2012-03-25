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
#include <libconfig.h>
#include <iostream>

#include "cfg.h"
#include "utils.h"
#include "public.h"
#include "defines.h"

#define INCLUDE_SYSHEADER_ENV "include_sys_header"
#define INCLUDE_SYSHEADER_DEFAULT true

#define ROOT_PATH_ENV  "rootpath"
#define ROOT_PATH_DEFAULT "/tmp/gccring"

#define MACRO_FILENAME_ENV "macro_filename"
#define MACRO_FILENAME_DEFAULT "compile_macros.h"


#define ORI_LIST_FILENAME_ENV "orilist/filename"
#define ORI_LIST_FILENAME_DEFAULT "orilist.txt"

#define ORI_LIST_PREFIX_ENV "orilist/prefix"
#define ORI_LIST_PREFIX_DEFAULT ""

#define ORI_LIST_FRONT_REMOVE_ENV "orilist/front_removed"
#define ORI_LIST_FRONT_REMOVE_DEFAULT ""

#define NEW_LIST_FILENAME_ENV "newlist/filename"
#define NEW_LIST_FILENAME_DEFAULT "newlist.txt"

#define NEW_LIST_PREFIX_ENV "newlist/prefix"
#define NEW_LIST_PREFIX_DEFAULT ""

#define NEW_LIST_FRONT_REMOVE_ENV "newlist/front_removed"
#define NEW_LIST_FRONT_REMOVE_DEFAULT ""

#define TRACE_FILE_ENV "trace_file"
#define TRACE_FILE_DEFAULT ""

#define CHANGE_DIR_CHAR_ENV "change_dir_char"
#define CHANGE_DIR_CHAR_DEFAULT true

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
	config_t cfg;
	const char *str;
	int value;
  
	config_init(&cfg);
	if(! config_read_file(&cfg, CFG_FILE_NAME))
	{
		fprintf(stderr, "%d - %s\n", config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return;
	}

	if(config_lookup_bool(&cfg,INCLUDE_SYSHEADER_ENV,&value))
		_includeSysHeader=value;
	else
		_includeSysHeader=INCLUDE_SYSHEADER_DEFAULT;
		
	if(config_lookup_string(&cfg,ROOT_PATH_ENV,&str))
		_rootPath=str;
	else
		_rootPath=ROOT_PATH_DEFAULT;

	if(config_lookup_string(&cfg,MACRO_FILENAME_ENV,&str))
		_macroFileName=_rootPath+"/"+str;
	else
		_macroFileName=_rootPath+"/"+MACRO_FILENAME_DEFAULT;

	if(config_lookup_string(&cfg,ORI_LIST_FILENAME_ENV,&str))
		_oriListFileName=_rootPath+"/"+str;
	else
		_oriListFileName=_rootPath+"/"+ORI_LIST_FILENAME_DEFAULT;

	if(config_lookup_string(&cfg,ORI_LIST_PREFIX_ENV,&str))
		_oriListPrefix=str;
	else
		_oriListPrefix=ORI_LIST_PREFIX_DEFAULT;

	if(config_lookup_string(&cfg,ORI_LIST_FRONT_REMOVE_ENV,&str))
		_oriListFrontRemove=str;
	else
		_oriListFrontRemove=ORI_LIST_FRONT_REMOVE_DEFAULT;

	if(config_lookup_string(&cfg,NEW_LIST_FILENAME_ENV,&str))
		_newListFileName=_rootPath+"/"+str;
	else
		_newListFileName=_rootPath+"/"+NEW_LIST_FILENAME_DEFAULT;

	if(config_lookup_string(&cfg,NEW_LIST_PREFIX_ENV,&str))
		_newListPrefix=str;
	else
		_newListPrefix=NEW_LIST_PREFIX_DEFAULT;

	if(config_lookup_string(&cfg,NEW_LIST_FRONT_REMOVE_ENV,&str))
		_newListFrontRemove=str;
	else
		_newListFrontRemove=NEW_LIST_FRONT_REMOVE_DEFAULT;

	if(config_lookup_string(&cfg,TRACE_FILE_ENV,&str))
		_traceFileName=str;
	else
		_traceFileName=TRACE_FILE_DEFAULT;

	if(!_traceFileName.empty())
		_traceFileName=_rootPath+"/"+_traceFileName;
	
	if(config_lookup_bool(&cfg,CHANGE_DIR_CHAR_ENV,&value))
		_changeDirChar=value;
	else
		_changeDirChar=CHANGE_DIR_CHAR_DEFAULT;	
	 
  	config_destroy(&cfg);
  	
	return;
}
std::string CCfg::help()
{
	std::string ret;
	
	return ret;
}

