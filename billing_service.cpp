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

lpCardNode cardList_b = NULL;			//定义更新消费文件中的消费信息卡信息链表
lpCardNode headcard_b = NULL;			//定义更新消费文件中的消费信息卡信息链表表头

lpBillingNode billingList = NULL;		//定义更新消费文件中的消费信息的消费信息链表
lpBillingNode headbilling = NULL;		//定义更新消费文件中的消费信息的消费信息链表表头


/****************************************************************************************
[函数名]：addBilling
[功能]：将计费信息保存到billion.mas文件中
[参数]：Billing billing
[返回值]：TRUE-正确返回 FALSE-错误返回
*****************************************************************************************/
int addBilling(Billing billing)
{
	return saveBilling(&billing, BILLINGPATH);
}
/****************************************************************************************
[函数名]：initBillingList
[功能]：初始化链表
[参数]：无
[返回值]：无
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
[函数名]：releaseBillingList
[功能]：释放链表
[参数]：无
[返回值]：无
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
[函数名]：getBilling
[功能]：获取消费文件中的消费信息
[参数]：无
[返回值]：正确执行返回TRUE 错误执行返回FALSE
*****************************************************************************************/
int getBilling()
{
	int nCount = 0;
	int i = 0;				//定义for循环变量
	Billing* pBilling = NULL;
	lpBillingNode node = NULL;

	if (billingList != NULL)
	{
		releaseBillingList();
	}

	initBillingList();

	nCount = getBillingCount(BILLINGPATH);

	//动态分配内存
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling == NULL)
	{
		printf("内存申请失败！");
		return FALSE;
	}

	//获取卡信息
	if (readBillings(pBilling, BILLINGPATH) == FALSE)
	{
		free(pBilling);
		pBilling = NULL;
		printf("读取卡信息失败！");
		return FALSE;
	}

	//将卡信息保存到链表中
		for (i = 0, node = billingList; i < nCount; i++)
		{
			lpBillingNode cur = NULL;
			//为结点分配内存
			cur = (lpBillingNode)malloc(sizeof(CardNode));

			if (cur == NULL)
			{
				free(pBilling);
				printf("节点内存分配失败！");
				return FALSE;
			}
			//初始化新的空间，全部赋值为0
			memset(cur, 0, sizeof(CardNode));

			//将卡信息保存到结点中
			cur->data = pBilling[i];
			cur->next = NULL;

			//将结点添加到链表尾部
			node->next = cur;
			node = cur;

		}
		free(pBilling);
		pBilling = NULL;

		return TRUE;

}

/****************************************************************************************
[函数名]：dologon
[功能]：上机并更新上机卡信息
[参数]：char* aName(上机卡卡号), char* aPwd(上机卡密码), pLogonInfo* pLogonInfo(上机信息结构体用于临时保存上机信息)
[返回值]：0-密码错误，1-上机成功，2-正在上机，3-余额不足
*****************************************************************************************/
int dologon(const char* aName, const char* aPwd, LogonInfo* pLogonInfo)
{
	lpBillingNode billingNode = NULL;
	lpCardNode cardNode = NULL;		//新建节点，用于接收从getCard中传递多来的链表信息（卡结构体信息）
	int nIndex_b = 0;
	int nIndex_c = 0;					//上机卡在文件中的行数
	Billing billing;

	//获取卡信息
	if (FALSE == getCard_b())
	{
		return NULL;
	}
	cardNode = cardList_b->next;

	//获取消费信息
	if (FALSE == getBilling())
	{
		return NULL;
	}
	billingNode = billingList->next;

	while (billingNode != NULL)
	{
		//判断卡号密码是否正确
		if ((strcmp(billingNode->data.aCardName, aName) == 0) && (strcmp(billingNode->data.aCardPwd, aPwd) == 0))
		{
			break;
		}
		billingNode = billingNode->next;
		nIndex_b++;
	}


	while (cardNode != NULL)		//接收到的卡信息不为空
	{
		//判断卡号密码是否正确
		if ((strcmp(cardNode->data.aName, aName) == 0) && (strcmp(cardNode->data.aPwd, aPwd) == 0))
		{
			//判断能否进行上机:判断卡状态和卡余额
			if (cardNode->data.nStatus != 0)
			{
				return 2;						//正在上机
			}
			if (cardNode->data.fBalance <= 0)
			{
				return 3;						//余额不足
			}

			//更新上机卡的卡信息:文件和链表中的卡信息

			//更新链表中上机卡的卡信息
			cardNode->data.nStatus = 1;
			cardNode->data.nUseCount++;
			cardNode->data.tLastTime = time(NULL);

			//更新文件中的卡信息
			if (updateCard(&cardNode->data, CARDPATH, nIndex_c) == TRUE)
			{
				//组装上机前卡的消费信息
				strcpy(billing.aCardName, aName);
				strcpy(billing.aCardPwd, aPwd);
				billing.tStart = time(NULL);
				billing.fAmount = 0;
				billing.fBalance = cardNode->data.fBalance;
				billing.nDel = 0;
				billing.tEnd = 0;
				billing.nStatus = 1;

				billingNode->data = billing;

				//保存消费信息
				if (TRUE == updateBilling(&billingNode->data, BILLINGPATH, nIndex_b))
				{
					strcpy(pLogonInfo->aCardName, aName);				//将卡号保存到上机卡结构体中
					pLogonInfo->fBalance = cardNode->data.fBalance;		//将当前余额保存到上机卡结构体中
					pLogonInfo->tLogon = billing.tStart;				//将上机时间保存到上机卡结构体中
					//返回相应信息
					return 1;											//上机成功
				}
			}
		}
		cardNode = cardNode->next;
		nIndex_c++;
	}
	return 0;


}

