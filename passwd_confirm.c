#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define admin_pass "123456"


int confirm_passwd()
{

	char pass[10];
	printf("请输入管理员密码:\n");
	system("stty -echo");
	scanf("%s", pass);
	system("stty echo");
	return !strcmp(pass, admin_pass);
}
