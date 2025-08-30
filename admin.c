#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"
#include "reg.h"
#include "unfinish.h"
#include "clear_input_buffer.h"


#define  MENU_SIZE 6
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
		getchar();

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
                printf("\n不存在此会员, 请重试\n");
        }
        else
        {
            char info[][20] = {{"卡号"}, {"姓名"}, {"性别"}, {"手机号"}, {"会员类型"}, {"账户状态"}, {"注销状态"}, {"账户余额"},  {"到期时间"}};
			char select;
            printf("要修改的会员信息如下, 请选择要修改的项目: \n");
            print_user_info(&ui);
		
		for(int i = 0; i < 9; i++)
		{
			printf("\n请输入 %d 来修改 %s\n", i + 1, info[i]);
		}
		

		scanf("%c", &select);
		getchar();	
		printf("请输入新的 %s\n", info[(select - '0') - 1]);


		if(select == '1')
		{
			printf("\n请刷卡\n");
			scanf("%s", ui.card_num);
		}
		else if(select == '2')
		{
			scanf("%s", ui.name);	
		}
		else if(select == '3')
		{
			printf("0女 1男\n");
			scanf("%d", (int *)&ui.gender);
		}
		else if(select == '4')
		{
			scanf("%s", ui.phone_num);
		}
		else if(select == '5')
		{
			scanf("%u", &ui.card_type);
		}
		else if(select == '6')
		{
			printf("0正常 1禁用");
			scanf("%d", &ui.ban_state);
		}
		else if(select == '7')
		{
			printf("0未注销 1已注销\n");
			scanf("%d", &(ui.ban_state));
		}
		else if(select == '8')
		{
			scanf("%lf", &ui.balance);
		}
		else if(select == '9')
		{
			printf("格式: 2025-1-1\n");	
			char date[20];
			scanf("%s", date);
			int year, month, day;
			sscanf(date, "%d-%d-%d", &year, &month, &day);
			struct tm tm = {0};
			tm.tm_year = year - 1900;
			tm.tm_mon = month - 1;
			tm.tm_mday = day;
			ui.expire_time = mktime(&tm);


            // 更新过期状态
            if(ui.expire_time > time(NULL))
            {
                ui.out_date = 0;
            }
            else
            {
                ui.out_date = 1;
            }
		}



                printf("\n是否确认修改此会员(Y/N):\n");
                char c;
                while((c = getchar()) != '\n' && c != EOF);  // 清空缓冲区

                select = getchar();
				int succeed = 0;
                if(select == 'Y' || select == 'y')
                {
                    fseek(fp, -sizeof(ui), SEEK_CUR);
                    fwrite(&ui, sizeof(ui), 1, fp);
					succeed = 1;
                    printf("\n会员修改成功!\n");
                }
				if(succeed == 0) printf("会员修改失败!\n");
        }

        fclose(fp);

}





// 辅助函数：将时间戳转换为格式化字符串
void format_time(time_t t, char* buffer, int buffer_size) {
    if (t == 0) {
        snprintf(buffer, buffer_size, "未设置");
        return;
    }
    struct tm *tm_info = localtime(&t);
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M", tm_info);
}


// 辅助函数：获取性别字符串
const char* get_gender_str(sex g) {
    switch(g) {
        case female: return "女";
        case male: return "男";
        case unknow: return "未知";
        default: return "未知";
    }
}

// 辅助函数：获取卡类型字符串
const char* get_card_type_str(package p) {
    switch(p) {
        case daily: return "天卡";
        case weekly: return "周卡";
        case monthly: return "月卡";
        case yearly: return "年卡";
        default: return "未知";
    }
}

// 辅助函数：获取状态字符串
const char* get_state_str(int state) {
    return state ? "是" : "否";
}


// 计算字符串的显示宽度（UTF-8编码，汉字算2个字符宽度，ASCII字符算1个）
int get_display_width(const char *str) {
    int width = 0;
    const char *p = str;
    
    while (*p) {
        if ((*p & 0x80) == 0) {
            // ASCII字符，占1个宽度
            width += 1;
            p += 1;
        } 
        
        // else if ((*p & 0xE0) == 0xC0) {
        //     // 2字节UTF-8字符（通常为欧洲语言字符）
        //     width += 1; // 假设欧洲字符宽度为1
        //     p += 2;
        // } 
        
        else if ((*p & 0xF0) == 0xE0) {
            // 3字节UTF-8字符（通常为中文字符）
            width += 2; // 中文字符宽度为2
            p += 3;
        } 
        
        // else if ((*p & 0xF8) == 0xF0) {
        //     // 4字节UTF-8字符（特殊字符或表情）
        //     width += 2; // 假设宽度为2
        //     p += 4;
        // } 
        
        else {
            // 无效的UTF-8序列，跳过1字节
            p += 1;
        }
    }
    return width;
}

