#define _CRT_SECURE_NO_WARNINGS

#include<string.h>
#include<stdio.h>
#include "model.h"
#include "Billing_file.h"
#include "card_service.h"
#include "card_file.h"
#include "global.h"
#include "billing_service.h"
#include "service.h"

void releaseList()
{
	releaseCardList();
}
/****************************************************************************************
[函数名]：getAmount
[功能]：计算消费额度
[参数]：time_t t-下机时间
[返回值]：消费金额
*****************************************************************************************/
double getAmount(time_t t)
{
	int nSce;				//消费时长(s)
	int nMin;
	int nCount;
	double dbAmount;			//消费
	time_t tEnd = time(NULL);

	nSce = tEnd - t;
	nMin = nSce / 60;

	//计算有多少个消费单位

	if (nMin==0)
	{
		nCount = 1;
	}
	else
	{
		if (nMin % UNIT == 0)
		{
			nCount = nMin / UNIT;
		}
		else
		{
			nCount = (nMin / UNIT) + 1;
		}
	}

	dbAmount = (nCount)*CHARGE;

	return dbAmount;
	

}
