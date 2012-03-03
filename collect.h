//Gccring - collect.h         
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

#ifndef _COLLECT_H_INCLUDED_
#define _COLLECT_H_INCLUDED_

#include <map>
#include <string>
#include <vector>

class CCollect
{
public:
	CCollect();
	virtual ~CCollect();
public:
	int init(int argc,char* argv[]);
	int prepareObjFile(std::string oldFile,std::string& newFile);
	int copyFiles(std::string newObjFile);
	int addMacros(std::map<std::string,std::string>& macros);
protected:
	int addOriListFile(std::string fileName);//增加原始文件到list文件
	int addNewListFile(std::string fileName);//增加收集到的文件名称到list文件
	int copyFile(std::string fileName);
};

#endif//_COLLECT_H_INCLUDED_
