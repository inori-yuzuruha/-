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
[������]��getAmount
[����]���������Ѷ��
[����]��time_t t-�»�ʱ��
[����ֵ]�����ѽ��
*****************************************************************************************/
double getAmount(time_t t)
{
	int nSce;				//����ʱ��(s)
	int nMin;
	int nCount;
	double dbAmount;			//����
	time_t tEnd = time(NULL);

	nSce = tEnd - t;
	nMin = nSce / 60;

	//�����ж��ٸ����ѵ�λ

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
