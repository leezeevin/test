#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//**************************16���� ��ȯ#1. strtol ���*******************************//
/*
int main() {

	char* main;
	char ast[1024];

	int len;
	int hexa;


	printf("16���� ���ڿ� �Է� : ");
	scanf("%s", ast);

	len = strlen(ast);

	main = (char*)calloc(len, sizeof(char));

	strcpy(main, ast);

	hexa = strtol(main, NULL, 16);

	printf("16���� ��ȯ : 0x%02X\n", hexa);
	
}
*/



//**************************16���� ��ȯ#2. ASCII Ư�� �̿�*******************************//

int main() {

	char* main;
	char ast[10];

	int len;
	int hexa = 0;

	printf("16���� ���ڿ� �Է� : ");
	scanf("%s", ast);

	len = strlen(ast);

	main = (char*)calloc(len, sizeof(char));

	strcpy(main, ast);

	while (*main) {
		hexa <<= 4;

		if (*main >= 'A' && *main <= 'Z') {
			hexa |= *main - 'A' + 10;
		}
		else if (*main >= 'a' && *main <= 'z') {
			hexa |= *main - 'a' + 10;
		}
		else if (*main >= '0' && *main <= '9') {
			hexa |= *main - '0';
		}

		++main;
	}

	printf("16���� ��ȯ : 0x%02X\n", hexa);


//**************************2���� ��ȯ*******************************//

	int bin[10];
	int i = 0;
	int k = 0;

	for (i = 0; ; i++) {
		bin[i] = hexa % 2;
		hexa /= 2;

		if (hexa == 0)
			break;
	}

	printf("2���� ��ȯ : ");
	for (k = i; k >= 0; k--) {
		printf("%d", bin[k]);
	}

}


