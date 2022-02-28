#pragma once
#include "model.h"
#ifndef MODEL_H
#define MODEL_H

#include<time.h>

//卡信息结构体
//char aName[18]-卡号
//char aPwd[8]-卡密码
//int nStatus-卡状态（0-未上机，1-正在上机，2-已注销，3-失效）
//time_t tSart-开卡时间
//time_t tEnd-卡的截止时间
//float fTotaluse-累计金额
//time_t tLastTime-最后使用时间
//int nUseCount-累计使用次数
//float fBalance-余额
//int Privilge-权限等级（1-用户，2-管理员）
//int nDel-删除标识（0-未删除，1-已删除）
typedef struct Card
{
	char aName[18];		//卡号
	char aPwd[8];		//卡密码
	int nStatus;		//卡状态（0-未上机，1-正在上机，2-已注销，3-失效）
	time_t tSart;		//开卡时间
	time_t tEnd;		//卡的截止时间
	float fTotaluse;	//累计金额
	time_t tLastTime;	//最后使用时间
	int nUseCount;		//累计使用次数
	float fBalance;		//余额
	int Privilge;		//权限等级（1-用户，2-管理员）
	int nDel;			//删除标识（0-未删除，1-已删除）
}Card;

//定义链表

//链表结点 CardNode-—（表头，表尾） lpCardNode-结点
//Card data-数据域
//struct CardNode* next-指针域（指向下一个节点）
typedef struct CardNode
{
	Card data;
	struct CardNode* next;

}CardNode, * lpCardNode;

//消费信息结构体
//char aCardName[18]-卡号
//char aCardPwd[8]-密码
//time_t tStart-上机时间
//time_t tEnd-下机时间
//float fAmount-消费金额
//float fBalance-卡余额
//int nStatus-消费状态，0-未结算,1-已经结算
//int nDel-删除标识，0-未删除，1-已删除
typedef struct Billing
{
	char aCardName[18];		//卡号
	char aCardPwd[8];		//密码
	time_t tStart;			//上机时间
	time_t tEnd;			//下机时间
	float fAmount;			//消费金额
	float fBalance;			//卡余额
	int nStatus;			//消费状态，0-未结算,1-已经结算
	int nDel;				//删除标识，0-未删除，1-已删除
}Billing;

//上机信息结构体
//char aCardName[18]-上机卡号
//time_t tLogon-上机时间
//float fBalance-上机前的余额
typedef struct LogonInfo
{
	char aCardName[18];		//上机卡号
	time_t tLogon;			//上机时间
	float fBalance;			//上机前的余额
}LogonInfo;

//计费信息结点
//信息数据域
//信息指针域
//BillingNode-(表头,表尾),lpBillingNode-结点
typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;

}BillingNode,*lpBillingNode;

//下机信息结构体
//char aCardName[18];	//卡号
//time_t tStart;		//上机时间
//time_t tEnd;			//下机时间
//float fAmount;		//消费金额
//float fBalance;		//余额
typedef struct SettleInfo
{
	char aCardName[18];		//卡号
	time_t tStart;			//上机时间
	time_t tEnd;			//下机时间
	float fAmount;			//消费金额
	float fBalance;			//余额

}SettleInfo;

//充值退费结构体
//char CardName[18];	//卡号
//float fMoney;		//充值退费金额
//float fBalance;		//余额
typedef struct MoneyInfo
{
	char CardName[18];			//卡号
	float fMoney;				//充值退费金额
	float fBalance;				//余额

}MoneyInfo;

//注销结构体
//char CardName[18];	//注销卡卡号
//float fMoney;			//卡剩余的金额
typedef struct LogoutInfo
{
	char CardName[18];		//注销卡卡号
	float fMoney;		//卡剩余的金额

}LogoutInfo;

#endif // !MODEL_H


