#include<stdio.h>
#include"menu.h"
#include"card_service.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char mName[18] = { 0 };
	char mPwd[8] = { 0 };
	int nSelection_m = -1;			//输入管理员菜单项编号
	int nSelection_u = -1;			//输入用户菜单项编号
	int Index = 0;
	Card* pCard=NULL;
	int flag = 0;					//限制密码输入次数

	pCard = (Card*)malloc(sizeof(Card));

	printf("------登录系统------\n");

	printf("请输入登录卡号：");
	scanf_s("%s", mName, sizeof(mName));

	while ((pCard = queryCards(mName, &Index)) == NULL)
	{

		printf("未查询到该卡号，请重新输入：");
		scanf_s("%s", mName, sizeof(mName));
		Index = 0;
	}



	printf("请输入登录密码：");
	scanf_s("%s", mPwd, sizeof(mPwd));

	while (strcmp(pCard->aPwd,mPwd)!=0)
	{
		printf("密码输入错误，请重新输入：");
		scanf_s("%s", mPwd, sizeof(mPwd));
		flag++;
		if (flag>5)
		{
			printf("密码输入错误次数过多，已退出程序！谢谢使用\n");
			return 0;
		}
	}

	if (pCard->Privilge==2)
	{
		//TODO：输出系统名称--欢迎进入计费管理系统
		printf("管理员：%s，",mName);
		printf("欢迎进入计费管理系统\n");

		do
		{
			//输出系统菜单
			outputMenu_m();

			//释放scanf_s缓冲区

			//输入菜单项编号为-1
			scanf_s("%d", &nSelection_m);

			//释放scanf_s缓冲区
			scanf_s("%*[^\n]%*c");

			//输出选择的菜单编号
			switch (nSelection_m)
			{
			case 1:		//添加卡
			{
				add();
				break;
			}
			case 2:		//查询卡
			{
				printf("------查询卡------\n");
				query();
				break;
			}
			case 3:		//上机
			{
				printf("------上机------\n");
				logon();
				break;
			}
			case 4:		//下机
			{
				printf("------下机------\n");
				settle();
				break;
			}
			case 5:		//充值
			{
				printf("------充值------\n");
				rechage();
				break;
			}
			case 6:		//退费
			{
				printf("------退费------\n");
				refund();
				break;
			}
			case 7:		//查询统计
			{
				printf("------权限管理------\n");
				break;
			}
			case 8:		//注销卡
			{
				printf("------注销卡------\n");
				logout();
				break;
			}
			case 0:		//退出
			{
				exitApp();
				printf("已退出\n谢谢您的使用！");
			} break;
			default:printf("输入的菜单序号错误\n"); break;		//输入错误
			}
		} while (nSelection_m != 0);
	}
	if (pCard->Privilge==1)
	{
		//TODO：欢迎使用
		printf("\n欢迎使用\n");

		do
			{
				//输出系统菜单
				outputMenu_u();

				//释放scanf_s缓冲区

				//输入菜单项编号为-1
				scanf_s("%d", &nSelection_u);

				//释放scanf_s缓冲区
				scanf_s("%*[^\n]%*c");

				//输出选择的菜单编号
				switch (nSelection_u)
				{
				case 1:		//查询卡
				{
					printf("------查询卡------\n");
					query();
					break;
				}
				case 2:		//上机
				{
					printf("------上机------\n");
					logon();
					break;
				}
				case 3:		//下机
				{
					printf("------下机------\n");
					settle();
					break;
				}
				case 4:		//注销卡
				{
					printf("------注销卡------\n");
					logout();
					break;
				}
				case 0:		//退出
				{
					exitApp();
					printf("已退出\n谢谢您的使用！");
				} break;
				default:printf("输入的菜单序号错误\n"); break;		//输入错误
				}
			} while (nSelection_u != 0);
	}

	return 0;
}