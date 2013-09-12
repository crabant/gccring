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
 * @brief 类CArgs的头文件
 * 
 */
 

#ifndef _ARGS_H_INCLUDED_
#define _ARGS_H_INCLUDED_

#include <map>
#include <string>
#include <vector>


/**
* @brief 编译器命令行参数处理
* @details 分析当前gcc 的编译参数，基于当前参数生成依赖文件的编译参数
*/
class CArgs
{
public:
	CArgs();
	virtual ~CArgs();
public:
	/**
	* 解析参数 
	* @param[in] argc 参数个数，和传给main函数的argc相同意义
	* @param[in] argv 参数内容，和传给main函数的argv相同意义
	* @return 解析是否成功
	* @retval 0 成功
	* @retval !0 失败
	*/
	int parse(int argc,char* argv[]);

	/**
	* 当前参数是否支持文件信息收集
	* @return 表示是否支持文件信息收集
	* @retval true 支持
	* @retval false 不支持
	*/
	bool getNeedCollect();
	/**
	* 获取原始的命令参数
	* @param[out]  args 原来的命令行参数
	* @return 获取是否成功
	* @retval 0 成功
	* @retval !0 失败
	*/
	int getOriCmd(char**& args);
	/**
	* 取得编译命令要生产的目标文件的名称
	* @return 文件名称
	*/
	std::string getObjFileName();
	/**
	* 根据现有的编译命令和新的目标文件生产进行收集操作的编译命令
	* @param[in] newObjFileName 新的目标文件
	* @param[out] args 新的编译命令
	* @return 获取是否成功
	* @retval 0 成功
	* @retval !0 失败
	*/
	int createNewCmd(std::string newObjFileName,char**& args);
	/**
	* 获取编译命令中的宏定义
	* @param[out] macros 宏定义组成的map，key为宏的名称,value为宏的值
	* @return 获取是否成功
	* @retval 0 成功
	* @retval !0 失败
	*/
	int getMacros(std::map<std::string,std::string>& macros);
protected:
	/**
	* 从参数中解析宏定义
	* @param[in] arg 当前参数
	* @return 是否解析出宏定义
	* @retval 0 成功
	* @retval !0 失败
	*/
	int parseMacro(const char* arg);
protected:
	char**			_oriArgs; ///< 存储原始的编译命令
	char**			_newArgs; ///< 存储新的编译命令
	int				_objFilePos; ///< obj文件在原始编译命令数组中的索引
	std::map<std::string,std::string> _macros; ///<存储解析出来的宏定义
	bool			_needCollect; ///<是否需要进行信息收集，根据gcc的 "-c" 参数取值
};

#endif//_ARGS_H_INCLUDED_
