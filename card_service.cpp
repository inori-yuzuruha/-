#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include "global.h"
#include"model.h"
#include"card_file.h"
#include"Billing_file.h"
#include"billing_service.h"
#include"tool.h"

#define CARDCHARNUM 256
Card acard[50];		//���ṹ������
int nCount = 0;		//ʵ�ʿ���Ϣ����

int getCard();		//��ȡ�����濨��Ϣ

lpCardNode cardList = NULL;
lpCardNode head = NULL;

/****************************************************************************************
[������]��initCardList
[����]����ʼ������
[����]����
[����ֵ]����
*****************************************************************************************/
int initCardList()
{
	head = (lpCardNode)malloc(sizeof(CardNode));	//Ϊ����ͷ�������ڴ�
	if (head != NULL)
	{
		head->next = NULL;
		cardList = head;
		return TRUE;
	}
	else
		return FALSE;
}
/****************************************************************************************
[������]��addCard
[����]��������Ϣ�ṹ����ӵ��ṹ��������
[����]��card:��ӵ��ṹ�������еĿ���Ϣ�ṹ��
[����ֵ]��true or false
*****************************************************************************************/
int addCard(Card card)
{
	return saveCard(&card, CARDPATH);
}
/****************************************************************************************
[������]��void
[����]���ͷŽڵ�
[����]��*pName������������׵�ַ
[����ֵ]����ѯ���Ŀ���Ϣ�ṹ����׵�ַ
*****************************************************************************************/
void releaseCardList()
{


		lpCardNode cur = cardList;
		lpCardNode next = NULL;

		while (cur != NULL)
		{
			next = cur->next; //�ͷ�cur���ǰ����next�����̽��
			free(cur);
			cur = next;
		}
		cardList = NULL;

}
Card* queryCards(const char* pName, int* pIndex)
{
	lpCardNode cur = NULL;
	Card* pCard = NULL;
	pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)
	{
		printf("�����ڴ�ʧ�ܣ�");
		return NULL;
	}

	if (FALSE != getCard())
	{
		cur = head;

		while (cur != NULL)
		{
			if (strstr(cur->data.aName, pName) != NULL)
			{
				pCard[*pIndex] = cur->data;
				(*pIndex)+=1;
				pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
			}
			if (cur->next != NULL)
			{
				cur = cur->next;
			}
			else
			{
				break;
			}
		}
	}
	if (*pIndex != 0)
	{
		return pCard;
	}
	else
	{
		return NULL;
	}
}
/****************************************************************************************
[������]��getCard
[����]�����ļ��еĿ���Ϣ���浽������
[����]����
[����ֵ]��0-FALSE,1-TRUE
*****************************************************************************************/
int getCard()
{
	int nCount = 0;
	int i = 0;						//����forѭ������
	Card* pCard = NULL;				//���ڽ��ն�ȡ���Ŀ���Ϣ
	lpCardNode node = NULL;
	lpCardNode cur = NULL;

	if (cardList!=NULL)
	{
		releaseCardList();
	}

	initCardList();

	//��ȡ����Ϣ����
	nCount = getCardCount(CARDPATH);			//���ļ��л�ȡ����Ϣ����

	//��̬�����ڴ�
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard==NULL)
	{
		printf("�ڴ�����ʧ�ܣ�");
		return FALSE;
	}
	
	//��ȡ����Ϣ
	if (readCard(pCard,CARDPATH)== FALSE)
	{
		free(pCard);
		pCard = NULL;
		printf("��ȡ����Ϣʧ�ܣ�");
		return FALSE;
	}

	//������Ϣ������pCard[i]���浽������
	for (i = 0,node=cardList; i < nCount; i++)
	{
		//Ϊ�������ڴ�
		cur = (lpCardNode)malloc(sizeof(CardNode));

		if (cur==NULL)
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
[������]��getCardCount
[����]����ȡ����Ϣ
[����]��Ҫ��ȡ�����·��
[����ֵ]����ȡ���Ŀ���Ϣ����
*****************************************************************************************/
int getCardCount(const char* pPath)
{
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };
	int nCount = 0;

	//���ļ�
	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		printf("�ļ���ʧ�ܣ�");
		return FALSE;
	}

	//��ȡ�ļ�
	while ((!feof(fp)))
	{
		memset(aBuf, 0, CARDCHARNUM);				//�������	

		if ((fgets(aBuf, CARDCHARNUM, fp)) == NULL)
		{
			break;
		}
		if (strlen(aBuf) > 0)
		{
			nCount++;
		}
	}



	//�ر��ļ�
	fclose(fp);

	return nCount;
}
