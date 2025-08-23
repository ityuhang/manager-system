#ifndef USER_H
#define USER_H
#include <stdio.h>
#include <time.h>

typedef enum
{
	female = 1,
	male = 2,
	unknow = 3
} sex;


typedef enum
{
	daily = 1,		// 天卡
	weekly = 2,		// 周卡
    monthly = 3,	// 月卡
	yearly = 4,		// 年卡
} package;


typedef struct 
{
	unsigned int user_id;	// 用户ID
	char card_num[11];	// 卡号

	
	char name[50];		// 姓名
	sex gender;		// 性别
	char phone_num[12]; 	// 手机号
	package card_type;	// 卡类型

	int ban_state; 		// 删除状态, 0 未禁用, 1 已禁用
	int out_date;  		// 是否过期, 0 未过期, 1 已过期
	double balance;		// 账户余额
	time_t reg_time;	// 注册时间
	time_t expire_time;	// 到期时间
} user_info;

void user_reg(void);
void user_show(void);

#endif
