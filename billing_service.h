#pragma once
#include "model.h"
#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H

int addBilling(Billing billing);												//声明添加消费信息函数
int getBilling();																//声明获取消费信息函数
int dosettle(const char* pName, const char* pPwd, SettleInfo* pSettleInfo);		//声明下机函数
int dorefund(const char* pName, char* pPwd, MoneyInfo* pRefundInfo);			//声明退费函数
int dologout(const char* pName, char* pPwd, LogoutInfo* pLogoutInfo);			//声明注销函数

#endif // !BILLING_SERVICE_H