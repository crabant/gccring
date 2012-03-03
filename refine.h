//Gccring - refine.h
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

#ifndef _REFIND_H_INCLUDED_
#define _REFIND_H_INCLUDED_

#include <string>
class CRefine
{
public:
	CRefine();
	virtual ~CRefine();
public:
	int refine();
protected:
	int refineMacroFile();
	int refineListFile();
	int refineFile(const char* fileName);
};

#endif//_REFIND_H_INCLUDED_
