#pragma once
#include "model.h"
#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H

int addBilling(Billing billing);												//�������������Ϣ����
int getBilling();																//������ȡ������Ϣ����
int dosettle(const char* pName, const char* pPwd, SettleInfo* pSettleInfo);		//�����»�����
int dorefund(const char* pName, char* pPwd, MoneyInfo* pRefundInfo);			//�����˷Ѻ���
int dologout(const char* pName, char* pPwd, LogoutInfo* pLogoutInfo);			//����ע������

#endif // !BILLING_SERVICE_H