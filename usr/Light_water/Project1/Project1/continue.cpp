#include <stdio.h>
int main(void)
{
	//for continue
	printf("for continue ex.\n");
	for (int i = 0; i < 10; ++i)
	{
		// i�� 2�� �������� �� �������� 0�̸� continue�� Ÿ��
		if (i % 2 == 1) continue;
		
		printf("%d ", i);
	}
	printf("\n");

	return 0;
}