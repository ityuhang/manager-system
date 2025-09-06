#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "user_info.h"
#include "common.h"
#include "admin.h"
#include "unfinish.h"
#include "clear_input_buffer.h"
#include "passwd_confirm.h"

#define USER_INFO_FILE "data/user_info.dat"
#define USER_ID_BASE 100001

void print_user_info(user_info* ui);



// 检查到期时间，已到期返回1，未到期返回0
int check_data(user_info* ui)
{
	if(ui->expire_time <= time(NULL))
	{
		return 1;
	}
	return 0;
}

void updata_expire_data()
{
	FILE* fp = fopen(USER_INFO_FILE, "rb+");

	if(fp == NULL)
	{
		perror("读取用户信息失败");
		return;
	}

	user_info ui;
	while(fread(&ui, sizeof(ui), 1, fp) > 0)
	{
		if(check_data(&ui) == 1)
		{
			ui.out_date = 1;
			fseek(fp, -sizeof(ui), SEEK_CUR);
			fwrite(&ui, sizeof(ui), 1, fp);
		}
		else if(ui.out_date == 1 && check_data(&ui) == 0)
		{
			ui.out_date = 0;
			fseek(fp, -sizeof(ui), SEEK_CUR);
			fwrite(&ui, sizeof(ui), 1, fp);
		}
	}

	fclose(fp);
}




// 仅在当前文件使用
static unsigned int get_new_uid(void)
{
	user_info ui;
	unsigned int uid;
	
	FILE* fp = fopen(USER_INFO_FILE, "rb");  // 以二进制打开

	if(fp == NULL)
	{
		//perror("get_new_uid:fopen");
		return USER_ID_BASE;
	}

	fseek(fp, -sizeof(ui), SEEK_END);
	if(fread(&ui, sizeof(ui), 1, fp)==1)
	{
		uid = ui.user_id + 1;
	}
	else
	{
		uid = USER_ID_BASE;
	}

	fclose(fp);

	return uid;

}
void add_month(time_t* cur_t, int n) {
    struct tm* st = localtime(cur_t);
    st->tm_mon += n; // 增加n个月
    // 处理年份进位
    st->tm_year += st->tm_mon / 12;
    st->tm_mon %= 12;
    *cur_t = mktime(st); // 转换回time_t
}

void add_day(time_t* cur_t, int n) {
    struct tm* st = localtime(cur_t);
    st->tm_mday += n; // 增加n天
    *cur_t = mktime(st); // 转换回time_t
}

void add_year(time_t* cur_t, int n) {
	struct tm* st = localtime(cur_t);
	st->tm_year += n; // 增加n年
	*cur_t = mktime(st); // 转换回time_t
}

void add_week(time_t* cur_t, int n) {
    struct tm* st = localtime(cur_t);
    st->tm_mday += n * 7; // 增加n周
    *cur_t = mktime(st); // 转换回time_t
}



// 如果卡号被注册返回0， 未被注册返回1
int check_card_num(char num[])
{
	FILE* fp = fopen(USER_INFO_FILE, "r");

	if(fp == NULL)
	{
		return 1;
	}

	user_info ui;
	while(fread(&ui, sizeof(ui), 1, fp) == 1)
	{
		if(strcmp(num, ui.card_num) == 0)
		{
			return 0;
		}
	}
	if(feof(fp))
	{
		return 1;
	}
	return 0;

}


