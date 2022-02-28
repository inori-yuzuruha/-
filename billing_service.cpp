#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include "model.h"
#include "global.h"
#include "Billing_file.h"
#include "card_service.h"
#include "service.h"
#include "card_file.h"

int initCardList_b();
void releaseCardList_b();
int getCard_b();

lpCardNode cardList_b = NULL;			//������������ļ��е�������Ϣ����Ϣ����
lpCardNode headcard_b = NULL;			//������������ļ��е�������Ϣ����Ϣ�����ͷ

lpBillingNode billingList = NULL;		//������������ļ��е�������Ϣ��������Ϣ����
lpBillingNode headbilling = NULL;		//������������ļ��е�������Ϣ��������Ϣ�����ͷ


/****************************************************************************************
[������]��addBilling
[����]�����Ʒ���Ϣ���浽billion.mas�ļ���
[����]��Billing billing
[����ֵ]��TRUE-��ȷ���� FALSE-���󷵻�
*****************************************************************************************/
int addBilling(Billing billing)
{
	return saveBilling(&billing, BILLINGPATH);
}
/****************************************************************************************
[������]��initBillingList
[����]����ʼ������
[����]����
[����ֵ]����
*****************************************************************************************/
void initBillingList()
{
	lpBillingNode headbilling = NULL;
	if (billingList == NULL)
	{
		headbilling = (lpBillingNode)malloc(sizeof(BillingNode));
		headbilling->next = NULL;
		billingList = headbilling;
	}
}
/****************************************************************************************
[������]��releaseBillingList
[����]���ͷ�����
[����]����
[����ֵ]����
*****************************************************************************************/
void releaseBillingList()
{
	lpBillingNode cur = billingList;
	lpBillingNode next = NULL;

	while (cur != NULL)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	billingList = NULL;
}
/****************************************************************************************
[������]��getBilling
[����]����ȡ�����ļ��е�������Ϣ
[����]����
[����ֵ]����ȷִ�з���TRUE ����ִ�з���FALSE
*****************************************************************************************/
int getBilling()
{
	int nCount = 0;
	int i = 0;				//����forѭ������
	Billing* pBilling = NULL;
	lpBillingNode node = NULL;

	if (billingList != NULL)
	{
		releaseBillingList();
	}

	initBillingList();

	nCount = getBillingCount(BILLINGPATH);

	//��̬�����ڴ�
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling == NULL)
	{
		printf("�ڴ�����ʧ�ܣ�");
		return FALSE;
	}

	//��ȡ����Ϣ
	if (readBillings(pBilling, BILLINGPATH) == FALSE)
	{
		free(pBilling);
		pBilling = NULL;
		printf("��ȡ����Ϣʧ�ܣ�");
		return FALSE;
	}

	//������Ϣ���浽������
		for (i = 0, node = billingList; i < nCount; i++)
		{
			lpBillingNode cur = NULL;
			//Ϊ�������ڴ�
			cur = (lpBillingNode)malloc(sizeof(CardNode));

			if (cur == NULL)
			{
				free(pBilling);
				printf("�ڵ��ڴ����ʧ�ܣ�");
				return FALSE;
			}
			//��ʼ���µĿռ䣬ȫ����ֵΪ0
			memset(cur, 0, sizeof(CardNode));

			//������Ϣ���浽�����
			cur->data = pBilling[i];
			cur->next = NULL;

			//�������ӵ�����β��
			node->next = cur;
			node = cur;

		}
		free(pBilling);
		pBilling = NULL;

		return TRUE;

}

