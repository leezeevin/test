#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	

	/*
	typedef struct {

		char* buff;
		int component;
		int* end;
		int* tol;


	}project;
	*/


int main() {


	/******************************************************************
	// #1. Basic

	int aa[1024];
	int* p;
	int i;
	p = aa;

	for (i = 0; i < sizeof(aa) / sizeof(int); i++) {
		p[i] = i;

	}

	for (i = 0; i < sizeof(aa)/sizeof(int); i++) {
		printf("p[%d] = %d\n", i,*(p + i));
	}
******************************************************************/




	/*
	// #2. ���� ���

	int aa[1024];
	int* p;
	int i;
	int size;
	p = aa;

	size = sizeof(aa) / sizeof(int);

	for (i = size - 1; i >= 0; i--) {
		*(p + size - i - 1) = i;
	}

	int k;

	for (k = 0; k < 1024; k++) {
		printf("p[%d] = %d\n", k, *(p + k));
	}
	*/

	/*
	// #3. malloc �Լ� �̿�

	int* p;
	int num;

	printf("�迭 ���� ���� : ");
	scanf("%d", &num);

	p = (int*)malloc(sizeof(int) * num);


	int i = 0;
	while (1) {
		*(p + i) = i;
		i++;

		if (i == num)
			break;

	}

	for (i = 0; i < num; i++) {
		printf("p[%d] = %d\n", i, *(p+i));
	}

	free(p);
	*/



	/*
	// #4. �迭 ���� �Է� -> ��� -> ����
	int* p;
	int num;
	int temp;

	printf("�迭 ���� ���� : ");
	scanf("%d", &num);

	p = (int*)malloc(sizeof(int) * num);


	puts("\n-------�迭 ���� �Է�-------\n");

	int i = 0;
	while (1) {

		printf("p[%d] = ", i);
		scanf("%d", p + i);

		i++;

		if (i == num)
			break;

	}

	puts("\n-------�迭 ���� Ȯ��-------\n");

	for (i = 0; i < num; i++) {
		printf("p[%d] = %d\n", i, *(p + i));
	}

	puts("\n-------�迭 ���� ����-------\n");

	for (i = 0; i < num-1; i++) {
		for (int k = i + 1; k < num; k++) {
			if (*(p + i) > *(p + k)) {
				temp = *(p + i);
				*(p + i) = *(p + k);
				*(p + k) = temp;
			}
		}
	}

	for (i = 0; i < num; i++) {
		printf("p[%d] = %d\n", i, *(p + i));

	}

	free(p);
	*/

	/*
	// ����ü ����
	project* pj = malloc(sizeof(project));

	printf("���� �迭 ���� : ");
	scanf("%d", &pj->component);

	pj->buff = malloc(sizeof(int) * (pj->component));
	pj->tol = malloc(sizeof(int) * (pj->component));

	int i;

	for (i = 0; i < pj->component; i++) {
		sprintf(pj->buff, "%d", i);
	}


	for (i = 0; i < pj->component; i++) {
		if (i == 0)
			pj->tol[i] = strtol(pj->buff, &pj->end, 10);
		else if (i == pj->component)
			pj->tol[i] = strtol(pj->end, NULL, 10);
		else if (i > 0 && i < pj->component)
			pj->tol[i] = strtol(pj->end, &pj->end, 10);
	}

	for (i = 0; i < pj->component; i++) {
		printf("p[%d] = %d\n", i, *pj->tol + i);
	}


	free(pj);
	free(pj->buff);
	free(pj->component);
	*/


	char* string;
	int size, length;
	int i;
	int bcnt, scnt, icnt, pcnt;
	int buffer;


	buffer = 'a' - 'A';
	bcnt = scnt = icnt = pcnt = 0;

	printf("���� ���� : ");
	scanf("%d", &length);
	string = (char*)calloc(length, sizeof(char));

	printf("\n���ڿ� �Է� : ");
	scanf("%s", string);

	size = strlen(string);

	printf("\n�Ųٷ� ��� : ");
	for (i = size - 1; i >= 0; i--) {
		printf("%c", *(string + i));
	}

	for (i = 0; i < size; i++) {
		if (*(string + i) >= 'A' && *(string + i) <= 'Z')
		{
			bcnt++;
		}
		else if (*(string + i) >= 'a' && *(string + i) <= 'z')
		{
			scnt++;
		}
		else if (*(string + i) >= '0' && *(string + i) <= '9')
		{
			icnt++;
		}
		else if ((*(string + i) >= '!' && *(string + i) <= '/') || (*(string + i) >= ':' && *(string + i) <= '@'))
		{
			pcnt++;
		}
	}

	printf("\n\n�빮�� : %d, �ҹ��� : %d, ���� : %d, Ư������ : %d\n", bcnt, scnt, icnt, pcnt);


	printf("\n��ҹ��� ��ȯ : ");

	for (i = 0; i < size; i++) {
		if (*(string + i) >= 'A' && *(string + i) <= 'Z')
		{
			*(string + i) += buffer;
		}
		else if (*(string + i) >= 'a' && *(string + i) <= 'z')
		{
			*(string + i) -= buffer;
		}
		else if (*(string + i) >= '0' && *(string + i) <= '9')
		{
			string[i] = *(string + i);
		}
		else if ((*(string + i) >= '!' && *(string + i) <= '/') || (*(string + i) >= ':' && *(string + i) <= '@'))
		{
			string[i] = *(string + i);
		}

		printf("%c", string[i]);
	}

	for (i = 0; i < size; i++) {
		if (*(string + i) >= 'A' && *(string + i) <= 'Z')
		{
			*(string + i) += buffer;
		}
		else if (*(string + i) >= 'a' && *(string + i) <= 'z')
		{
			*(string + i) -= buffer;
		}
		else if (*(string + i) >= '0' && *(string + i) <= '9')
		{
			string[i] = *(string + i);
		}
		else if ((*(string + i) >= '!' && *(string + i) <= '/') || (*(string + i) >= ':' && *(string + i) <= '@'))
		{
			string[i] = *(string + i);
		}

	}

	char* compare;
	int to;

	compare = (char*)calloc(length, sizeof(char));

	printf("\n\n���� ������ �Է��ϼ��� : ");
	scanf("%s", compare);

	to = strcmp(string, compare);

	switch (to) {
	case 0: printf("\n�� ������ �����ϴ�.\n"); break;
	case 1: printf("\n�� ������ �ٸ��ϴ�.\n"); break;
	case -1: printf("\n�� ������ �ٸ��ϴ�.\n"); break;
	}
	

	int temp;
	printf("\n���� �����ϱ� : ");
	for (i = 0; i < size - 1; i++) {
		for (int k = i + 1; k < size; k++) {
			if (*(string + i) > *(string + k)) {
				temp = *(string + i);
				*(string + i) = *(string + k);
				*(string + k) = temp;
			}
		}
	}
	for (i = 0; i < size; i++) {
		printf("%c", *(string + i));
	}
	
	

	char ch;

	printf("\n\n�ܾ��� ������? [�ܾ� �Է�] : ");
	scanf(" %c", &ch);

	int component = 0;

	while (string != '\0') {
		string = strchr(string + 1, ch);
		component++;
	}

	printf("\n%c�� %d�� �Դϴ�.\n", ch, component - 1);


	
	

}

	

	

