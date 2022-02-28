#pragma once
#include "model.h"
#ifndef CARD_FILE_H
#define CARD_FILE_H

int saveCard(const Card* pCard, const char* pPath);								//�������溯��
int getCardCount(const char* pPath);											//������ȡ�ļ��п���������
int readCard(Card* pCard, const char* pPath);									//������ȡ���ļ��еĿ���Ϣ����
int updateCard(const Card* pCard, const char* pPath, int nIndex);				//�������¿��ļ��п���Ϣ����
Card praseCard(char* pBuf);														//�����������ļ��п���Ϣ����
int rechargeCard(const Card*, const char* pPath, int nIndex);					//������ֵ�˷�ʱ���¿��ļ��п���Ϣ����
int rechargeBilling(const Billing* pBilling, const char* pPath, int nIndex);	//������ֵ�˷�ʱ���������ļ��п���Ϣ����

#endif // !GLOBAL_H