/****************************************************************************************
[������]��dologon
[����]���ϻ��������ϻ�����Ϣ
[����]��char* aName(�ϻ�������), char* aPwd(�ϻ�������), pLogonInfo* pLogonInfo(�ϻ���Ϣ�ṹ��������ʱ�����ϻ���Ϣ)
[����ֵ]��0-�������1-�ϻ��ɹ���2-�����ϻ���3-����
*****************************************************************************************/
int dologon(const char* aName, const char* aPwd, LogonInfo* pLogonInfo)
{
	lpBillingNode billingNode = NULL;
	lpCardNode cardNode = NULL;		//�½��ڵ㣬���ڽ��մ�getCard�д��ݶ�����������Ϣ�����ṹ����Ϣ��
	int nIndex_b = 0;
	int nIndex_c = 0;					//�ϻ������ļ��е�����
	Billing billing;

	//��ȡ����Ϣ
	if (FALSE == getCard_b())
	{
		return NULL;
	}
	cardNode = cardList_b->next;

	//��ȡ������Ϣ
	if (FALSE == getBilling())
	{
		return NULL;
	}
	billingNode = billingList->next;

	while (billingNode != NULL)
	{
		//�жϿ��������Ƿ���ȷ
		if ((strcmp(billingNode->data.aCardName, aName) == 0) && (strcmp(billingNode->data.aCardPwd, aPwd) == 0))
		{
			break;
		}
		billingNode = billingNode->next;
		nIndex_b++;
	}


	while (cardNode != NULL)		//���յ��Ŀ���Ϣ��Ϊ��
	{
		//�жϿ��������Ƿ���ȷ
		if ((strcmp(cardNode->data.aName, aName) == 0) && (strcmp(cardNode->data.aPwd, aPwd) == 0))
		{
			//�ж��ܷ�����ϻ�:�жϿ�״̬�Ϳ����
			if (cardNode->data.nStatus != 0)
			{
				return 2;						//�����ϻ�
			}
			if (cardNode->data.fBalance <= 0)
			{
				return 3;						//����
			}

			//�����ϻ����Ŀ���Ϣ:�ļ��������еĿ���Ϣ

			//�����������ϻ����Ŀ���Ϣ
			cardNode->data.nStatus = 1;
			cardNode->data.nUseCount++;
			cardNode->data.tLastTime = time(NULL);

			//�����ļ��еĿ���Ϣ
			if (updateCard(&cardNode->data, CARDPATH, nIndex_c) == TRUE)
			{
				//��װ�ϻ�ǰ����������Ϣ
				strcpy(billing.aCardName, aName);
				strcpy(billing.aCardPwd, aPwd);
				billing.tStart = time(NULL);
				billing.fAmount = 0;
				billing.fBalance = cardNode->data.fBalance;
				billing.nDel = 0;
				billing.tEnd = 0;
				billing.nStatus = 1;

				billingNode->data = billing;

				//����������Ϣ
				if (TRUE == updateBilling(&billingNode->data, BILLINGPATH, nIndex_b))
				{
					strcpy(pLogonInfo->aCardName, aName);				//�����ű��浽�ϻ����ṹ����
					pLogonInfo->fBalance = cardNode->data.fBalance;		//����ǰ���浽�ϻ����ṹ����
					pLogonInfo->tLogon = billing.tStart;				//���ϻ�ʱ�䱣�浽�ϻ����ṹ����
					//������Ӧ��Ϣ
					return 1;											//�ϻ��ɹ�
				}
			}
		}
		cardNode = cardNode->next;
		nIndex_c++;
	}
	return 0;


}

