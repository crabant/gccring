//Gccring - public.h
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

#ifndef PUBLIC_H_INCLUDED
#define PUBLIC_H_INCLUDED


#define cond_check_r(cond,msg,ret)	\
	do { if(!(cond)){ printf(#cond" failed,%s:%d,%s\n",__FILE__,__LINE__,(msg)); return(ret);} } while (0)

#endif//PUBLIC_H_INCLUDED
