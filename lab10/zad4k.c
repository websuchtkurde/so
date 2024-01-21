#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 12345
#define IP_ADDR "127.0.0.1"

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_adr;
    FILE* fp;

    fp = fopen("send.txt", "r");
    if(fp == NULL)
    {
        printf("Cannot open file to read\n");
        return -1;
    }

    sock=socket(PF_INET, SOCK_DGRAM, 0);   
    if(sock==-1)
        perror("socket");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(IP_ADDR);
    serv_adr.sin_port=htons(PORT);

    while(1)
    {
        int read_cnt = fread((void*)message, 1, BUF_SIZE, fp);
        if(read_cnt > 0)
        {
            sendto(sock, message, read_cnt, 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        }

        if(read_cnt < BUF_SIZE)
        {
            break;
        }
    }

    fclose(fp);
    close(sock);
    return 0;
}
