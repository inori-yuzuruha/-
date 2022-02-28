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
[函数名]：outputMenu_m
[功能]：输出管理员系统菜单
[参数]：void
[返回值]：void
*****************************************************************************************/
void outputMenu_m(void)
{
	//输出系统菜单
	printf("------菜单------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.查询统计\n");
	printf("8.注销卡\n");
	printf("0.退出\n");
	printf("请选择菜单项编号(0-8):");//提示用户输入菜单
}
/****************************************************************************************
[函数名]：outputMenu_m
[功能]：输出用户系统菜单
[参数]：void
[返回值]：void
*****************************************************************************************/
void outputMenu_u(void)
{
	//输出系统菜单
	printf("------菜单------\n");
	printf("1.查询\n");
	printf("2.上机\n");
	printf("3.下机\n");
	printf("4.注销卡\n");
	printf("0.退出\n");
	printf("请选择菜单项编号(0-4):");//提示用户输入菜单
}
/****************************************************************************************
[函数名]：add
[功能]：添加卡
[参数]：void
[返回值]：void
*****************************************************************************************/
void add()
{
	Card card;
	Billing billing;
	char aName[32] = { '\0' };			//临时记录卡号
	char aPwd[20] = { '\0' };			//临时保存密码
	int nNameSize, nPwsSize;
	struct tm* startTime;				//开卡时间
	struct tm* endTime;					//截止时间
	Card* pCard = NULL;
	int nIndex = 0;
	FILE* fp = NULL;
	int privilige = 0;					//卡权限

	printf("------添加卡------\n");		//当前状态提示

	//提示输入并接收卡号

	if (fopen(CARDPATH,"r")==NULL)
	{
		printf("请设置卡号(1-18个字符)：");
		scanf_s("%s", aName, sizeof(aName));
	}
	else
	{
		printf("请设置卡号(1-18个字符)：");
		while (1)
		{
			fflush(stdin);
			scanf_s("%s", aName, sizeof(aName));
			nIndex = 0;
			pCard = queryCards(aName, &nIndex);
			if (pCard != NULL)
			{
				printf("卡号已存在，请重新输入：");
			}
			else
			{
				break;
			}
		}
	}

	//判断卡号长度
	nNameSize = getsize(aName);
	if (nNameSize > 18)
	{
		printf("输入的长度超过规定!\n");
		return;
	}

	//将卡号保存到结构体中
	strcpy_s(card.aName, aName);
	//提示输入并接收密码
	printf("\n请设置密码（1-8个字符）：");
	scanf_s("%s", aPwd, sizeof(aPwd));

	//判断密码长度
	nPwsSize = getsize(aPwd);
	if (nPwsSize > 8)
	{
		printf("输入的长度超过规定!\n");
		return;
	}

	//将密码保存到卡结构体中
	strcpy_s(card.aPwd, aPwd);

	printf("\n请设置权限(1-用户，2-管理员)：");
	scanf_s("%d", &privilige);

	//提示输入并接收开卡金额
	printf("\n请输入开卡金额(RMB)：");
	scanf_s("%f", &card.fBalance);
	card.fBalance = card.fBalance;					//卡余额
	card.fTotaluse = 0;								//卡总使用时长
	card.nUseCount = 0;								//上机次数
	card.nDel = 0;									//删除标识
	card.nStatus = 0;								//卡状态
	card.Privilge = privilige;						//卡权限等级

	card.tSart = card.tEnd = card.tLastTime = time(NULL);	//初始化开卡，有效期至，和最后使用时间

														//设计卡的有效期（初始为1年）
	startTime = localtime(&card.tSart);
	endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;

	//将最后有效期转化为time_t的格式并保存到卡信息结构体中
	card.tEnd = mktime(endTime);

	//构造消费信息
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
		printf("添加卡信息失败\n");
	}
	else
	{
		printf("---添加的卡信息如下:---\n");
		printf("卡号\t密码\t卡状态\t余额\n%s\t%s\t%d\t%.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
	}
}
/****************************************************************************************
[函数名]：query
[功能]：查询卡
[参数]：void
[返回值]：void
*****************************************************************************************/
void query()
{
	int nIndex = 0, i;
	Card* pCard = NULL;
	char aTime[20] = { '0' };
	char aName[18] = { '0' };
	printf("请输入您要查询的卡号：");
	scanf_s("%s", aName, sizeof(aName));

	//调用queryCard函数遍历数组查询
	pCard = queryCards(aName, &nIndex);

	if ((pCard != NULL) || (nIndex != 0))
	{
		printf("查询到的卡信息如下：\n");
		printf("卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n");

		for (i = 0; i < nIndex; i++)
		{
			//将时间转化为格式为“年-月-日 时:分 的格式”
			timeToString(pCard[i].tLastTime, aTime);

			//显示
			printf("%s\t%d\t%.1f\t%d\t\t", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotaluse);
			printf("%d\t\t%s\n",pCard[i].nUseCount ,aTime);

		}


	}
	else
	{
		printf("未查询到相关信息，请重新检查您的查询关键词！\n");
	}
	//释放动态分配的内存
	free(pCard);
	pCard = NULL;

}
/****************************************************************************************
[函数名]：logon
[功能]：上机
[参数]：void
[返回值]：void
*****************************************************************************************/
void logon()
{
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	Card* pCard = NULL;									//将从文件中读取的数据存放的数组
	char LastTime[TINMELENGTH] = { 0 };
	LogonInfo* pInfo = NULL;
	int nResult = 0;
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));		//用于保存上机卡的卡号，余额和上机时间

	//接受用户输入的卡号和密码
	printf("请输入卡号：");
	scanf_s("%s", &aName,sizeof(aName));
	printf("请输入密码：");
	scanf_s("%s", &aPwd,sizeof(aPwd));


	//根据上机结果，提示不同信息
	nResult= dologon(aName, aPwd,pInfo);

	switch (nResult)
	{
	case 0:
		printf("上机失败！\n");
		break;
	case 1:
		printf("上机成功！\n");
		printf_s("卡号\t余额\t\t上机时间\n");
		timeToString(pInfo->tLogon, LastTime);
		printf_s("%s\t%0.1f\t\t%s\n", pInfo->aCardName, pInfo->fBalance, LastTime);
		break;
	case 2:
		printf("该卡不能使用！\n");
		break;
	case 3:
		printf("该卡的余额不足！\n");
		break;
	}
	//释放上机信息
	free(pInfo);
}
/****************************************************************************************
[函数名]：settle
[功能]：下机
[参数]：void
[返回值]：void
*****************************************************************************************/
void settle()
{

	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nResult = 0;
	char tStart[20] = { 0 };
	char tEnd[20] = { 0 };
	SettleInfo* pInfo = NULL;							//用于保存下机卡的卡号，上机时间，下机时间，消费金额和余额
	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	//提示用户输入下机卡号
	printf("请输入下机卡卡号:");

	//接收下机卡号
	scanf_s("%s", aName, 18);

	//提示用户输入下机密码
	printf("请输入下机卡密码:");

	//接收下机密码
	scanf_s("%s", aPwd, 8);

	//根据下机结果，提示不同信息
	nResult = dosettle(aName, aPwd, pInfo);

	switch (nResult)
	{
	case 0:
		printf_s("密码错误，下机失败\n");
		break;
	case 1:
		printf_s("下机成功\n");
		printf_s("卡号\t消费金额\t余额\t\t上机时间\t\t下机时间\n");

		timeToString(pInfo->tStart, tStart);
		timeToString(pInfo->tEnd, tEnd);

		printf_s("%s\t%.1f\t\t%.1f\t\t%s\t%s\n", aName, pInfo->fAmount, pInfo->fBalance, tStart, tEnd);
		break;
	case 2:
		printf("已下机，无法下机！\n");
		break;
	case 3:
		printf("余额不足，无法下机!\n");
		break;

}

	//释放下机信息
	free(pInfo);
}
/****************************************************************************************
[函数名]：rechage
[功能]：充值
[参数]：无
[返回值]：空
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

	printf("请输入充值的卡号：");
	while (1)
	{
		scanf_s("%s", aName,sizeof(aName));
		nIndex = 0;
		pCard = queryCards(aName, &nIndex);
		if (pCard == NULL)
		{
			printf("卡号不存在，请重新输入：");
		}
		else
		{

			if ((pCard->nStatus == 2) || (pCard->nStatus == 3))
			{
				printf("该卡已注销或已失效，无法充值，请重新检查卡号！");
			}
			else
			{
				break;
			}
		}
	}

	printf("请输入您要充值的金额：");
	scanf_s("%f", &money);

	//为文件中的卡信息进行充值
	nResult = dorecharge(aName, money, pInfo);
	switch (nResult)
	{
	case 0:
		printf("充值失败！");
		break;
	case 1:
		printf("充值成功！\n");
		printf("卡号\t充值金额\t余额\n");
		printf("%s\t%.1f\t\t%.1f\n", pInfo->CardName, pInfo->fMoney, pInfo->fBalance);
	}

}
/****************************************************************************************
[函数名]：refund
[功能]：退费
[参数]：无
[返回值]：空
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

	printf("请输入退费的卡号：");
	while (1)
	{
		scanf_s("%s", aName, sizeof(aName));
		scanf_s("%*[^\n]%*c");
		nIndex = 0;
		pCard = queryCards(aName, &nIndex);
		if (pCard == NULL)
		{
			printf("卡号不存在，请重新输入：");
		}
		else
		{

			if ((pCard->nStatus == 2) || (pCard->nStatus == 3))
			{
				printf("该卡已注销或已失效，无法充值，请重新检查卡号！");
			}
			else
			{
				break;
			}
		}
	}

	printf("请输入卡密码：");
	scanf_s("%f", aPwd);

	getchar();

	//为文件中的卡信息进行退费
	nResult = dorefund(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0:
		printf("密码错误，退费失败！");
		break;
	case 1:
		printf("退费成功！\n");
		printf("卡号\t退费金额\t余额\n");
		printf("%s\t%.1f\t\t%.1f\n", pInfo->CardName, pInfo->fMoney, pInfo->fBalance);
		break;
	case 2:
		printf("余额不足，退费失败！");
		break;

	}
}
/****************************************************************************************
[函数名]：logout
[功能]：注销卡
[参数]：无
[返回值]：空
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

	//提示用户输入要注销的卡卡号
	printf("请输入要注销的卡号：");

	while (1)
	{
		fflush(stdin);
		scanf_s("%s", aName, sizeof(aName));
		nIndex = 0;
		pCard = queryCards(aName, &nIndex);
		if (pCard == NULL)
		{
			printf("卡号不存在，请重新输入：");
		}
		else
		{
			break;
		}
	}

	//提示用户输入要注销的卡卡密码
	printf("请输入要注销的卡卡密码：");

	scanf_s("%s", aPwd, sizeof(aPwd));

	//清除scanf_s缓存区
	scanf_s("%*[^\n]%*c");

	//定位并注销卡
	nResult = dologout(aName, aPwd, pInfo);;

	//根据返回信息判断注销结果
	switch (nResult)
	{
	case 0:
		printf("密码输入错误！\n");
		break;
	case 1:
		printf("注销成功！\n");
		printf("卡号\t退费金额\n");
		printf("%s\t%.1f\n", pInfo->CardName, pInfo->fMoney);
		break;
	case 2:
		printf("正在上机！\n");
		break;
	case 3:
		printf("已注销！\n");
		break;
	case 4:
		printf("余额不足，注销失败！\n");
		break;
	}

}
/****************************************************************************************
[函数名]：exitApp
[功能]：退出应用程序
[参数]：void
[返回值]：void
*****************************************************************************************/
void exitApp()
{
	releaseList();
}
