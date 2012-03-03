//Gccring - utils.h
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

#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

namespace utils
{
char* dirname(const char* path);
char* basename(const char* path);
int cp(const char* src, const char* dst);
int rm(const char* src);
int createPathByFileName(std::string fileName);
int createPathByDirName(std::string dirName);
}

#endif//_UTILS_H_INCLUDED_
