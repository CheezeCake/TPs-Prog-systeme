#include <stdio.h>

int main(void)
{
	char o;
	int op1, op2;

	printf("> ");
	scanf("%c%d%d", &o, &op1, &op2);
	printf("< ");

	switch(o)
	{
		case '+':
			printf("%d", op1+op2);
			break;
		case '-':
			printf("%d", op1-op2);
			break;
		case '*':
			printf("%d", op1*op2);
			break;
		case '/':
			printf("%d", op1/op2);
			break;
	}

	return 0;
}