/****************************************************************************************
[������]��dosettle
[����]���»��������»�����Ϣ
[����]��char* aName(�»�������), char* aPwd(�»�������), pLogonInfo* pLogonInfo(�»���Ϣ�ṹ��������ʱ�����»���Ϣ)
[����ֵ]��0-�������1-�»��ɹ���2-���»���3-����
*****************************************************************************************/
int dosettle(const char* pName, const char* pPwd, SettleInfo* pSettleInfo)
{
	lpCardNode cardNode= NULL;
	lpBillingNode billingNode = NULL;	//�½��ڵ㣬���ڽ��մ�getBilling�д��ݶ�����������Ϣ�����ṹ����Ϣ��
	int nIndex_c = 0;
	int nIndex_b = 0;
	Billing billing;
	int nPosition = 0;
	float nAmount;

	//��ȡ����Ϣ
	if (FALSE == getCard_b())
	{
		return NULL;
	}
	cardNode = cardList_b->next;

	//��ȡ������Ϣ
	if (FALSE == getBilling())
	{
		return NULL;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)		//���յ��Ŀ���Ϣ��Ϊ��
	{
		while (billingNode != NULL)
		{
			//�жϿ��������Ƿ���ȷ
			if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd) == 0))
			{

				//�ж��ܷ�����»�:�жϿ�״̬�Ϳ����
				if (cardNode->data.nStatus != 1)
				{
					return 2;						//���»�
				}
				if (cardNode->data.fBalance <= 0)
				{
					return 3;						//����
				}

				//���汾�����ѽ��
				nAmount = getAmount(billingNode->data.tStart);

				//�����»����Ŀ���Ϣ:�ļ��������еĿ���Ϣ

				//����cardList�����е���Ϣ
				cardNode->data.nStatus = 0;
				cardNode->data.fTotaluse+=nAmount;
				cardNode->data.tLastTime = time(NULL);
				cardNode->data.fBalance -= nAmount;

				//����billingList�������»����Ŀ���Ϣ
				billingNode->data.nStatus = 1;
				billingNode->data.tEnd = cardNode->data.tLastTime;
				billingNode->data.fBalance -= nAmount;
				billingNode->data.fAmount = nAmount;

				//�����ļ��еĿ���Ϣ
				if (updateCard(&cardNode->data, CARDPATH, nIndex_c) == TRUE)
				{
					//��װ������Ϣ
					strcpy(billing.aCardName, cardNode->data.aName);
					strcpy(billing.aCardPwd , cardNode->data.aPwd);
					billing.tStart = billingNode->data.tStart;
					billing.nStatus = 1;
					billing.nDel = 0;
					billing.tEnd = cardNode->data.tLastTime;
					billing.fAmount = nAmount;
					billing.fBalance = cardNode->data.fBalance;

					//�����ļ��е�������Ϣ
					if (TRUE == updateBilling(&billing, BILLINGPATH, nIndex_b))
					{
						strcpy(pSettleInfo->aCardName, pName);				//�����ű��浽�»����ṹ����
						pSettleInfo->tStart = billingNode->data.tStart;		//���ϻ�ʱ�䱣�浽�»����ṹ����
						pSettleInfo->tEnd = billingNode->data.tEnd;			//���»�ʱ�䱣�浽�»����ṹ����
						pSettleInfo->fBalance = billingNode->data.fBalance;	//�����浽�»����ṹ����
						pSettleInfo->fAmount = billingNode->data.fAmount;	//�����ѽ��浽�»����ṹ����

						//���ر�ʶ��Ϣ,�»��ɹ�
						return 1;											//�»��ɹ�
					}
				}
			}
			cardNode = cardNode->next;
			nIndex_c++;
			billingNode = billingNode->next;
			nIndex_b++;
		}
	}
	//�����������
	return 0;
}

/****************************************************************************************
[������]��dorecharge
[����]����ֵ�����¿���Ϣ
[����]��char* aName(����ֵ����), float Money(��ֵ���), MoneyInfo* pLogonInfo()
[����ֵ]��0-��ʧЧ��1-��ֵ�ɹ�
*****************************************************************************************/
int dorecharge(const char* pName, float Money, MoneyInfo* pMoneyInfo)
{

	lpCardNode cardNode = NULL;
	lpBillingNode billingNode = NULL;
	int nIndex_c = 0;
	int nIndex_b = 0;
	Billing billing;
	//��ȡ���ļ���Ϣ
	if (FALSE == getCard_b())
	{
		printf("��ֵ��ȡ���ļ���Ϣʧ�ܣ�");
		return FALSE;
	}
	cardNode = cardList_b->next;			//���տ���Ϣ

	if (FALSE == getBilling())
	{
		printf("��ֵ��ȡ�����ļ���Ϣʧ�ܣ�");
		return FALSE;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)
	{
		if ((strcmp(cardNode->data.aName, pName)) == 0)
		{
			//����������Ϣ
			cardNode->data.fBalance += Money;
			billing = billingNode->data;
			billingNode->data.fBalance = cardNode->data.fBalance;

			//����Ϣ���浽Card.txt�ļ�
			rechargeCard(&cardNode->data, CARDPATH, nIndex_c);

			//����Ϣ���浽billin.txt�ļ�
			if (FALSE == rechargeBilling(&billingNode->data,BILLINGPATH, nIndex_b))
			{
				printf("��ֵ����billing��Ϣʧ��");
				return FALSE;
			}

			strcpy(pMoneyInfo->CardName, cardNode->data.aName);
			pMoneyInfo->fBalance = cardNode->data.fBalance;
			pMoneyInfo->fMoney = Money;

			return 1;			//��ֵ�ɹ�
		}
		cardNode = cardNode->next;
		nIndex_c++;
		billingNode = billingNode->next;
		nIndex_b++;
	}
	return 0;					////��ֵʧ��

}

