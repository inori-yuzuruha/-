#pragma once
#include <ctime>
#ifndef Tool_H
#define Tool_H

void timeToString(time_t t, char* pBuf);		//������ʱ����time_t����ת��Ϊ�ַ������ͺ���
time_t stringToTime(char* pTime);				//������ʱ�����ַ�������ת��Ϊtime_t���ͺ���

#endif // !Tool_H
