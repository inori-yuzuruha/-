#pragma once
#include "model.h"
#include "global.h"
#ifndef BILLING_FILE_H
#define BILLING_FILE_H

int saveBilling(const Billing* pBilling, const char* pPath);					//��������������Ϣ����
int readBillings(Billing* pBilling, const char* pPath);							//������ȡ������Ϣ����
int getBillingCount(const char* pPath);											//�����������ļ��ж�ȡ������Ϣ�����ĺ���
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex);		//�������������ļ���������Ϣ����

#endif // !BILLING_FILE_H