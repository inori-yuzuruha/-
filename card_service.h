#pragma once
#include"model.h"
#ifndef CARD_SEVICE_H
#define CARD_SEVICE_H

int addCard(Card card);														//声明添加卡函数			
int getCard();																//声明获取卡信息函数
void releaseCardList();														//声明释放链表结点函数
void exitApp();																//声明退出应用函数
Card* queryCards(const char* pName, int* pIndex);							//声明查询卡信息函数
int dorecharge(const char* pName, float Money, MoneyInfo* pLogonInfo);		//声明充值费数

#endif // !CARD_SEVICE_H