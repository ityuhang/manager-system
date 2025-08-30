#include <stdio.h>
#include <stdlib.h>
#include "clear_input_buffer.h"


void press_any_key(void)
{
	printf("\n请按任意键返回...\n");
	getchar();
	system("stty -icanon -echo");  // 禁用规范模式
	getchar();
	system("stty icanon echo");    // 恢复规范模式
}
