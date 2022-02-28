#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "model.h"
#include"card_service.h"
#include"tool.h"
#include"global.h"
#include "card_service.h"
#include "service.h"
#include "billing_service.h"
#pragma warning( disable : 4996 )
int getsize(const char* pInfor);

int getsize(const char* pInfor)
{
	int nSize = 0;
	char* pinput = NULL;
	while (*(pInfor + nSize) != '\0')
	{
		nSize++;
	}
	return nSize;
}
/****************************************************************************************
[������]��outputMenu_m
[����]���������Աϵͳ�˵�
[����]��void
[����ֵ]��void
*****************************************************************************************/
void outputMenu_m(void)
{
	//���ϵͳ�˵�
	printf("------�˵�------\n");
	printf("1.��ӿ�\n");
	printf("2.��ѯ��\n");
	printf("3.�ϻ�\n");
	printf("4.�»�\n");
	printf("5.��ֵ\n");
	printf("6.�˷�\n");
	printf("7.��ѯͳ��\n");
	printf("8.ע����\n");
	printf("0.�˳�\n");
	printf("��ѡ��˵�����(0-8):");//��ʾ�û�����˵�
}
/****************************************************************************************
[������]��outputMenu_m
[����]������û�ϵͳ�˵�
[����]��void
[����ֵ]��void
*****************************************************************************************/
void outputMenu_u(void)
{
	//���ϵͳ�˵�
	printf("------�˵�------\n");
	printf("1.��ѯ\n");
	printf("2.�ϻ�\n");
	printf("3.�»�\n");
	printf("4.ע����\n");
	printf("0.�˳�\n");
	printf("��ѡ��˵�����(0-4):");//��ʾ�û�����˵�
}
/****************************************************************************************
[������]��add
[����]����ӿ�
[����]��void
[����ֵ]��void
*****************************************************************************************/
void add()
{
	Card card;
	Billing billing;
	char aName[32] = { '\0' };			//��ʱ��¼����
	char aPwd[20] = { '\0' };			//��ʱ��������
	int nNameSize, nPwsSize;
	struct tm* startTime;				//����ʱ��
	struct tm* endTime;					//��ֹʱ��
	Card* pCard = NULL;
	int nIndex = 0;
	FILE* fp = NULL;
	int privilige = 0;					//��Ȩ��

	printf("------��ӿ�------\n");		//��ǰ״̬��ʾ

	//��ʾ���벢���տ���

	if (fopen(CARDPATH,"r")==NULL)
	{
		printf("�����ÿ���(1-18���ַ�)��");
		scanf_s("%s", aName, sizeof(aName));
	}
	else
	{
		printf("�����ÿ���(1-18���ַ�)��");
		while (1)
		{
			fflush(stdin);
			scanf_s("%s", aName, sizeof(aName));
			nIndex = 0;
			pCard = queryCards(aName, &nIndex);
			if (pCard != NULL)
			{
				printf("�����Ѵ��ڣ����������룺");
			}
			else
			{
				break;
			}
		}
	}

	//�жϿ��ų���
	nNameSize = getsize(aName);
	if (nNameSize > 18)
	{
		printf("����ĳ��ȳ����涨!\n");
		return;
	}

	//�����ű��浽�ṹ����
	strcpy_s(card.aName, aName);
	//��ʾ���벢��������
	printf("\n���������루1-8���ַ�����");
	scanf_s("%s", aPwd, sizeof(aPwd));

	//�ж����볤��
	nPwsSize = getsize(aPwd);
	if (nPwsSize > 8)
	{
		printf("����ĳ��ȳ����涨!\n");
		return;
	}

	//�����뱣�浽���ṹ����
	strcpy_s(card.aPwd, aPwd);

	printf("\n������Ȩ��(1-�û���2-����Ա)��");
	scanf_s("%d", &privilige);

	//��ʾ���벢���տ������
	printf("\n�����뿪�����(RMB)��");
	scanf_s("%f", &card.fBalance);
	card.fBalance = card.fBalance;					//�����
	card.fTotaluse = 0;								//����ʹ��ʱ��
	card.nUseCount = 0;								//�ϻ�����
	card.nDel = 0;									//ɾ����ʶ
	card.nStatus = 0;								//��״̬
	card.Privilge = privilige;						//��Ȩ�޵ȼ�

	card.tSart = card.tEnd = card.tLastTime = time(NULL);	//��ʼ����������Ч�����������ʹ��ʱ��

														//��ƿ�����Ч�ڣ���ʼΪ1�꣩
	startTime = localtime(&card.tSart);
	endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;

	//�������Ч��ת��Ϊtime_t�ĸ�ʽ�����浽����Ϣ�ṹ����
	card.tEnd = mktime(endTime);

	//����������Ϣ
	strcpy(billing.aCardName , card.aName);
	strcpy(billing.aCardPwd, card.aPwd);
	billing.fAmount = 0;
	billing.fBalance = card.fBalance;
	billing.nDel = 0;
	billing.nStatus = 1;
	billing.tEnd = card.tLastTime;
	billing.tStart = 0;


	if ((FALSE == addCard(card))|| (FALSE == addBilling(billing)))
	{
		printf("��ӿ���Ϣʧ��\n");
	}
	else
	{
		printf("---��ӵĿ���Ϣ����:---\n");
		printf("����\t����\t��״̬\t���\n%s\t%s\t%d\t%.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
	}
}
/****************************************************************************************
[������]��query
[����]����ѯ��
[����]��void
[����ֵ]��void
*****************************************************************************************/
void query()
{
	int nIndex = 0, i;
	Card* pCard = NULL;
	char aTime[20] = { '0' };
	char aName[18] = { '0' };
	printf("��������Ҫ��ѯ�Ŀ��ţ�");
	scanf_s("%s", aName, sizeof(aName));

	//����queryCard�������������ѯ
	pCard = queryCards(aName, &nIndex);

	if ((pCard != NULL) || (nIndex != 0))
	{
		printf("��ѯ���Ŀ���Ϣ���£�\n");
		printf("����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");

		for (i = 0; i < nIndex; i++)
		{
			//��ʱ��ת��Ϊ��ʽΪ����-��-�� ʱ:�� �ĸ�ʽ��
			timeToString(pCard[i].tLastTime, aTime);

			//��ʾ
			printf("%s\t%d\t%.1f\t%d\t\t", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotaluse);
			printf("%d\t\t%s\n",pCard[i].nUseCount ,aTime);

		}


	}
	else
	{
		printf("δ��ѯ�������Ϣ�������¼�����Ĳ�ѯ�ؼ��ʣ�\n");
	}
	//�ͷŶ�̬������ڴ�
	free(pCard);
	pCard = NULL;

}
/****************************************************************************************
[������]��logon
[����]���ϻ�
[����]��void
[����ֵ]��void
*****************************************************************************************/
void logon()
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	Card* pCard = NULL;									//�����ļ��ж�ȡ�����ݴ�ŵ�����
	char LastTime[TINMELENGTH] = { 0 };
	LogonInfo* pInfo = NULL;
	int nResult = 0;
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));		//���ڱ����ϻ����Ŀ��ţ������ϻ�ʱ��

	//�����û�����Ŀ��ź�����
	printf("�����뿨�ţ�");
	scanf_s("%s", &aName,sizeof(aName));
	printf("���������룺");
	scanf_s("%s", &aPwd,sizeof(aPwd));


	//�����ϻ��������ʾ��ͬ��Ϣ
	nResult= dologon(aName, aPwd,pInfo);

	switch (nResult)
	{
	case 0:
		printf("�ϻ�ʧ�ܣ�\n");
		break;
	case 1:
		printf("�ϻ��ɹ���\n");
		printf_s("����\t���\t\t�ϻ�ʱ��\n");
		timeToString(pInfo->tLogon, LastTime);
		printf_s("%s\t%0.1f\t\t%s\n", pInfo->aCardName, pInfo->fBalance, LastTime);
		break;
	case 2:
		printf("�ÿ�����ʹ�ã�\n");
		break;
	case 3:
		printf("�ÿ������㣡\n");
		break;
	}
	//�ͷ��ϻ���Ϣ
	free(pInfo);
}
/****************************************************************************************
[������]��settle
[����]���»�
[����]��void
[����ֵ]��void
*****************************************************************************************/
void settle()
{

	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nResult = 0;
	char tStart[20] = { 0 };
	char tEnd[20] = { 0 };
	SettleInfo* pInfo = NULL;							//���ڱ����»����Ŀ��ţ��ϻ�ʱ�䣬�»�ʱ�䣬���ѽ������
	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	//��ʾ�û������»�����
	printf("�������»�������:");

	//�����»�����
	scanf_s("%s", aName, 18);

	//��ʾ�û������»�����
	printf("�������»�������:");

	//�����»�����
	scanf_s("%s", aPwd, 8);

	//�����»��������ʾ��ͬ��Ϣ
	nResult = dosettle(aName, aPwd, pInfo);

	switch (nResult)
	{
	case 0:
		printf_s("��������»�ʧ��\n");
		break;
	case 1:
		printf_s("�»��ɹ�\n");
		printf_s("����\t���ѽ��\t���\t\t�ϻ�ʱ��\t\t�»�ʱ��\n");

		timeToString(pInfo->tStart, tStart);
		timeToString(pInfo->tEnd, tEnd);

		printf_s("%s\t%.1f\t\t%.1f\t\t%s\t%s\n", aName, pInfo->fAmount, pInfo->fBalance, tStart, tEnd);
		break;
	case 2:
		printf("���»����޷��»���\n");
		break;
	case 3:
		printf("���㣬�޷��»�!\n");
		break;

}

	//�ͷ��»���Ϣ
	free(pInfo);
}
/****************************************************************************************
[������]��rechage
[����]����ֵ
[����]����
[����ֵ]����
*****************************************************************************************/
void rechage()
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	float money;
	int nIndex;
	Card* pCard = NULL;
	int nResult = 0;
	MoneyInfo* pInfo = NULL;

	pInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));

	printf("�������ֵ�Ŀ��ţ�");
	while (1)
	{
		scanf_s("%s", aName,sizeof(aName));
		nIndex = 0;
		pCard = queryCards(aName, &nIndex);
		if (pCard == NULL)
		{
			printf("���Ų����ڣ����������룺");
		}
		else
		{

			if ((pCard->nStatus == 2) || (pCard->nStatus == 3))
			{
				printf("�ÿ���ע������ʧЧ���޷���ֵ�������¼�鿨�ţ�");
			}
			else
			{
				break;
			}
		}
	}

	printf("��������Ҫ��ֵ�Ľ�");
	scanf_s("%f", &money);

	//Ϊ�ļ��еĿ���Ϣ���г�ֵ
	nResult = dorecharge(aName, money, pInfo);
	switch (nResult)
	{
	case 0:
		printf("��ֵʧ�ܣ�");
		break;
	case 1:
		printf("��ֵ�ɹ���\n");
		printf("����\t��ֵ���\t���\n");
		printf("%s\t%.1f\t\t%.1f\n", pInfo->CardName, pInfo->fMoney, pInfo->fBalance);
	}

}
/****************************************************************************************
[������]��refund
[����]���˷�
[����]����
[����ֵ]����
*****************************************************************************************/
void refund()
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nIndex;
	Card* pCard = NULL;
	int nResult = 0;
	MoneyInfo* pInfo = NULL;

	pInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));

	printf("�������˷ѵĿ��ţ�");
	while (1)
	{
		scanf_s("%s", aName, sizeof(aName));
		scanf_s("%*[^\n]%*c");
		nIndex = 0;
		pCard = queryCards(aName, &nIndex);
		if (pCard == NULL)
		{
			printf("���Ų����ڣ����������룺");
		}
		else
		{

			if ((pCard->nStatus == 2) || (pCard->nStatus == 3))
			{
				printf("�ÿ���ע������ʧЧ���޷���ֵ�������¼�鿨�ţ�");
			}
			else
			{
				break;
			}
		}
	}

	printf("�����뿨���룺");
	scanf_s("%f", aPwd);

	getchar();

	//Ϊ�ļ��еĿ���Ϣ�����˷�
	nResult = dorefund(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0:
		printf("��������˷�ʧ�ܣ�");
		break;
	case 1:
		printf("�˷ѳɹ���\n");
		printf("����\t�˷ѽ��\t���\n");
		printf("%s\t%.1f\t\t%.1f\n", pInfo->CardName, pInfo->fMoney, pInfo->fBalance);
		break;
	case 2:
		printf("���㣬�˷�ʧ�ܣ�");
		break;

	}
}
/****************************************************************************************
[������]��logout
[����]��ע����
[����]����
[����ֵ]����
*****************************************************************************************/
void logout()
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nIndex = 0;
	int nResult = 0;
	Card* pCard = NULL;
	LogoutInfo* pInfo = NULL;

	pInfo = (LogoutInfo*)malloc(sizeof(LogoutInfo));

	//��ʾ�û�����Ҫע���Ŀ�����
	printf("������Ҫע���Ŀ��ţ�");

	while (1)
	{
		fflush(stdin);
		scanf_s("%s", aName, sizeof(aName));
		nIndex = 0;
		pCard = queryCards(aName, &nIndex);
		if (pCard == NULL)
		{
			printf("���Ų����ڣ����������룺");
		}
		else
		{
			break;
		}
	}

	//��ʾ�û�����Ҫע���Ŀ�������
	printf("������Ҫע���Ŀ������룺");

	scanf_s("%s", aPwd, sizeof(aPwd));

	//���scanf_s������
	scanf_s("%*[^\n]%*c");

	//��λ��ע����
	nResult = dologout(aName, aPwd, pInfo);;

	//���ݷ�����Ϣ�ж�ע�����
	switch (nResult)
	{
	case 0:
		printf("�����������\n");
		break;
	case 1:
		printf("ע���ɹ���\n");
		printf("����\t�˷ѽ��\n");
		printf("%s\t%.1f\n", pInfo->CardName, pInfo->fMoney);
		break;
	case 2:
		printf("�����ϻ���\n");
		break;
	case 3:
		printf("��ע����\n");
		break;
	case 4:
		printf("���㣬ע��ʧ�ܣ�\n");
		break;
	}

}
/****************************************************************************************
[������]��exitApp
[����]���˳�Ӧ�ó���
[����]��void
[����ֵ]��void
*****************************************************************************************/
void exitApp()
{
	releaseList();
}