// 格式化显示宽度
void format_field(char *dest, const char *src, int max_width) {
    // 计算当前显示宽度
    int current_width = get_display_width(src);
    
    // 复制原始内容
    strcpy(dest, src);
    
    // 如果当前宽度小于最大宽度，添加空格
    if (current_width < max_width) {
        int i;
        for (i = strlen(dest); current_width < max_width; current_width++, i++) {
            dest[i] = ' ';
        }
        dest[i] = '\0';
    }
	
}


// 展示所有用户，包括已注销用户
void show_all_users()
{
    user_info u;
    FILE* fp = fopen(USER_INFO_FILE, "rb");
    
    if(fp == NULL)
    {
        perror("用户信息读取失败");
        return;
    }
    

    system("clear");
    // 表头 - 每列固定20字符宽度
    printf("┌────────────────────┬────────────────────┬────────────────────┬────────────────────┬────────────────────┬────────────────────┬────────────────────┬────────────────────┬────────────────────┬────────────────────┬────────────────────┐\n");
    printf("│      用户ID        │       卡号         │       姓名         │       性别         │      手机号        │      卡类型        │      已禁用        │      已过期        │       余额         │     注册时间       │     到期时间       │\n");
    printf("├────────────────────┼────────────────────┼────────────────────┼────────────────────┼────────────────────┼────────────────────┼────────────────────┼────────────────────┼────────────────────┼────────────────────┼────────────────────┤\n");
    
    while(fread(&u, sizeof(u), 1, fp) > 0)
    {
        // 格式化时间
        char reg_time_str[32];
        char expire_time_str[32];
        format_time(u.reg_time, reg_time_str, sizeof(reg_time_str));
        format_time(u.expire_time, expire_time_str, sizeof(expire_time_str));
        
        // 格式化所有字段为20字符宽度
        char formatted_user_id[60];
        char formatted_card_num[60];
        char formatted_name[60];
        char formatted_gender[60];
        char formatted_phone[60];
        char formatted_card_type[60];
        char formatted_ban_state[60];
        char formatted_out_date[60];
        char formatted_balance[60];
        char formatted_reg_time[60];
        char formatted_expire_time[60];
        
        // 转换为字符串并格式化
        char temp[50];
        
        snprintf(temp, sizeof(temp), "%u", u.user_id);
        format_field(formatted_user_id, temp, 20);
        
        format_field(formatted_card_num, u.card_num, 20);
        format_field(formatted_name, u.name, 20);
        format_field(formatted_gender, get_gender_str(u.gender), 20);
        format_field(formatted_phone, u.phone_num, 20);
        format_field(formatted_card_type, get_card_type_str(u.card_type), 20);
        format_field(formatted_ban_state, get_state_str(u.ban_state), 20);
        format_field(formatted_out_date, get_state_str(u.out_date), 20);
        
        snprintf(temp, sizeof(temp), "%.2f", u.balance);
        format_field(formatted_balance, temp, 20);
        
        format_field(formatted_reg_time, reg_time_str, 20);
        format_field(formatted_expire_time, expire_time_str, 20);
        
        printf("│%s│%s│%s│%s│%s│%s│%s│%s│%s│%s│%s│\n",
               formatted_user_id,
               formatted_card_num,
               formatted_name,
               formatted_gender,
               formatted_phone,
               formatted_card_type,
               formatted_ban_state,
               formatted_out_date,
               formatted_balance,
               formatted_reg_time,
               formatted_expire_time);
    }
    
    // 表尾
    printf("└────────────────────┴────────────────────┴────────────────────┴────────────────────┴────────────────────┴────────────────────┴────────────────────┴────────────────────┴────────────────────┴────────────────────┴────────────────────┘\n");
    
    fclose(fp);
}
 

// 先不删了
/*
// 彻底删除用户信息
void delete_user()
{
 char num[50];
        user_info ui;

        printf("请输入要修改的会员卡号/会员ID/手机号:\n");
        scanf("%s", num);
		getchar();

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
                printf("\n不存在此会员, 请重试\n");
        }
        else
		{
			fseek(fp, -sizeof(ui), SEEK_CUR);
			

		}

}
*/

void admin_menu(void)
{
	while(1)
	{
		system("clear");

		hello_admin();
		printf("\n");
		char menu[MENU_SIZE][50] = {"注册用户", "查找用户", "修改用户信息", "用户注销", "展示所有用户", "退出系统"};

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
			user_mod();
		}
		else if(op == 4)
		{
			user_del();
		}
		else if(op == 5)
		{
			show_all_users();
		}
		else if(op == 6)
		{
			break;
		}
		press_any_key();
	}
	
}