#include<stdio.h>

int main(void)
{
	for (int i = 0; i < 10; ++i)
	{
		//i�� 4���� ũ�� for ���� �׸� �����ʶ�.
		if (i > 4) break;

		printf("%d ", i);
	}
	return 0;
}