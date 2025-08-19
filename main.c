#include <stdio.h>
#include <stdlib.h>

int main()
{
	
 	int op;	

	printf("欢迎使用本系统!\n");
	printf("\n操作说明\n");
	printf("[1] 注册会员\n");
	
	printf("[2] 修改会员\n");
	
	printf("[1] 删除会员\n");
	
					
	
	while(1)
	{
		printf("\n:>");
		scanf("%d", &op);

		switch(op) 
		{
			case 1:
				{
					printf("感谢使用\n");
					exit(0);	
				}
				break;
			case2:
				{
					printf("感谢使用\n");
				}
				break;
			default:
				{
					break;
				}
		}
	}

	return 0;
}
