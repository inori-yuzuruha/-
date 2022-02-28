#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include<string.h>

#include "global.h"
#include "model.h"
#include <stdlib.h>
#include "tool.h"
#include "service.h"

#define CARDCHARNUM 256

Billing praseBiiling(char* pBuf);

//将计费信息保存到billion.txt文件中
/****************************************************************************************
[函数名]：saveBilling
[功能]：保存计费信息
[参数]：Billing* pBilling(待保存的信息)，char* pPath(保存路径)
[返回值]：正确执行返回TRUE 错误执行返回FALSE
*****************************************************************************************/
int saveBilling(const Billing* pBilling,const char* pPath)
{
	FILE* fp = NULL;
	char tStart[20] = { 0 };
	char tEnd[20] = { 0 };

	//以追加的方式打开一个文本文件
	if ((fp = fopen(pPath, "a")) == NULL)
	{
		fp = fopen(pPath, "w");
		if (fp == NULL)
		{
			printf("打开文件失败\n");
			return FALSE;
		}
	}

	//转换时间格式为字符串
	timeToString(pBilling->tStart, tStart);
	timeToString(pBilling->tEnd, tEnd);


	//将计费信息写进文本文文件中
	//格式为 卡号##密码##上机时间##下机时间##消费金额##卡余额##消费状态##删除标识
	fprintf(fp, "%s##%s##%s##%s##%.1f##%.1f##%d##%d\n", pBilling->aCardName, pBilling->aCardPwd, tStart, tEnd,
		pBilling->fAmount, pBilling->fBalance, pBilling->nStatus, pBilling->nDel);

	//关闭文件
	fclose(fp);
	return TRUE;
}
/****************************************************************************************
[函数名]：readBillings
[功能]：读取计费文件中所有的计费信息
[参数]：Billing* pBilling,char* pPath
[返回值]：正确执行返回TRUE 错误执行返回FALSE
*****************************************************************************************/
int readBillings(Billing* pBilling, const char* pPath)
{
	//读取计费文件中所有的计费信息
	char aBuf[CARDCHARNUM] = { 0 };
	int i = 0;

	FILE* fp = NULL;  // 文件指针

	if ((fp = fopen(pPath, "r")) == NULL) {
		printf("打开文件失败!\n");
		return FALSE;

	}
	while ((!feof(fp)))
	{
		memset(aBuf, 0, CARDCHARNUM);				//清空数组	



		if ((fgets(aBuf, CARDCHARNUM, fp)) == NULL)
		{
			break;
		}
		if (strlen(aBuf) > 0)
		{
			pBilling[i] = praseBiiling(aBuf);
			i++;
		}
	}
	fclose(fp);
	return TRUE;
}
/****************************************************************************************
[函数名]：getBillingCount
[功能]：读取文件中的计费信息数量
[参数]：char* pPath(待读取文件的路径)
[返回值]：正确执行返回TRUE 错误执行返回FALSE
*****************************************************************************************/
int getBillingCount(const char* pPath)
{
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };
	int nCount = 0;

	//打开文本文件
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("文件打开失败！");
		return FALSE;
	}

	//读取文本文件
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
/****************************************************************************************
[函数名]：updateBillingCount
[功能]：更新文件中的计费信息数量
[参数]：Billing* pBilling(要添加进消费文件的信息) char* pPath(待读取文件的路径) int nIndex(读取信息参数为0)
[返回值]：正确执行返回TRUE 错误执行返回FALSE
*****************************************************************************************/
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nLine = 0;
	char aBuf[CARDCHARNUM] = { 0 };
	long lPosition = 0;
	char tStart[20] = { '0' };
	char tEnd[20] = { '0' };

	//以只读方式打开文件
	fp = fopen(pPath, "r+");

	if (fp == NULL)
	{
		printf("文件打开失败！");
		return FALSE;
	}

	//遍历文件，找到该条记录，进行更新

	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//获取文件标识位置
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//移动到文件标识位置
	fseek(fp, lPosition, 0);

	//转换日期格式
	timeToString(pBilling->tStart, tStart);
	timeToString(pBilling->tEnd, tEnd);

	//格式为：卡号##密码##上机时间##下机时间##消费金额##卡余额##消费状态##删除标识
	fprintf(fp, "%s##%s##%s##%s##%.1f##%.1f##%d##%d\n", pBilling->aCardName, pBilling->aCardPwd, tStart, tEnd,
		pBilling->fAmount,pBilling->fBalance, pBilling->nStatus, pBilling->nDel);
	
	//关闭文件
	fclose(fp);

	return TRUE;

}
/****************************************************************************************
[函数名]：praseBiiling
[功能]：解析消费文件中的计费信息数量
[参数]：char* pBuf(要解析的信息)
[返回值]：正确执行返回解析后保存为Billing结构体的信息
*****************************************************************************************/
Billing praseBiiling(char* pBuf)
{
	Billing billing;
	const char* delines = "##";			//定义字符串中的分割符
	char* buf = NULL;
	char* str = NULL;
	char flag[10][20] = { 0 };			//保存分割后的字符串
	int index = 0;

	buf = pBuf;

	while (((str = strtok(buf, delines)) != NULL)&&(index<=8))
	{
		strcpy(flag[index], str);
		buf = NULL;					//后面调用strtok函数是第一个参数为NULL
		index++;

	}

	strcpy(billing.aCardName, flag[0]);
	strcpy(billing.aCardPwd, flag[1]);
	billing.tStart = stringToTime(flag[2]);
	billing.tEnd= stringToTime(flag[3]);
	billing.fAmount = (float)atof(flag[4]);
	billing.fBalance = (float)atof(flag[5]);
	billing.nStatus = atoi(flag[6]);
	billing.nDel = atoi(flag[7]);

	return billing;
}