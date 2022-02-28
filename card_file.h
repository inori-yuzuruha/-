#pragma once
#include "model.h"
#ifndef CARD_FILE_H
#define CARD_FILE_H

int saveCard(const Card* pCard, const char* pPath);								//声明保存函数
int getCardCount(const char* pPath);											//声明获取文件中卡数量函数
int readCard(Card* pCard, const char* pPath);									//声明读取卡文件中的卡信息函数
int updateCard(const Card* pCard, const char* pPath, int nIndex);				//声明更新卡文件中卡信息函数
Card praseCard(char* pBuf);														//声明解析卡文件中卡信息函数
int rechargeCard(const Card*, const char* pPath, int nIndex);					//声明充值退费时更新卡文件中卡信息函数
int rechargeBilling(const Billing* pBilling, const char* pPath, int nIndex);	//声明充值退费时更新消费文件中卡信息函数

#endif // !GLOBAL_H
