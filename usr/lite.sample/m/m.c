#include <common.h>
#include <xcommon.h>
#include <__nsocket.h>

#define _IP              "127.0.0.1" // IP �ּ�
#define _PORT            "7810" // ��Ʈ ��ȣ
#define _COMPORT         "/dev/ttyAMA1" // �ø��� ��Ʈ ���
#define _COMBAUDRATE     "115200" // ��� �ӵ�

#if __NSOCKET__ 

void main(int argc, char** argv)
{
    cnsocket socket;
    memset(&socket, 0, sizeof(cnsocket)); // socket �����ϰ� �ʱ�ȭ 

    socket.io.argv[0], _IP;
    socket.io.argv[1], _PORT;
    socket.io.argv[2], "TCP";
    socket.io.argv[3], "CLIENT";
    socket.io.argv[4],  "STREAM"; // socket.io.argv�� ����





    // ���� ����(�����ϸ� 0 �Ǵ� ��� ��ȯ)
    int32_t open_result = nsocket_open(&socket);
    if (open_result < 0)
    {
        printf("Failed to open socket. Error code: %d\n", open_result);

    }
    else
    {
        printf("Socket opened successfully!\n");

    }


    // ������ �Է�
    char user_input[256];
    printf("Enter data to send to the server: ");
    fgets(user_input, sizeof(user_input), stdin);
    int data_length = strlen(user_input);

    int32_t write_result = nsocket_write(socket._imtif.h, 0, (int8_t*)user_input, data_length, 0, &socket);



    // ���� �б�
    int8_t read_buffer[256];
    int32_t read_result = nsocket_read(socket._imtif.h, 0, read_buffer, sizeof(read_buffer), 0, &socket);

    read_buffer[read_result] = '\0';  // �������� NULL �� �߰� -> ���ڿ� ����
    printf("Received data from server: %s\n", read_buffer);

    // ���� �ݱ�
    nsocket_close(&socket);
}

#endif
