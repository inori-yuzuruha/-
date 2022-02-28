#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <corecrt_memory.h>
#include "model.h"
#include"global.h"
#include"tool.h"
#include "tool.h"

Card praseCard(char* pBuf);

#define CARDCHARNUM 256

int saveCard(const Card* pCard, const char* pPath)
{
	FILE* fp = NULL;
	char startTime[TINMELENGTH] = { '0' };			//开卡时间
	char endTime[TINMELENGTH] = { '0' };				//截止时间
	char lastTime[TINMELENGTH] = { '0' };			//最后使用时间

	//打开文件
	if ((fp = fopen(pPath, "a")) == NULL)
	{
		fp = fopen(pPath, "w");
		if (fp == NULL)
		{
			printf("打开文件失败\n");
			return FALSE;
		}
	}

	//将时间转换为字符串
	timeToString(pCard->tSart, startTime);			//转换开卡时间
	timeToString(pCard->tEnd, endTime);			//转换截止时间
	timeToString(pCard->tSart, lastTime);			//转换最后使用时间

	//将数据写进文件
	//格式为：卡号##密码##状态##开卡时间##截止时间##累计金额##最后使用时间##使用次数##当前余额##删除标识##卡权限

	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotaluse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel,pCard->Privilge);

	//关闭文件
	fclose(fp);

	return TRUE;
}
/****************************************************************************************
[函数名]：readCard
[功能]：将pPath文件中的信息按行保存到pCard[i]中
[参数]：要保存的数组，要读取文件的路径
[返回值]：TURE-成功，FALSE-失败
*****************************************************************************************/
int readCard(Card* pCard,const char* pPath)
{
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };
	int i = 0;

	//打开文件
	fp = fopen(pPath, "r");
	if (fp==NULL)
	{
		printf("文件打开失败！");
		return FALSE;
	}

	//读取文件，将文件中的卡信息按行读入pCard[i]中
	while ((!feof(fp)))
	{
		memset(aBuf, 0, CARDCHARNUM);				//清空数组	

		if ((fgets(aBuf, CARDCHARNUM, fp))==NULL)
		{
			break;
		}
		if (strlen(aBuf)>0)
		{
			pCard[i] = praseCard(aBuf);
			i++;
		}
	}
	//关闭文件
	fclose(fp);
	return TRUE;
}
/****************************************************************************************
[函数名]：Card praseCard(char* pBuf)
[功能]：解析函数,并将文件中的信息分条返回
[参数]：char* pBuf
[返回值]：解析出的卡结构体地址
*****************************************************************************************/
Card praseCard(char* pBuf)
{
	Card card;
	const char* delines = "##";			//定义字符串中的分割符
	char* buf = NULL;
	char* str = NULL;
	char flag[11][20] = { 0 };			//保存分割后的字符串
	int index = 0;

	buf = pBuf;
	
	while (((str = strtok(buf, delines))!=NULL)&&(index<=10))
	{
			strcpy(flag[index], str);
			buf = NULL;				//后面调用strtok函数是第一个参数为NULL
			index++;

	}

	strcpy(card.aName, flag[0]);
	strcpy(card.aPwd, flag[1]);
	card.nStatus = atoi(flag[2]);
	card.tSart = stringToTime(flag[3]);
	card.tEnd = stringToTime(flag[4]);
	card.fTotaluse = (float)atof(flag[5]);
	card.tLastTime = stringToTime(flag[6]);
	card.nUseCount = atoi(flag[7]);
	card.fBalance = (float)atof(flag[8]);
	card.nDel = atoi(flag[9]);
	card.Privilge = atoi(flag[10]);

	return card;
}
/****************************************************************************************
[函数名]：updateCard
[功能]：将数据写进文件标识处的卡信息中
[参数]：Card* pCard(源文件) char* pPath(写入二进制文件的文件路径) int nIndex(要写入的文件标识)
[返回值]：TRUE-写入成功，FALSE-写入失败
*****************************************************************************************/
int updateCard(const Card* pCard, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nLine = 0;
	char aBuf[CARDCHARNUM] = { 0 };
	long lPosition = 0;
	char  startTime[20] = {' 0'};
	char endTime[20] = { '0 '};
	char lastTime[TINMELENGTH] = { '0' };

	//以读写方式打开文件
	fp = fopen(pPath, "r+");

	if (fp == NULL)
	{
		printf("文件打开失败！");
		return FALSE;
	}

	//遍历文件，找到该条记录，进行更新

	while ((!feof(fp))&&(nLine<nIndex))
	{
		if (fgets(aBuf,CARDCHARNUM,fp)!=NULL)
		{
			//获取文件标识位置
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//移动到文件标识位置
	fseek(fp, lPosition, 0);

	//将数据写进文件

	//将时间转化为字符串
	timeToString(pCard->tSart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLastTime, lastTime);			//转换最后使用时间

	//格式为：卡号##密码##%d状态##开卡时间##截止时间##累计金额##最后使用时间##使用次数##当前余额##删除标识##卡权限
	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotaluse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel,pCard->Privilge) ;

	//关闭文件
	fclose(fp);

	return TRUE;

}

int rechargeCard(const Card* pCard , const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nLine = 0;
	char aBuf[CARDCHARNUM] = { 0 };
	long lPosition = 0;
	char startTime[20] = { 0 };
	char endTime[20] = { 0 };
	char lastTime[20] = { 0 };

	//以读写方式打开文件
	fp = fopen(pPath, "r+");

	if (fp==NULL)
	{
		printf("退费时，文件打开失败！");
		return FALSE;
	}

	//遍历文件，找到卡位置
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//获取文件标识位置
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//将文件指针指向该信息
	fseek(fp, lPosition, 0);

	//时间转换
	timeToString(pCard->tSart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLastTime, lastTime);

	//对卡信息进行写入
	//格式为：卡号##密码##状态##开卡时间##截止时间##累计金额##最后使用时间##使用次数##当前余额##删除标识##卡权限
	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, 
		endTime, pCard->fTotaluse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel, pCard->Privilge);

	//关闭文件
	fclose(fp);

	return TRUE;
}
int rechargeBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nLine = 0;
	char aBuf[CARDCHARNUM] = { 0 };
	long lPosition = 0;
	char tStart[20] = { 0 };
	char tEnd[20] = { 0 };

	//以读写方式打开文件
	fp = fopen(pPath, "r+");

	if (fp == NULL)
	{
		printf("充值时，文件打开失败！");
		return FALSE;
	}

	//遍历文件，找到卡位置
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//获取文件标识位置
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//将文件指针指向该信息
	fseek(fp, lPosition, 0);

	//时间转换
	timeToString(pBilling->tStart, tStart);
	timeToString(pBilling->tEnd, tEnd);

	//对卡信息进行写入
	//格式为;卡号##密码##上机时间##下机时间##消费金额##卡余额##消费状态##删除标识
	fprintf(fp, "%s##%s##%s##%s##%.1f##%.1f##%d##%d\n", pBilling->aCardName, pBilling->aCardPwd, tStart, tEnd,
		pBilling->fAmount, pBilling->fBalance, pBilling->nStatus, pBilling->nDel);


	//关闭文件
	fclose(fp);

	return TRUE;
}