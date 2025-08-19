#include <stdio.h>
#include <string.h>
#define admin_pass "123456"


int confirm_passwd()
{
	char pass[10];
	printf("请输入管理员密码:\n");

	scanf("%s", pass);
	return !strcmp(pass, admin_pass);
}
