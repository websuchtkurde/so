#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 12345

int main(int argc, char *argv[])
{
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;

    struct sockaddr_in serv_adr, clnt_adr;
    FILE* fp;

    fp = fopen("odbiorca2.txt", "w");
    if(fp == NULL)
    {
        printf("Cannot open file to write\n");
        return -1;
    }

    serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock==-1)
       perror("socket");
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(PORT);

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        perror("bind");

    while(1)
    {
        clnt_adr_sz=sizeof(clnt_adr);
        str_len=recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        fwrite(message, sizeof(char), str_len, fp);

        if(str_len < BUF_SIZE)
            break;
    }

    fclose(fp);
    close(serv_sock);
    return 0;
}
