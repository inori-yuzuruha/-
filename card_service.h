#pragma once
#include"model.h"
#ifndef CARD_SEVICE_H
#define CARD_SEVICE_H

int addCard(Card card);														//������ӿ�����			
int getCard();																//������ȡ����Ϣ����
void releaseCardList();														//�����ͷ������㺯��
void exitApp();																//�����˳�Ӧ�ú���
Card* queryCards(const char* pName, int* pIndex);							//������ѯ����Ϣ����
int dorecharge(const char* pName, float Money, MoneyInfo* pLogonInfo);		//������ֵ����

#endif // !CARD_SEVICE_H