/****************************************************************************************
[������]��dorefund
[����]���˷Ѳ����¿���Ϣ
[����]��const char*pName(�˷ѿ�����), char* pPwd(�˷ѿ�����), MoneyInfo* pRefundInfo(�˷ѿ��ṹ�壬������ʱ�����˷ѿ���Ϣ)
[����ֵ]��0-�������1-�˷ѳɹ���2-���㣬3-��ʧЧ
*****************************************************************************************/
int dorefund(const char*pName, char* pPwd, MoneyInfo* pRefundInfo)
{
	lpCardNode cardNode = NULL;
	lpBillingNode billingNode = NULL;
	float Money;
	int nIndex_c = 0;
	int nIndex_b = 0;
	Billing billing;
	//��ȡ���ļ���Ϣ
	if (FALSE == getCard_b())
	{
		printf("�˷ѻ�ȡ���ļ���Ϣʧ�ܣ�");
		return FALSE;
	}
	cardNode = cardList_b->next;			//���տ���Ϣ

	if (FALSE == getBilling())
	{
		printf("�˷ѻ�ȡ�����ļ���Ϣʧ�ܣ�");
		return FALSE;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)
	{
		if ((strcmp(cardNode->data.aName, pName) == 0)&&(strcmp(cardNode->data.aName, pName)==0))
		{
			//��ѯ�����<0�����˷�
			if (cardNode->data.fBalance<=0)
			{

				return 2;		//����
			}


			//����������Ϣ
			Money = cardNode->data.fBalance;
			cardNode->data.fBalance = 0;
			billing = billingNode->data;
			billingNode->data.fBalance = cardNode->data.fBalance;

			//����Ϣ���浽Card.txt�ļ�
			rechargeCard(&cardNode->data, CARDPATH, nIndex_c);

			//����Ϣ���浽billin.txt�ļ�
			if (FALSE == rechargeBilling(&billingNode->data, BILLINGPATH, nIndex_b))
			{
				printf("�˷Ѹ���billing��Ϣʧ��");
				return FALSE;
			}

			strcpy(pRefundInfo->CardName, cardNode->data.aName);
			pRefundInfo->fBalance = cardNode->data.fBalance;
			pRefundInfo->fMoney = Money;

			return 1;			//�˷ѳɹ�
		}
		cardNode = cardNode->next;
		nIndex_c++;
		billingNode = billingNode->next;
		nIndex_b++;
	}
	return 0;					////�˷�ʧ��

}

/****************************************************************************************
[������]��logout
[����]����λ��λ�ò�ע����
[����]��const char* pName(��ע��������), char* pPwd(��ע����������),LogoutInfo* pLogoutInfo()
[����ֵ]��0-�������1-ע���ɹ���2-�����ϻ�
*****************************************************************************************/
int dologout(const char* pName, char* pPwd,LogoutInfo* pLogoutInfo)
{
	lpCardNode cardNode = NULL;
	lpBillingNode billingNode = NULL;
	int nIndex_c = 0;
	int nIndex_b = 0;
	float Money = 0;
	Billing billing;

	if (FALSE == getCard_b())
	{
		printf("ע������ȡ���ļ���Ϣʧ�ܣ�");
		return FALSE;
	}
	cardNode = cardList_b->next;			//���տ���Ϣ

	if (FALSE == getBilling())
	{
		printf("ע������ȡ�����ļ���Ϣʧ�ܣ�");
		return FALSE;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)		//���յ��Ŀ���Ϣ��Ϊ��
	{
		while (billingNode != NULL)
		{
			//�жϿ��������Ƿ���ȷ
			if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd) == 0))
			{

				//�ж��ܷ����ע��:�жϿ�״̬�Ϳ����
				if (cardNode->data.nStatus == 1)
				{
					return 2;						//�����ϻ�
				}
				if (cardNode->data.nStatus == 2)
				{
					return 3;						//��ע��
				}

				if (cardNode->data.fBalance <= 0)
				{
					return 4;						//����
				}

				//���������Ϣ
				Money = cardNode->data.fBalance;

				//�����»����Ŀ���Ϣ:�ļ��������еĿ���Ϣ

				//����cardList�����е���Ϣ
				cardNode->data.nStatus = 2;
				cardNode->data.fBalance = 0;
				cardNode->data.nDel = 1;
				cardNode->data.fTotaluse = 0;
				cardNode->data.nUseCount = 0;
				cardNode->data.tEnd = 0;
				cardNode->data.tLastTime = 0;
				cardNode->data.tSart = 0;

				//����billingList�������»����Ŀ���Ϣ
				billingNode->data.nStatus = 1;
				billingNode->data.tEnd = cardNode->data.tLastTime;
				billingNode->data.fBalance = cardNode->data.fBalance;
				billingNode->data.fAmount = 0;
				billingNode->data.nDel = cardNode->data.nDel;
				billingNode->data.tStart = cardNode->data.tSart;

				//�����ļ��еĿ���Ϣ
				if (updateCard(&cardNode->data, CARDPATH, nIndex_c) == TRUE)
				{
					//��װ������Ϣ
					strcpy(billing.aCardName, cardNode->data.aName);
					strcpy(billing.aCardPwd, cardNode->data.aPwd);
					billing.tStart = billingNode->data.tStart;
					billing.nStatus = billingNode->data.nStatus;
					billing.nDel = billingNode->data.nDel;
					billing.tEnd = billingNode->data.tEnd;
					billing.fAmount = 0;
					billing.fBalance = cardNode->data.fBalance;

					//�����ļ��е�������Ϣ
					if (TRUE == updateBilling(&billing, BILLINGPATH, nIndex_b))
					{
						strcpy(pLogoutInfo->CardName, pName);				//�����ű��浽�»����ṹ����
						pLogoutInfo->fMoney = Money;						//�˷���Ϣ

						//���ر�ʶ��Ϣ,ע���ɹ�
						return 1;											//ע���ɹ�
					}
				}
			}
			cardNode = cardNode->next;
			nIndex_c++;
			billingNode = billingNode->next;
			nIndex_b++;
		}
	}

	//�����������
	return 0;
}