/****************************************************************************************
[函数名]：dosettle
[功能]：下机并更新下机卡信息
[参数]：char* aName(下机卡卡号), char* aPwd(下机卡密码), pLogonInfo* pLogonInfo(下机信息结构体用于临时保存下机信息)
[返回值]：0-密码错误，1-下机成功，2-已下机，3-余额不足
*****************************************************************************************/
int dosettle(const char* pName, const char* pPwd, SettleInfo* pSettleInfo)
{
	lpCardNode cardNode= NULL;
	lpBillingNode billingNode = NULL;	//新建节点，用于接收从getBilling中传递多来的链表信息（卡结构体信息）
	int nIndex_c = 0;
	int nIndex_b = 0;
	Billing billing;
	int nPosition = 0;
	float nAmount;

	//获取卡信息
	if (FALSE == getCard_b())
	{
		return NULL;
	}
	cardNode = cardList_b->next;

	//获取消费信息
	if (FALSE == getBilling())
	{
		return NULL;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)		//接收到的卡信息不为空
	{
		while (billingNode != NULL)
		{
			//判断卡号密码是否正确
			if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd) == 0))
			{

				//判断能否进行下机:判断卡状态和卡余额
				if (cardNode->data.nStatus != 1)
				{
					return 2;						//已下机
				}
				if (cardNode->data.fBalance <= 0)
				{
					return 3;						//余额不足
				}

				//保存本次消费金额
				nAmount = getAmount(billingNode->data.tStart);

				//更新下机卡的卡信息:文件和链表中的卡信息

				//更新cardList链表中的信息
				cardNode->data.nStatus = 0;
				cardNode->data.fTotaluse+=nAmount;
				cardNode->data.tLastTime = time(NULL);
				cardNode->data.fBalance -= nAmount;

				//更新billingList链表中下机卡的卡信息
				billingNode->data.nStatus = 1;
				billingNode->data.tEnd = cardNode->data.tLastTime;
				billingNode->data.fBalance -= nAmount;
				billingNode->data.fAmount = nAmount;

				//更新文件中的卡信息
				if (updateCard(&cardNode->data, CARDPATH, nIndex_c) == TRUE)
				{
					//组装消费信息
					strcpy(billing.aCardName, cardNode->data.aName);
					strcpy(billing.aCardPwd , cardNode->data.aPwd);
					billing.tStart = billingNode->data.tStart;
					billing.nStatus = 1;
					billing.nDel = 0;
					billing.tEnd = cardNode->data.tLastTime;
					billing.fAmount = nAmount;
					billing.fBalance = cardNode->data.fBalance;

					//更新文件中的消费信息
					if (TRUE == updateBilling(&billing, BILLINGPATH, nIndex_b))
					{
						strcpy(pSettleInfo->aCardName, pName);				//将卡号保存到下机卡结构体中
						pSettleInfo->tStart = billingNode->data.tStart;		//将上机时间保存到下机卡结构体中
						pSettleInfo->tEnd = billingNode->data.tEnd;			//将下机时间保存到下机卡结构体中
						pSettleInfo->fBalance = billingNode->data.fBalance;	//将余额保存到下机卡结构体中
						pSettleInfo->fAmount = billingNode->data.fAmount;	//将消费金额保存到下机卡结构体中

						//返回标识信息,下机成功
						return 1;											//下机成功
					}
				}
			}
			cardNode = cardNode->next;
			nIndex_c++;
			billingNode = billingNode->next;
			nIndex_b++;
		}
	}
	//密码输入错误
	return 0;
}

