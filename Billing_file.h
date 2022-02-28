#pragma once
#include "model.h"
#include "global.h"
#ifndef BILLING_FILE_H
#define BILLING_FILE_H

int saveBilling(const Billing* pBilling, const char* pPath);					//声明保存消费信息函数
int readBillings(Billing* pBilling, const char* pPath);							//声明读取消费信息函数
int getBillingCount(const char* pPath);											//声明从消费文件中读取消费信息数量的函数
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex);		//声明更新消费文件中消费信息函数

#endif // !BILLING_FILE_H