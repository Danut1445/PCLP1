#include<stdio.h>

int main(void)
{
	char text1[20], text2[20], text3[20];
	if(scanf("%s%s%s", text1, text2, text3) == 3)
		printf("WOW\n");
	else
		printf("NOT WOW\n");
}