/****************************************************************************************
[函数名]：dorecharge
[功能]：充值并更新卡信息
[参数]：char* aName(待充值卡号), float Money(充值金额), MoneyInfo* pLogonInfo()
[返回值]：0-卡失效，1-充值成功
*****************************************************************************************/
int dorecharge(const char* pName, float Money, MoneyInfo* pMoneyInfo)
{

	lpCardNode cardNode = NULL;
	lpBillingNode billingNode = NULL;
	int nIndex_c = 0;
	int nIndex_b = 0;
	Billing billing;
	//获取卡文件信息
	if (FALSE == getCard_b())
	{
		printf("充值获取卡文件信息失败！");
		return FALSE;
	}
	cardNode = cardList_b->next;			//接收卡信息

	if (FALSE == getBilling())
	{
		printf("充值获取消费文件信息失败！");
		return FALSE;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)
	{
		if ((strcmp(cardNode->data.aName, pName)) == 0)
		{
			//更新链表信息
			cardNode->data.fBalance += Money;
			billing = billingNode->data;
			billingNode->data.fBalance = cardNode->data.fBalance;

			//将信息保存到Card.txt文件
			rechargeCard(&cardNode->data, CARDPATH, nIndex_c);

			//将信息保存到billin.txt文件
			if (FALSE == rechargeBilling(&billingNode->data,BILLINGPATH, nIndex_b))
			{
				printf("充值更新billing信息失败");
				return FALSE;
			}

			strcpy(pMoneyInfo->CardName, cardNode->data.aName);
			pMoneyInfo->fBalance = cardNode->data.fBalance;
			pMoneyInfo->fMoney = Money;

			return 1;			//充值成功
		}
		cardNode = cardNode->next;
		nIndex_c++;
		billingNode = billingNode->next;
		nIndex_b++;
	}
	return 0;					////充值失败

}

/****************************************************************************************
[函数名]：dorefund
[功能]：退费并更新卡信息
[参数]：const char*pName(退费卡卡号), char* pPwd(退费卡密码), MoneyInfo* pRefundInfo(退费卡结构体，用于临时保存退费卡信息)
[返回值]：0-密码错误，1-退费成功，2-余额不足，3-卡失效
*****************************************************************************************/
int dorefund(const char*pName, char* pPwd, MoneyInfo* pRefundInfo)
{
	lpCardNode cardNode = NULL;
	lpBillingNode billingNode = NULL;
	float Money;
	int nIndex_c = 0;
	int nIndex_b = 0;
	Billing billing;
	//获取卡文件信息
	if (FALSE == getCard_b())
	{
		printf("退费获取卡文件信息失败！");
		return FALSE;
	}
	cardNode = cardList_b->next;			//接收卡信息

	if (FALSE == getBilling())
	{
		printf("退费获取消费文件信息失败！");
		return FALSE;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)
	{
		if ((strcmp(cardNode->data.aName, pName) == 0)&&(strcmp(cardNode->data.aName, pName)==0))
		{
			//查询余额，余额<0不能退费
			if (cardNode->data.fBalance<=0)
			{

				return 2;		//余额不足
			}


			//更新链表信息
			Money = cardNode->data.fBalance;
			cardNode->data.fBalance = 0;
			billing = billingNode->data;
			billingNode->data.fBalance = cardNode->data.fBalance;

			//将信息保存到Card.txt文件
			rechargeCard(&cardNode->data, CARDPATH, nIndex_c);

			//将信息保存到billin.txt文件
			if (FALSE == rechargeBilling(&billingNode->data, BILLINGPATH, nIndex_b))
			{
				printf("退费更新billing信息失败");
				return FALSE;
			}

			strcpy(pRefundInfo->CardName, cardNode->data.aName);
			pRefundInfo->fBalance = cardNode->data.fBalance;
			pRefundInfo->fMoney = Money;

			return 1;			//退费成功
		}
		cardNode = cardNode->next;
		nIndex_c++;
		billingNode = billingNode->next;
		nIndex_b++;
	}
	return 0;					////退费失败

}

