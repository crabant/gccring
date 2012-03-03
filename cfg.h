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
	static bool _includeSysHeader;			//��������ʱ���Ƿ����ϵͳͷ�ļ�
	static std::string _rootPath;			//�����ļ��洢�ĸ�Ŀ¼
	static std::string _macroFileName;		//�����м����ɵĺ��ļ�����	

	static std::string _oriListFileName;	//��������ԭʼ�����ļ��嵥
	static std::string _oriListPrefix;		//���ɵ�ԭʼ�����ļ��嵥�У�ÿ���ļ����ӵ�ǰ׺
	static std::string _oriListFrontRemove;//����ļ�������ǰ�������������Ҫȥ��
	
	static std::string _newListFileName;	//�������������ļ��ͺ��ļ����ļ��嵥
	static std::string _newListPrefix;		//���ɵ��ļ��嵥�У�ÿ���ļ����ӵ�ǰ׺
	static std::string _newListFrontRemove;//����ļ�������ǰ�������������Ҫȥ��

	static std::string _traceFileName;		//gccring���м��������ļ���������
	static bool _changeDirChar;				//�Ƿ�ı�Ŀ¼��'/'Ϊ'\'
};

#endif//_CFG_H_INCLUDED_
