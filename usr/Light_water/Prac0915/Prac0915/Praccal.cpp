#include <stdio.h>
#pragma warning(disable:4996);

int calc(int i, int j);

int main()
{
	int num1, num2;

	printf("���� �Է��ϼ��� : ");
	scanf("%d", &num1);

	printf("���� ��: ");
	scanf("%d", &num2);

	printf("�� ���� ���� %d �Դϴ�. ", calc(num1, num2));
	
	return 0;
}

int calc(int i, int j)
{
	return i * j;
}
