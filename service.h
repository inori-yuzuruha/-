#pragma once

#ifndef SERVICE_H
#define SERVICE_H

int dologon(const char* aName, const char* aPwd, LogonInfo* pLogonInfo);		//声明上机函数
void releaseCardList();															//声明释放链表结点函数
void releaseList();																//声明释放链表结点函数
double getAmount(time_t t);														//声明计算消费金额函数

#endif // SERVICE_H