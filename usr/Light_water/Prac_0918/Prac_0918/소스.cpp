#include <stdio.h>

int main()
{
	for (int i = 1; i <= 9; i++)  //i 1���� 9���� 1�� ����
	{
		for (int j = 1; j <= 9; j++)  //j 1���� 9���� 1�� ����
		{
			printf("%d * %d = %d\n", i, j, i * j);

		}
		printf("-------------------\n");

	}
}
