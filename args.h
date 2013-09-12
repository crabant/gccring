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

/**
 * @file args.h
 * @brief ��CArgs��ͷ�ļ�
 * 
 */
 

#ifndef _ARGS_H_INCLUDED_
#define _ARGS_H_INCLUDED_

#include <map>
#include <string>
#include <vector>


/**
* @brief �����������в�������
* @details ������ǰgcc �ı�����������ڵ�ǰ�������������ļ��ı������
*/
class CArgs
{
public:
	CArgs();
	virtual ~CArgs();
public:
	/**
	* �������� 
	* @param[in] argc �����������ʹ���main������argc��ͬ����
	* @param[in] argv �������ݣ��ʹ���main������argv��ͬ����
	* @return �����Ƿ�ɹ�
	* @retval 0 �ɹ�
	* @retval !0 ʧ��
	*/
	int parse(int argc,char* argv[]);

	/**
	* ��ǰ�����Ƿ�֧���ļ���Ϣ�ռ�
	* @return ��ʾ�Ƿ�֧���ļ���Ϣ�ռ�
	* @retval true ֧��
	* @retval false ��֧��
	*/
	bool getNeedCollect();
	/**
	* ��ȡԭʼ���������
	* @param[out]  args ԭ���������в���
	* @return ��ȡ�Ƿ�ɹ�
	* @retval 0 �ɹ�
	* @retval !0 ʧ��
	*/
	int getOriCmd(char**& args);
	/**
	* ȡ�ñ�������Ҫ������Ŀ���ļ�������
	* @return �ļ�����
	*/
	std::string getObjFileName();
	/**
	* �������еı���������µ�Ŀ���ļ����������ռ������ı�������
	* @param[in] newObjFileName �µ�Ŀ���ļ�
	* @param[out] args �µı�������
	* @return ��ȡ�Ƿ�ɹ�
	* @retval 0 �ɹ�
	* @retval !0 ʧ��
	*/
	int createNewCmd(std::string newObjFileName,char**& args);
	/**
	* ��ȡ���������еĺ궨��
	* @param[out] macros �궨����ɵ�map��keyΪ�������,valueΪ���ֵ
	* @return ��ȡ�Ƿ�ɹ�
	* @retval 0 �ɹ�
	* @retval !0 ʧ��
	*/
	int getMacros(std::map<std::string,std::string>& macros);
protected:
	/**
	* �Ӳ����н����궨��
	* @param[in] arg ��ǰ����
	* @return �Ƿ�������궨��
	* @retval 0 �ɹ�
	* @retval !0 ʧ��
	*/
	int parseMacro(const char* arg);
protected:
	char**			_oriArgs; ///< �洢ԭʼ�ı�������
	char**			_newArgs; ///< �洢�µı�������
	int				_objFilePos; ///< obj�ļ���ԭʼ�������������е�����
	std::map<std::string,std::string> _macros; ///<�洢���������ĺ궨��
	bool			_needCollect; ///<�Ƿ���Ҫ������Ϣ�ռ�������gcc�� "-c" ����ȡֵ
};

#endif//_ARGS_H_INCLUDED_
