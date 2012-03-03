//Gccring - cfg.h          
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

#ifndef _CFG_H_INCLUDED_
#define _CFG_H_INCLUDED_

#include <string>
class CCfg
{
public:
	static void init();
	static std::string help();
public:
	static bool _includeSysHeader;			//生成依赖时候，是否包括系统头文件
	static std::string _rootPath;			//依赖文件存储的根目录
	static std::string _macroFileName;		//编译中间生成的宏文件名称	

	static std::string _oriListFileName;	//包含所有原始依赖文件清单
	static std::string _oriListPrefix;		//生成的原始依赖文件清单中，每个文件名加的前缀
	static std::string _oriListFrontRemove;//如果文件名中最前面和这个相符，则要去掉
	
	static std::string _newListFileName;	//包含所有依赖文件和宏文件的文件清单
	static std::string _newListPrefix;		//生成的文件清单中，每个文件名加的前缀
	static std::string _newListFrontRemove;//如果文件名中最前面和这个相符，则要去掉

	static std::string _traceFileName;		//gccring，中间输出结果文件，调试用
	static bool _changeDirChar;				//是否改变目录的'/'为'\'
};

#endif//_CFG_H_INCLUDED_