/****************************************************************************************
[函数名]：logout
[功能]：定位卡位置并注销卡
[参数]：const char* pName(待注销卡卡号), char* pPwd(待注销卡卡密码),LogoutInfo* pLogoutInfo()
[返回值]：0-密码错误，1-注销成功，2-正在上机
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
		printf("注销卡获取卡文件信息失败！");
		return FALSE;
	}
	cardNode = cardList_b->next;			//接收卡信息

	if (FALSE == getBilling())
	{
		printf("注销卡获取消费文件信息失败！");
		return FALSE;
	}
	billingNode = billingList->next;

	while (cardNode != NULL)		//接收到的卡信息不为空
	{
		while (billingNode != NULL)
		{
			//判断卡号密码是否正确
			if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd) == 0))
			{

				//判断能否进行注销:判断卡状态和卡余额
				if (cardNode->data.nStatus == 1)
				{
					return 2;						//正在上机
				}
				if (cardNode->data.nStatus == 2)
				{
					return 3;						//已注销
				}

				if (cardNode->data.fBalance <= 0)
				{
					return 4;						//余额不足
				}

				//保存余额信息
				Money = cardNode->data.fBalance;

				//更新下机卡的卡信息:文件和链表中的卡信息

				//更新cardList链表中的信息
				cardNode->data.nStatus = 2;
				cardNode->data.fBalance = 0;
				cardNode->data.nDel = 1;
				cardNode->data.fTotaluse = 0;
				cardNode->data.nUseCount = 0;
				cardNode->data.tEnd = 0;
				cardNode->data.tLastTime = 0;
				cardNode->data.tSart = 0;

				//更新billingList链表中下机卡的卡信息
				billingNode->data.nStatus = 1;
				billingNode->data.tEnd = cardNode->data.tLastTime;
				billingNode->data.fBalance = cardNode->data.fBalance;
				billingNode->data.fAmount = 0;
				billingNode->data.nDel = cardNode->data.nDel;
				billingNode->data.tStart = cardNode->data.tSart;

				//更新文件中的卡信息
				if (updateCard(&cardNode->data, CARDPATH, nIndex_c) == TRUE)
				{
					//组装消费信息
					strcpy(billing.aCardName, cardNode->data.aName);
					strcpy(billing.aCardPwd, cardNode->data.aPwd);
					billing.tStart = billingNode->data.tStart;
					billing.nStatus = billingNode->data.nStatus;
					billing.nDel = billingNode->data.nDel;
					billing.tEnd = billingNode->data.tEnd;
					billing.fAmount = 0;
					billing.fBalance = cardNode->data.fBalance;

					//更新文件中的消费信息
					if (TRUE == updateBilling(&billing, BILLINGPATH, nIndex_b))
					{
						strcpy(pLogoutInfo->CardName, pName);				//将卡号保存到下机卡结构体中
						pLogoutInfo->fMoney = Money;						//退费信息

						//返回标识信息,注销成功
						return 1;											//注销成功
					}
				}
			}
			cardNode = cardNode->next;
			nIndex_c++;
			billingNode = billingNode->next;
			nIndex_b++;
		}
	}

	//密码输入错误
	return 0;
}

/****************************************************************************************
[函数名]：getCard_b
[功能]：获取卡信息
[参数]：无
[返回值]：正确执行返回TRUE 错误执行返回FALSE
*****************************************************************************************/
int getCard_b()
{
	int nCount = 0;
	int i = 0;						//定义for循环变量
	Card* pCard = NULL;				//用于接收读取到的卡信息
	lpCardNode node = NULL;
	lpCardNode cur = NULL;

	if (cardList_b != NULL)
	{
		releaseCardList_b();
	}

	initCardList_b();

	//获取卡信息数量
	nCount = getCardCount(CARDPATH);			//从文件中获取卡信息数量

	//动态分配内存
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard == NULL)
	{
		printf("内存申请失败！");
		return FALSE;
	}

	//获取卡信息
	if (readCard(pCard, CARDPATH) == FALSE)
	{
		free(pCard);
		pCard = NULL;
		printf("读取卡信息失败！");
		return FALSE;
	}

	//将卡信息从数组pCard[i]保存到链表中
	for (i = 0, node = cardList_b; i < nCount; i++)
	{
		//为结点分配内存
		cur = (lpCardNode)malloc(sizeof(CardNode));

		if (cur == NULL)
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
[函数名]：initCardList
[功能]：初始化链表
[参数]：无
[返回值]：正确执行返回TRUE 错误执行返回FALSE
*****************************************************************************************/
int initCardList_b()
{
	headcard_b = (lpCardNode)malloc(sizeof(CardNode));	//为链表头结点分配内存
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
[函数名]：releaseCardList_b
[功能]：释放节点
[参数]：无
[返回值]：无
*****************************************************************************************/
void releaseCardList_b()
{


	lpCardNode cur = cardList_b;
	lpCardNode next = NULL;

	while (cur != NULL)
	{
		next = cur->next; //释放cur结点前，用next保存后继结点
		free(cur);
		cur = next;
	}
	cardList_b = NULL;

}