void user_reg(void)
{
	user_info ui;
	int tmp_sex;
	int tmp_type;
	system("clear");
	printf("请刷卡或者手动输入卡号:\n");
	scanf("%s", ui.card_num);

	if(!check_card_num(ui.card_num))
	{
		printf("此卡已被注册\n");
		return;
	}	
	
	printf("请输入姓名:\n");
	scanf("%s", ui.name);

	printf("请输入性别(1女, 2男):\n");
	scanf("%d", &tmp_sex);
	ui.gender = tmp_sex;

	printf("请输入手机号:\n");
	scanf("%s", ui.phone_num);

	printf("请输入卡类型:\n");
	scanf("%d", &tmp_type);
	ui.card_type = tmp_type;
	ui.reg_time = time(NULL);
	ui.expire_time = ui.reg_time;

	if(ui.card_type == daily)
	{
		add_day(&ui.expire_time, 1);
	}
	else if(ui.card_type == weekly)
	{
		add_week(&ui.expire_time, 1);
	}
	else if(ui.card_type == monthly)
	{
		add_month(&ui.expire_time, 1);
	}
	else if(ui.card_type == yearly)
	{
		add_year(&ui.expire_time, 1);
	}

	ui.ban_state = 0;
	ui.out_date = 0;
	ui.balance = 0;


	unsigned int uid = get_new_uid();
	ui.user_id = uid;

	FILE* fp = fopen(USER_INFO_FILE, "ab");

	if(fp == NULL)
	{
		perror("注册失败");
		return;
	}
	

	fwrite(&ui, sizeof(ui), 1, fp);
	fclose(fp);
	printf("会员注册成功!\n");
}



void print_user_info(user_info* ui)
{
        printf("\n");
        printf("卡号: %s\n", ui->card_num);
        //   printf("会员ID: %u\n", ui->user_id);
		printf("账户状态: ");
		if(ui->ban_state == 0)
		{
			printf("正常\n");
		}
		else
		{
			printf("已注销\n");
		}
		
		ui->out_date = check_data(ui);
		printf("是否过期: ");
		if(ui->out_date == 0)
		{
			printf("未过期\n");
		}
		else
		{
			printf("已过期\n");
		}
        printf("姓名: %s\n", ui->name);
        printf("性别: %s\n", ui->gender == female? "女" : "男");
        printf("手机号: %s\n", ui->phone_num);
		printf("会员类型: ");
        if(ui->card_type == 1)
        {
                printf("天卡\n");
        }
        else if(ui->card_type == 2)
        {
                printf("周卡\n");
        }
        else if(ui->card_type == 3)
        {
                printf("月卡\n");
        }
        else if(ui->card_type == 4)
        {
                printf("年卡\n");
        }
		printf("账户余额: %.2lf\n", ui->balance);
		
		struct tm* rt;
		rt = localtime(&ui->reg_time);	
                printf("注册时间: %d年%d月%d日 %02d:%02d\n", rt->tm_year + 1900, rt->tm_mon + 1, rt->tm_mday, rt->tm_hour, rt->tm_min);


                struct tm* dt;
                dt = localtime(&ui->expire_time);
                printf("到期时间: %d年%d月%d日 %02d:%02d\n", dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min);
	
		//press_any_key();
}


int pay(int user_id)
{
    user_info user;
    FILE* fp = fopen(USER_INFO_FILE, "r+");
    if(fp == NULL)
    {
        perror("读取用户信息失败");
        return -1; // 使用-1表示文件错误
    }
    
    int found = 0;
    long pos = 0;
    
    while(fread(&user, sizeof(user), 1, fp) == 1)
    {
        if(user.user_id == user_id) {
            found = 1;
            pos = ftell(fp) - sizeof(user);
            break;
        }
    }

    if(!found)
    {
        fclose(fp);
        printf("\n不存在此会员！\n");
        return -2; // 使用-2表示用户不存在
    }

    if(user.balance <= 0)
    {
        fclose(fp);
        return 0; // 余额不足
    }

    user.balance -= 1;
    fseek(fp, pos, SEEK_SET);
    fwrite(&user, sizeof(user), 1, fp);
    fclose(fp);
    return 1; // 支付成功
}

