#include <stdio.h>

int main()
{
	printf("1~100���� Ȧ������ ���� ������ ����. ");

	int sum = 0;
	for (int i = 1; i <= 100; ++i)
	{
		if (i % 2 == 0) continue;

		sum += i;
	}
	printf("Ȧ������ �� : %d\n", sum);

	printf("This displays %d, too\n", 99);
	printf("and also display this : %.3f", 3.14);
}