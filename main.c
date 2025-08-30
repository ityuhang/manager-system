#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "user_info.h"
#include "reg.h"


int main()
{
	int n;
	system("clear");
	printf("===============欢迎使用本系统===============\n\n");
	
	time_t t1 = time(NULL);
	updata_expire_data();
	while(1)	
	{
		user_show();
		// 每24小时（86400秒）运行一次更新用户过期状态
		if(time(NULL) - t1 == 86400)
		{
			updata_expire_data();
		}
	}
	return 0;
}