#pragma once
#include <ctime>
#ifndef Tool_H
#define Tool_H

void timeToString(time_t t, char* pBuf);		//声明将时间由time_t类型转换为字符串类型函数
time_t stringToTime(char* pTime);				//声明将时间由字符串类型转换为time_t类型函数

#endif // !Tool_H
