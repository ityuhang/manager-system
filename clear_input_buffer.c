#include <stdio.h>

void cb(void)
{
	int c;
	int i = 0;
	while(i < 1000)
	{
		c = getchar();
		i++;
	}
	printf("缓存已清空\n");
}