void user_show(void)
{
    system("clear");
    printf("\n会员请刷卡！\n");

    char card_num[11];
    scanf("%s", card_num);    

    // 管理员界面
    if(strcmp(card_num, "admin") == 0)
    {
        if(confirm_passwd()) {
            admin_menu();
        } else {
            printf("密码错误\n");
            press_any_key();
        }
        return;
    }

    // 查找用户信息
    user_info ui;
    int found = 0;
    
    FILE* fp = fopen(USER_INFO_FILE, "rb");
    if(fp == NULL)
    {
        printf("打开用户信息失败!\n");
        press_any_key();
        return;
    }

    while(fread(&ui, sizeof(ui), 1, fp) == 1)
    {
        if(strcmp(ui.card_num, card_num) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if(!found)
    {
        printf("\n卡片未注册, 请联系工作人员处理...\n");
        press_any_key();
        return;
    }

    // 检查账户状态
    if(ui.ban_state == 1) // 使用明确的常量
    {
        printf("\n该会员已注销\n");
        press_any_key();
        return;
    }
    else if(ui.ban_state == 1)
    {
        printf("\n该账户已被封禁,请联系工作人员进行处理...\n");
        press_any_key();
        return;
    }

    // 执行支付
    int pay_result = pay(ui.user_id);
    
    switch(pay_result)
    {
        case 1: // 支付成功
            ui.balance -= 1; // 更新本地余额
            ui.out_date = check_data(&ui);
            system("clear");
            print_user_info(&ui);
            break;
            
        case 0: // 余额不足
            system("clear");
            printf("\n余额不足，请充值\n");
            break;
            
        case -1: // 文件错误
            printf("\n系统错误，请重试\n");
            break;
            
        case -2: // 用户不存在
            printf("\n用户信息异常，请联系工作人员\n");
            break;
    }

    press_any_key();
}



void user_del(void)
{
	char num[50];
	user_info ui;
	unsigned int uid;
	printf("请输入要注销的会员卡号/会员ID/手机号:\n");
	scanf("%s", num);


	FILE* fp = fopen(USER_INFO_FILE, "r+");
	if(fp == NULL)
	{
		perror("读取用户信息失败");
		return;
	}
	
	while(fread(&ui, sizeof(ui), 1, fp) == 1)
	{
		if(strcmp(num, ui.card_num) == 0 || strcmp(num, ui.phone_num) == 0 || atol(num) == ui.user_id)
		{
			break;			
		}
	}

	if(feof(fp))
	{
		printf("\n不存在此会员, 注销失败！\n");
	}
	else
	{
		char select;
		printf("要注销的会员信息如下: \n");
		print_user_info(&ui);

		printf("\n是否确认注销此会员(Y/N):\n");
		char c;
		while((c = getchar()) != '\n' && c != EOF);  // 清空缓冲区

		select = getchar();

		if(select == 'Y' || select == 'y')
		{
			ui.ban_state = 1;
			fseek(fp, -sizeof(ui), SEEK_CUR);

			fwrite(&ui, sizeof(ui), 1, fp);

			printf("\n会员已注销\n");	
		}

	}

	fclose(fp);

}


void find_user(void)
{

	char num[50];
	user_info ui;
	unsigned int uid;
	while(1)
	{
		printf("请输入会员的卡号/ID/手机号(输入 0 退出):\n");
		//cb();
		scanf("%s", num);
		
		if(strcmp(num, "0") == 0)
		{
			return;
		}

		FILE* fp = fopen(USER_INFO_FILE, "r+");
		
		if(fp == NULL)
		{
		perror("读取用户信息失败");
		return;
		}

		fseek(fp, 0, SEEK_SET);

		while(fread(&ui, sizeof(ui), 1, fp) == 1)
		{
			if(strcmp(num, ui.card_num) == 0 || atol(num) == ui.user_id || strcmp(num, ui.phone_num) == 0)
			{
				break;
			}
		}

		if(feof(fp))
		{
			printf("\n不存在此会员，请重试!\n");
			fclose(fp);
			continue;
		}

		

		char select;
		system("clear");


		printf("查询到的会员信息如下:\n");

		if(ui.ban_state == 1)
		{
			printf("\n(该账户已注销)");
		}

		print_user_info(&ui);
		printf("\n修改会员信息请按3\n重新查询请按F\n");


		//cb();  // 清空缓冲区

		getchar();
		select = getchar();
		if(select == 'f' || select == 'F')
		{
			fclose(fp);
			continue;
		}

		if(select == '3')
		{
			user_mod();
		}


		fclose(fp);
	}

}