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
Card acard[50];		//卡结构体数组
int nCount = 0;		//实际卡信息数量

int getCard();		//获取并保存卡信息

lpCardNode cardList = NULL;
lpCardNode head = NULL;

/****************************************************************************************
[函数名]：initCardList
[功能]：初始化链表
[参数]：无
[返回值]：无
*****************************************************************************************/
int initCardList()
{
	head = (lpCardNode)malloc(sizeof(CardNode));	//为链表头结点分配内存
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
[函数名]：addCard
[功能]：将卡信息结构体添加到结构体数组中
[参数]：card:添加到结构体数组中的卡信息结构体
[返回值]：true or false
*****************************************************************************************/
int addCard(Card card)
{
	return saveCard(&card, CARDPATH);
}
/****************************************************************************************
[函数名]：void
[功能]：释放节点
[参数]：*pName：卡号数组的首地址
[返回值]：查询到的卡信息结构体的首地址
*****************************************************************************************/
void releaseCardList()
{


		lpCardNode cur = cardList;
		lpCardNode next = NULL;

		while (cur != NULL)
		{
			next = cur->next; //释放cur结点前，用next保存后继结点
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
		printf("申请内存失败！");
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
[函数名]：getCard
[功能]：将文件中的卡信息保存到链表里
[参数]：无
[返回值]：0-FALSE,1-TRUE
*****************************************************************************************/
int getCard()
{
	int nCount = 0;
	int i = 0;						//定义for循环变量
	Card* pCard = NULL;				//用于接收读取到的卡信息
	lpCardNode node = NULL;
	lpCardNode cur = NULL;

	if (cardList!=NULL)
	{
		releaseCardList();
	}

	initCardList();

	//获取卡信息数量
	nCount = getCardCount(CARDPATH);			//从文件中获取卡信息数量

	//动态分配内存
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard==NULL)
	{
		printf("内存申请失败！");
		return FALSE;
	}
	
	//获取卡信息
	if (readCard(pCard,CARDPATH)== FALSE)
	{
		free(pCard);
		pCard = NULL;
		printf("读取卡信息失败！");
		return FALSE;
	}

	//将卡信息从数组pCard[i]保存到链表中
	for (i = 0,node=cardList; i < nCount; i++)
	{
		//为结点分配内存
		cur = (lpCardNode)malloc(sizeof(CardNode));

		if (cur==NULL)
		{
			free(pCard);
			printf("节点内存分配失败！");
			return FALSE;
		}
		//初始化新的空间，全部赋值为0
		memset(cur, 0, sizeof(CardNode));

		//将卡信息保存到结点中
		cur->data = pCard[i];
		cur->next = NULL;

		//将结点添加到链表尾部
		node->next = cur;
		node = cur;

	}
	free(pCard);
	pCard = NULL;

	return TRUE;
}
/****************************************************************************************
[函数名]：getCardCount
[功能]：获取卡信息
[参数]：要获取对象的路径
[返回值]：获取到的卡信息数量
*****************************************************************************************/
int getCardCount(const char* pPath)
{
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };
	int nCount = 0;

	//打开文件
	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		printf("文件打开失败！");
		return FALSE;
	}

	//读取文件
	while ((!feof(fp)))
	{
		memset(aBuf, 0, CARDCHARNUM);				//清空数组	

		if ((fgets(aBuf, CARDCHARNUM, fp)) == NULL)
		{
			break;
		}
		if (strlen(aBuf) > 0)
		{
			nCount++;
		}
	}



	//关闭文件
	fclose(fp);

	return nCount;
}
