#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void Str_to_Hex(const char* str)
{
    int value = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isxdigit(str[i]))  // 16����
        {
            if (isupper(str[i]))  // �빮��
            {
                value = value * 16 + (str[i] - 'A' + 10);
            }
            else if (islower(str[i])) //�ҹ���
            {
                value = value * 16 + (str[i] - 'a' + 10);
            }
            else if (isdigit(str[i])) //����
            {
                value = value * 16 + (str[i] - '0');
            }
        }
    }

    printf("%s: 0x%02X\n", str, value);
}

int main()
{
    const char* s[] = { "A","B","C","D","E","F", "AA", "ABCDEF","01ABCDEF" }; //���ڿ� ������ �迭
    int size = sizeof(s) / sizeof(s[0]);

    for (int i = 0; i < size; i++) // ��ȯ
    {
        Str_to_Hex(s[i]);
    }

    printf("\n");

    return 0;
}
