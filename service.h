#pragma once

#ifndef SERVICE_H
#define SERVICE_H

int dologon(const char* aName, const char* aPwd, LogonInfo* pLogonInfo);		//�����ϻ�����
void releaseCardList();															//�����ͷ������㺯��
void releaseList();																//�����ͷ������㺯��
double getAmount(time_t t);														//�����������ѽ���

#endif // SERVICE_H