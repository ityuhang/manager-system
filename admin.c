#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"
#include "reg.h"
#include "unfinish.h"
#include "clear_input_buffer.h"


#define  MENU_SIZE 5
#define USER_INFO_FILE "./data/user_info.dat"




// 根据不同的时间进行问候
void hello_admin(void)
{
	time_t t = time(NULL);

	struct tm* st = localtime(&t);
	
	if(st->tm_hour >= 6 && st->tm_hour < 9)
	{
		printf("早上好,管理员\n准备好开始今天的工作了吗?");
	}
	else if(st->tm_hour >= 9 && st->tm_hour< 11)
	{
		printf("上午好,管理员\n工作辛苦了, 休息一下吧!");
	}	
	else if(st->tm_hour >= 11 && st->tm_hour < 13)
	{
		printf("中午好,管理员\n吃个午饭休息一下吧!");

	}
	 else if(st->tm_hour >= 13 && st->tm_hour < 18)
        {
                printf("下午好,管理员\n工作继续加油!");
        }
        else if(st->tm_hour >= 18 && st->tm_hour < 21)
        {
                printf("晚上好,管理员\n享受愉快的晚间时光吧!");
        }
        else if(st->tm_hour >= 21 && st->tm_hour < 23)
        {
                printf("晚上好,管理员\n注意休息，别工作太晚哦!");
        }
        else // 23:00-6:00
        {
                printf("夜深了,管理员\n请好好休息，晚安!");
        }
	printf("\n");
}

void user_mod(void)
{
        char num[50];
        user_info ui;
        unsigned int uid;
        printf("请输入要修改的会员卡号/会员ID/手机号:\n");
        scanf("%s", num);


        FILE* fp = fopen(USER_INFO_FILE, "r+");

        while(fread(&ui, sizeof(ui), 1, fp) == 1)
        {
            if(strcmp(num, ui.card_num) == 0 || strcmp(num, ui.phone_num) == 0 || atol(num) == ui.user_id)
			{	
                break;
            }
        }

        if(feof(fp))
        {
                printf("\n不存在此会员, 请重试\n");
        }
        else
        {
                char info[][20] = {{"卡号"}, {"姓名"}, {"性别"}, {"手机号"}, {"会员类型"}, {"账户状态"}, {"注销状态"}, {"账户余额"},  {"到期时间"}};
				user_info user = ui; // 复制一份原始数据
		char select;
                printf("要修改的会员信息如下, 请选择要修改的项目: \n");
                print_user_info(&ui);
		
		for(int i = 0; i < 9; i++)
		{
			printf("\n请输入 %d 来修改 %s\n", i + 1, info[i]);
		}
		
		getchar();
		scanf("%c", &select);
		printf("请输入新的 %s\n", info[(select - '0') - 1]);
		getchar();	

		if(select == '1')
		{
			printf("\n请刷卡\n");
			scanf("%s", user.card_num);
		}
		else if(select == '2')
		{
			scanf("%s", user.name);	
		}
		else if(select == '3')
		{
			printf("0女 1男\n");
			scanf("%d", (int *)&user.gender);
		}
		else if(select == '4')
		{
			scanf("%s", user.phone_num);
		}
		else if(select == '5')
		{
			scanf("%u", &user.card_type);
		}
		else if(select == '6')
		{
			printf("0正常 1禁用");
			scanf("%d", &user.ban_state);
		}
		else if(select == '7')
		{
			printf("0未注销 1已注销\n");
			scanf("%d", &(user.ban_state));
		}
		else if(select == '8')
		{
			scanf("%lf", &user.balance);
		}
		else if(select == '9')
		{
			printf("格式: 2025-1-1\n");	
		}



                printf("\n是否确认修改此会员(Y/N):\n");
                char c;
                while((c = getchar()) != '\n' && c != EOF);  // 清空缓冲区

                select = getchar();
				int succeed = 0;
                if(select == 'Y' || select == 'y')
                {
               
                        fseek(fp, -sizeof(ui), SEEK_CUR);

                        fwrite(&user, sizeof(user), 1, fp);
						succeed = 1;
                        printf("\n会员修改成功!\n");
                }
				if(succeed == 0) printf("会员修改失败!\n");
				press_any_key();

        }

        fclose(fp);

}
// 展示所有用户，包括已注销用户
void show_all_users()
{
	user_info ui;

	FILE* fp = fopen(USER_INFO_FILE, "rb");
	
	if(fp == NULL)
	{
		printf("用户信息读取失败\n");
		return;
	}

	while(fread(&ui, sizeof(ui), 1, fp) > 0)
	{
		print_user_info(&ui);
	}

}




void admin_menu(void)
{
	while(1)
	{
		system("clear");

		hello_admin();
		printf("\n");
		char menu[MENU_SIZE][50] = {"注册用户", "查找并修改用户", "用户注销", "展示所有用户", "退出系统"};

		for(int i = 0; i < MENU_SIZE; i++)
		{
			printf("请输入 %d 进行 %s\n", i + 1, menu[i]);
		}

		int op;
		scanf("%d", &op);
		if(op == 1)
		{
			user_reg();
		}
		else if(op == 2)
		{
			find_user();
		}
		else if(op == 3)
		{
			user_del();
		}
		else if(op == 4)
		{
				show_all_users();
		}
		else if(op == 5)
		{
			break;
		}

		press_any_key();
	}
}


