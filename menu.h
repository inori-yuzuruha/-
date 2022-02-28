#pragma once
#ifndef MENU_H
#define MENU_H

void outputMenu_m(void);	//声明输出管理员菜单函数
void outputMenu_u(void);	//声明输出消费者菜单函数
void add(void);				//声明添加卡信息函数
void query();				//声明查询卡信息函数
void logon();				//声明上机函数
void settle();				//声明下机函数
void rechage();				//声明充值函数
void refund();				//声明退费函数
void logout();				//声明注销函数
void exitApp();				//声明退出应用函数

#endif // !MENU_H