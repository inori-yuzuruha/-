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

//���Ʒ���Ϣ���浽billion.txt�ļ���
/****************************************************************************************
[������]��saveBilling
[����]������Ʒ���Ϣ
[����]��Billing* pBilling(���������Ϣ)��char* pPath(����·��)
[����ֵ]����ȷִ�з���TRUE ����ִ�з���FALSE
*****************************************************************************************/
int saveBilling(const Billing* pBilling,const char* pPath)
{
	FILE* fp = NULL;
	char tStart[20] = { 0 };
	char tEnd[20] = { 0 };

	//��׷�ӵķ�ʽ��һ���ı��ļ�
	if ((fp = fopen(pPath, "a")) == NULL)
	{
		fp = fopen(pPath, "w");
		if (fp == NULL)
		{
			printf("���ļ�ʧ��\n");
			return FALSE;
		}
	}

	//ת��ʱ���ʽΪ�ַ���
	timeToString(pBilling->tStart, tStart);
	timeToString(pBilling->tEnd, tEnd);


	//���Ʒ���Ϣд���ı����ļ���
	//��ʽΪ ����##����##�ϻ�ʱ��##�»�ʱ��##���ѽ��##�����##����״̬##ɾ����ʶ
	fprintf(fp, "%s##%s##%s##%s##%.1f##%.1f##%d##%d\n", pBilling->aCardName, pBilling->aCardPwd, tStart, tEnd,
		pBilling->fAmount, pBilling->fBalance, pBilling->nStatus, pBilling->nDel);

	//�ر��ļ�
	fclose(fp);
	return TRUE;
}
/****************************************************************************************
[������]��readBillings
[����]����ȡ�Ʒ��ļ������еļƷ���Ϣ
[����]��Billing* pBilling,char* pPath
[����ֵ]����ȷִ�з���TRUE ����ִ�з���FALSE
*****************************************************************************************/
int readBillings(Billing* pBilling, const char* pPath)
{
	//��ȡ�Ʒ��ļ������еļƷ���Ϣ
	char aBuf[CARDCHARNUM] = { 0 };
	int i = 0;

	FILE* fp = NULL;  // �ļ�ָ��

	if ((fp = fopen(pPath, "r")) == NULL) {
		printf("���ļ�ʧ��!\n");
		return FALSE;

	}
	while ((!feof(fp)))
	{
		memset(aBuf, 0, CARDCHARNUM);				//�������	



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
[������]��getBillingCount
[����]����ȡ�ļ��еļƷ���Ϣ����
[����]��char* pPath(����ȡ�ļ���·��)
[����ֵ]����ȷִ�з���TRUE ����ִ�з���FALSE
*****************************************************************************************/
int getBillingCount(const char* pPath)
{
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };
	int nCount = 0;

	//���ı��ļ�
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("�ļ���ʧ�ܣ�");
		return FALSE;
	}

	//��ȡ�ı��ļ�
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
/****************************************************************************************
[������]��updateBillingCount
[����]�������ļ��еļƷ���Ϣ����
[����]��Billing* pBilling(Ҫ��ӽ������ļ�����Ϣ) char* pPath(����ȡ�ļ���·��) int nIndex(��ȡ��Ϣ����Ϊ0)
[����ֵ]����ȷִ�з���TRUE ����ִ�з���FALSE
*****************************************************************************************/
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nLine = 0;
	char aBuf[CARDCHARNUM] = { 0 };
	long lPosition = 0;
	char tStart[20] = { '0' };
	char tEnd[20] = { '0' };

	//��ֻ����ʽ���ļ�
	fp = fopen(pPath, "r+");

	if (fp == NULL)
	{
		printf("�ļ���ʧ�ܣ�");
		return FALSE;
	}

	//�����ļ����ҵ�������¼�����и���

	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//��ȡ�ļ���ʶλ��
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//�ƶ����ļ���ʶλ��
	fseek(fp, lPosition, 0);

	//ת�����ڸ�ʽ
	timeToString(pBilling->tStart, tStart);
	timeToString(pBilling->tEnd, tEnd);

	//��ʽΪ������##����##�ϻ�ʱ��##�»�ʱ��##���ѽ��##�����##����״̬##ɾ����ʶ
	fprintf(fp, "%s##%s##%s##%s##%.1f##%.1f##%d##%d\n", pBilling->aCardName, pBilling->aCardPwd, tStart, tEnd,
		pBilling->fAmount,pBilling->fBalance, pBilling->nStatus, pBilling->nDel);
	
	//�ر��ļ�
	fclose(fp);

	return TRUE;

}
/****************************************************************************************
[������]��praseBiiling
[����]�����������ļ��еļƷ���Ϣ����
[����]��char* pBuf(Ҫ��������Ϣ)
[����ֵ]����ȷִ�з��ؽ����󱣴�ΪBilling�ṹ�����Ϣ
*****************************************************************************************/
Billing praseBiiling(char* pBuf)
{
	Billing billing;
	const char* delines = "##";			//�����ַ����еķָ��
	char* buf = NULL;
	char* str = NULL;
	char flag[10][20] = { 0 };			//����ָ����ַ���
	int index = 0;

	buf = pBuf;

	while (((str = strtok(buf, delines)) != NULL)&&(index<=8))
	{
		strcpy(flag[index], str);
		buf = NULL;					//�������strtok�����ǵ�һ������ΪNULL
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