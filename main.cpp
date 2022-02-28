#include<stdio.h>
#include"menu.h"
#include"card_service.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char mName[18] = { 0 };
	char mPwd[8] = { 0 };
	int nSelection_m = -1;			//�������Ա�˵�����
	int nSelection_u = -1;			//�����û��˵�����
	int Index = 0;
	Card* pCard=NULL;
	int flag = 0;					//���������������

	pCard = (Card*)malloc(sizeof(Card));

	printf("------��¼ϵͳ------\n");

	printf("�������¼���ţ�");
	scanf_s("%s", mName, sizeof(mName));

	while ((pCard = queryCards(mName, &Index)) == NULL)
	{

		printf("δ��ѯ���ÿ��ţ����������룺");
		scanf_s("%s", mName, sizeof(mName));
		Index = 0;
	}



	printf("�������¼���룺");
	scanf_s("%s", mPwd, sizeof(mPwd));

	while (strcmp(pCard->aPwd,mPwd)!=0)
	{
		printf("��������������������룺");
		scanf_s("%s", mPwd, sizeof(mPwd));
		flag++;
		if (flag>5)
		{
			printf("�����������������࣬���˳�����ллʹ��\n");
			return 0;
		}
	}

	if (pCard->Privilge==2)
	{
		//TODO�����ϵͳ����--��ӭ����Ʒѹ���ϵͳ
		printf("����Ա��%s��",mName);
		printf("��ӭ����Ʒѹ���ϵͳ\n");

		do
		{
			//���ϵͳ�˵�
			outputMenu_m();

			//�ͷ�scanf_s������

			//����˵�����Ϊ-1
			scanf_s("%d", &nSelection_m);

			//�ͷ�scanf_s������
			scanf_s("%*[^\n]%*c");

			//���ѡ��Ĳ˵����
			switch (nSelection_m)
			{
			case 1:		//��ӿ�
			{
				add();
				break;
			}
			case 2:		//��ѯ��
			{
				printf("------��ѯ��------\n");
				query();
				break;
			}
			case 3:		//�ϻ�
			{
				printf("------�ϻ�------\n");
				logon();
				break;
			}
			case 4:		//�»�
			{
				printf("------�»�------\n");
				settle();
				break;
			}
			case 5:		//��ֵ
			{
				printf("------��ֵ------\n");
				rechage();
				break;
			}
			case 6:		//�˷�
			{
				printf("------�˷�------\n");
				refund();
				break;
			}
			case 7:		//��ѯͳ��
			{
				printf("------Ȩ�޹���------\n");
				break;
			}
			case 8:		//ע����
			{
				printf("------ע����------\n");
				logout();
				break;
			}
			case 0:		//�˳�
			{
				exitApp();
				printf("���˳�\nлл����ʹ�ã�");
			} break;
			default:printf("����Ĳ˵���Ŵ���\n"); break;		//�������
			}
		} while (nSelection_m != 0);
	}
	if (pCard->Privilge==1)
	{
		//TODO����ӭʹ��
		printf("\n��ӭʹ��\n");

		do
			{
				//���ϵͳ�˵�
				outputMenu_u();

				//�ͷ�scanf_s������

				//����˵�����Ϊ-1
				scanf_s("%d", &nSelection_u);

				//�ͷ�scanf_s������
				scanf_s("%*[^\n]%*c");

				//���ѡ��Ĳ˵����
				switch (nSelection_u)
				{
				case 1:		//��ѯ��
				{
					printf("------��ѯ��------\n");
					query();
					break;
				}
				case 2:		//�ϻ�
				{
					printf("------�ϻ�------\n");
					logon();
					break;
				}
				case 3:		//�»�
				{
					printf("------�»�------\n");
					settle();
					break;
				}
				case 4:		//ע����
				{
					printf("------ע����------\n");
					logout();
					break;
				}
				case 0:		//�˳�
				{
					exitApp();
					printf("���˳�\nлл����ʹ�ã�");
				} break;
				default:printf("����Ĳ˵���Ŵ���\n"); break;		//�������
				}
			} while (nSelection_u != 0);
	}

	return 0;
}