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
	char startTime[TINMELENGTH] = { '0' };			//����ʱ��
	char endTime[TINMELENGTH] = { '0' };				//��ֹʱ��
	char lastTime[TINMELENGTH] = { '0' };			//���ʹ��ʱ��

	//���ļ�
	if ((fp = fopen(pPath, "a")) == NULL)
	{
		fp = fopen(pPath, "w");
		if (fp == NULL)
		{
			printf("���ļ�ʧ��\n");
			return FALSE;
		}
	}

	//��ʱ��ת��Ϊ�ַ���
	timeToString(pCard->tSart, startTime);			//ת������ʱ��
	timeToString(pCard->tEnd, endTime);			//ת����ֹʱ��
	timeToString(pCard->tSart, lastTime);			//ת�����ʹ��ʱ��

	//������д���ļ�
	//��ʽΪ������##����##״̬##����ʱ��##��ֹʱ��##�ۼƽ��##���ʹ��ʱ��##ʹ�ô���##��ǰ���##ɾ����ʶ##��Ȩ��

	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotaluse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel,pCard->Privilge);

	//�ر��ļ�
	fclose(fp);

	return TRUE;
}
/****************************************************************************************
[������]��readCard
[����]����pPath�ļ��е���Ϣ���б��浽pCard[i]��
[����]��Ҫ��������飬Ҫ��ȡ�ļ���·��
[����ֵ]��TURE-�ɹ���FALSE-ʧ��
*****************************************************************************************/
int readCard(Card* pCard,const char* pPath)
{
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };
	int i = 0;

	//���ļ�
	fp = fopen(pPath, "r");
	if (fp==NULL)
	{
		printf("�ļ���ʧ�ܣ�");
		return FALSE;
	}

	//��ȡ�ļ������ļ��еĿ���Ϣ���ж���pCard[i]��
	while ((!feof(fp)))
	{
		memset(aBuf, 0, CARDCHARNUM);				//�������	

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
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}
/****************************************************************************************
[������]��Card praseCard(char* pBuf)
[����]����������,�����ļ��е���Ϣ��������
[����]��char* pBuf
[����ֵ]���������Ŀ��ṹ���ַ
*****************************************************************************************/
Card praseCard(char* pBuf)
{
	Card card;
	const char* delines = "##";			//�����ַ����еķָ��
	char* buf = NULL;
	char* str = NULL;
	char flag[11][20] = { 0 };			//����ָ����ַ���
	int index = 0;

	buf = pBuf;
	
	while (((str = strtok(buf, delines))!=NULL)&&(index<=10))
	{
			strcpy(flag[index], str);
			buf = NULL;				//�������strtok�����ǵ�һ������ΪNULL
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
[������]��updateCard
[����]��������д���ļ���ʶ���Ŀ���Ϣ��
[����]��Card* pCard(Դ�ļ�) char* pPath(д��������ļ����ļ�·��) int nIndex(Ҫд����ļ���ʶ)
[����ֵ]��TRUE-д��ɹ���FALSE-д��ʧ��
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

	//�Զ�д��ʽ���ļ�
	fp = fopen(pPath, "r+");

	if (fp == NULL)
	{
		printf("�ļ���ʧ�ܣ�");
		return FALSE;
	}

	//�����ļ����ҵ�������¼�����и���

	while ((!feof(fp))&&(nLine<nIndex))
	{
		if (fgets(aBuf,CARDCHARNUM,fp)!=NULL)
		{
			//��ȡ�ļ���ʶλ��
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//�ƶ����ļ���ʶλ��
	fseek(fp, lPosition, 0);

	//������д���ļ�

	//��ʱ��ת��Ϊ�ַ���
	timeToString(pCard->tSart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLastTime, lastTime);			//ת�����ʹ��ʱ��

	//��ʽΪ������##����##%d״̬##����ʱ��##��ֹʱ��##�ۼƽ��##���ʹ��ʱ��##ʹ�ô���##��ǰ���##ɾ����ʶ##��Ȩ��
	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotaluse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel,pCard->Privilge) ;

	//�ر��ļ�
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

	//�Զ�д��ʽ���ļ�
	fp = fopen(pPath, "r+");

	if (fp==NULL)
	{
		printf("�˷�ʱ���ļ���ʧ�ܣ�");
		return FALSE;
	}

	//�����ļ����ҵ���λ��
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//��ȡ�ļ���ʶλ��
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//���ļ�ָ��ָ�����Ϣ
	fseek(fp, lPosition, 0);

	//ʱ��ת��
	timeToString(pCard->tSart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLastTime, lastTime);

	//�Կ���Ϣ����д��
	//��ʽΪ������##����##״̬##����ʱ��##��ֹʱ��##�ۼƽ��##���ʹ��ʱ��##ʹ�ô���##��ǰ���##ɾ����ʶ##��Ȩ��
	fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, 
		endTime, pCard->fTotaluse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel, pCard->Privilge);

	//�ر��ļ�
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

	//�Զ�д��ʽ���ļ�
	fp = fopen(pPath, "r+");

	if (fp == NULL)
	{
		printf("��ֵʱ���ļ���ʧ�ܣ�");
		return FALSE;
	}

	//�����ļ����ҵ���λ��
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//��ȡ�ļ���ʶλ��
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//���ļ�ָ��ָ�����Ϣ
	fseek(fp, lPosition, 0);

	//ʱ��ת��
	timeToString(pBilling->tStart, tStart);
	timeToString(pBilling->tEnd, tEnd);

	//�Կ���Ϣ����д��
	//��ʽΪ;����##����##�ϻ�ʱ��##�»�ʱ��##���ѽ��##�����##����״̬##ɾ����ʶ
	fprintf(fp, "%s##%s##%s##%s##%.1f##%.1f##%d##%d\n", pBilling->aCardName, pBilling->aCardPwd, tStart, tEnd,
		pBilling->fAmount, pBilling->fBalance, pBilling->nStatus, pBilling->nDel);


	//�ر��ļ�
	fclose(fp);

	return TRUE;
}