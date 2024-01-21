#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<string.h> // Dodajemy bibliotekę string.h
#define PORT 1096

void name_socket(int socket_descriptor)
{
    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = PORT,
        .sin_addr = {INADDR_ANY}
    };

    if(bind(socket_descriptor,(struct sockaddr*)&server_address,sizeof(server_address))<0)
        perror("bind");
}

void get_and_print_message(int socket_descriptor, int expected_sequence_number)
{
    char buffer[512];
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    int received_bytes = recvfrom(socket_descriptor,(void *)buffer,sizeof(buffer),0,(struct sockaddr*)&client_address,&address_length);
    if(received_bytes<0)
        perror("recvfrom");
    else {
        buffer[received_bytes]='\0';
        int sequence_number;
        sscanf(buffer, "Komunikat %d", &sequence_number);
        if(sequence_number == expected_sequence_number) {
            puts(buffer);
        } else {
            printf("Oczekiwano komunikatu %d, ale otrzymano komunikat %d\n", expected_sequence_number, sequence_number);
        }
    }
}

int main(void)
{
    int socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_descriptor<0)
        perror("socket");

    name_socket(socket_descriptor);
    for(int i = 0; i < 10; i++) {
        get_and_print_message(socket_descriptor, i);
    }

    if(close(socket_descriptor)<0)
        perror("close");
    return 0;
}
