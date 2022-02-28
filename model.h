#pragma once
#include "model.h"
#ifndef MODEL_H
#define MODEL_H

#include<time.h>

//����Ϣ�ṹ��
//char aName[18]-����
//char aPwd[8]-������
//int nStatus-��״̬��0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ��
//time_t tSart-����ʱ��
//time_t tEnd-���Ľ�ֹʱ��
//float fTotaluse-�ۼƽ��
//time_t tLastTime-���ʹ��ʱ��
//int nUseCount-�ۼ�ʹ�ô���
//float fBalance-���
//int Privilge-Ȩ�޵ȼ���1-�û���2-����Ա��
//int nDel-ɾ����ʶ��0-δɾ����1-��ɾ����
typedef struct Card
{
	char aName[18];		//����
	char aPwd[8];		//������
	int nStatus;		//��״̬��0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ��
	time_t tSart;		//����ʱ��
	time_t tEnd;		//���Ľ�ֹʱ��
	float fTotaluse;	//�ۼƽ��
	time_t tLastTime;	//���ʹ��ʱ��
	int nUseCount;		//�ۼ�ʹ�ô���
	float fBalance;		//���
	int Privilge;		//Ȩ�޵ȼ���1-�û���2-����Ա��
	int nDel;			//ɾ����ʶ��0-δɾ����1-��ɾ����
}Card;

//��������

//������ CardNode-������ͷ����β�� lpCardNode-���
//Card data-������
//struct CardNode* next-ָ����ָ����һ���ڵ㣩
typedef struct CardNode
{
	Card data;
	struct CardNode* next;

}CardNode, * lpCardNode;

//������Ϣ�ṹ��
//char aCardName[18]-����
//char aCardPwd[8]-����
//time_t tStart-�ϻ�ʱ��
//time_t tEnd-�»�ʱ��
//float fAmount-���ѽ��
//float fBalance-�����
//int nStatus-����״̬��0-δ����,1-�Ѿ�����
//int nDel-ɾ����ʶ��0-δɾ����1-��ɾ��
typedef struct Billing
{
	char aCardName[18];		//����
	char aCardPwd[8];		//����
	time_t tStart;			//�ϻ�ʱ��
	time_t tEnd;			//�»�ʱ��
	float fAmount;			//���ѽ��
	float fBalance;			//�����
	int nStatus;			//����״̬��0-δ����,1-�Ѿ�����
	int nDel;				//ɾ����ʶ��0-δɾ����1-��ɾ��
}Billing;

//�ϻ���Ϣ�ṹ��
//char aCardName[18]-�ϻ�����
//time_t tLogon-�ϻ�ʱ��
//float fBalance-�ϻ�ǰ�����
typedef struct LogonInfo
{
	char aCardName[18];		//�ϻ�����
	time_t tLogon;			//�ϻ�ʱ��
	float fBalance;			//�ϻ�ǰ�����
}LogonInfo;

//�Ʒ���Ϣ���
//��Ϣ������
//��Ϣָ����
//BillingNode-(��ͷ,��β),lpBillingNode-���
typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;

}BillingNode,*lpBillingNode;

//�»���Ϣ�ṹ��
//char aCardName[18];	//����
//time_t tStart;		//�ϻ�ʱ��
//time_t tEnd;			//�»�ʱ��
//float fAmount;		//���ѽ��
//float fBalance;		//���
typedef struct SettleInfo
{
	char aCardName[18];		//����
	time_t tStart;			//�ϻ�ʱ��
	time_t tEnd;			//�»�ʱ��
	float fAmount;			//���ѽ��
	float fBalance;			//���

}SettleInfo;

//��ֵ�˷ѽṹ��
//char CardName[18];	//����
//float fMoney;		//��ֵ�˷ѽ��
//float fBalance;		//���
typedef struct MoneyInfo
{
	char CardName[18];			//����
	float fMoney;				//��ֵ�˷ѽ��
	float fBalance;				//���

}MoneyInfo;

//ע���ṹ��
//char CardName[18];	//ע��������
//float fMoney;			//��ʣ��Ľ��
typedef struct LogoutInfo
{
	char CardName[18];		//ע��������
	float fMoney;		//��ʣ��Ľ��

}LogoutInfo;

#endif // !MODEL_H


