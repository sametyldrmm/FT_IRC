#include <stdio.h>

void asd(int *a)
{
	printf("%d\n",*a);
	int c;
	c  =50;
	a = &c;
	printf("%d\n",*a);
}

int main()
{
		int *a;
		int b;

		b  =20;
		a = &b;
		asd(a);

		printf("%d\n",*a);
		printf("%d",b);
}