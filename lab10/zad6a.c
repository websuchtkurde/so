#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#define SERVER_PORT 1096
#define SERVER_IP_ADDRESS "127.0.0.1"

void send_message(int socket_descriptor, int sequence_number)
{
    struct in_addr network_address;

    if(!inet_aton(SERVER_IP_ADDRESS,&network_address))
        perror("inet_aton");

    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = SERVER_PORT,
        .sin_addr = network_address
    };

    char message[512];
    sprintf(message, "Komunikat %d przesłany przez sieć.", sequence_number);
    if(sendto(socket_descriptor,message,strlen(message),0,
        (struct sockaddr *)&server_address,sizeof(server_address))<0)
        perror("sendto");
}

int main(void)
{
    int socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_descriptor<0)
        perror("socket");

    for(int i = 0; i < 10; i++) {
        send_message(socket_descriptor, i);
        sleep(1); // Delay to ensure messages can be processed in order
    }

    if(close(socket_descriptor)<0)
        perror("close");
    return 0;
}