/****************************************************************************************
[������]��getCard_b
[����]����ȡ����Ϣ
[����]����
[����ֵ]����ȷִ�з���TRUE ����ִ�з���FALSE
*****************************************************************************************/
int getCard_b()
{
	int nCount = 0;
	int i = 0;						//����forѭ������
	Card* pCard = NULL;				//���ڽ��ն�ȡ���Ŀ���Ϣ
	lpCardNode node = NULL;
	lpCardNode cur = NULL;

	if (cardList_b != NULL)
	{
		releaseCardList_b();
	}

	initCardList_b();

	//��ȡ����Ϣ����
	nCount = getCardCount(CARDPATH);			//���ļ��л�ȡ����Ϣ����

	//��̬�����ڴ�
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard == NULL)
	{
		printf("�ڴ�����ʧ�ܣ�");
		return FALSE;
	}

	//��ȡ����Ϣ
	if (readCard(pCard, CARDPATH) == FALSE)
	{
		free(pCard);
		pCard = NULL;
		printf("��ȡ����Ϣʧ�ܣ�");
		return FALSE;
	}

	//������Ϣ������pCard[i]���浽������
	for (i = 0, node = cardList_b; i < nCount; i++)
	{
		//Ϊ�������ڴ�
		cur = (lpCardNode)malloc(sizeof(CardNode));

		if (cur == NULL)
		{
			free(pCard);
			printf("�ڵ��ڴ����ʧ�ܣ�");
			return FALSE;
		}
		//��ʼ���µĿռ䣬ȫ����ֵΪ0
		memset(cur, 0, sizeof(CardNode));

		//������Ϣ���浽�����
		cur->data = pCard[i];
		cur->next = NULL;

		//�������ӵ�����β��
		node->next = cur;
		node = cur;

	}

	free(pCard);
	pCard = NULL;

	return TRUE;
}
/****************************************************************************************
[������]��initCardList
[����]����ʼ������
[����]����
[����ֵ]����ȷִ�з���TRUE ����ִ�з���FALSE
*****************************************************************************************/
int initCardList_b()
{
	headcard_b = (lpCardNode)malloc(sizeof(CardNode));	//Ϊ����ͷ�������ڴ�
	if (headcard_b != NULL)
	{
		headcard_b->next = NULL;
		cardList_b = headcard_b;
		return TRUE;
	}
	else
		return FALSE;
}
/****************************************************************************************
[������]��releaseCardList_b
[����]���ͷŽڵ�
[����]����
[����ֵ]����
*****************************************************************************************/
void releaseCardList_b()
{


	lpCardNode cur = cardList_b;
	lpCardNode next = NULL;

	while (cur != NULL)
	{
		next = cur->next; //�ͷ�cur���ǰ����next�����̽��
		free(cur);
		cur = next;
	}
	cardList_b = NULL;

}