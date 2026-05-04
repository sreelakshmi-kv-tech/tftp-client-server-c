#include "tftp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void send_file(int sockfd, struct sockaddr_in client_addr,socklen_t client_len, char *filename)
{
    FILE *fp = fopen(filename, "rb");
    tftp_packet packet, ack;
    uint16_t block = 1;
    int bytes;

    if (fp == NULL)
        return;

    while (1) 
    {
        memset(&packet, 0, sizeof(packet));
        packet.opcode = htons(DATA);
        packet.body.data_packet.block_number = htons(block);

        bytes = fread(packet.body.data_packet.data, 1, 512, fp);

        sendto(sockfd, &packet, bytes + 4, 0,(struct sockaddr *)&client_addr, client_len);

        recvfrom(sockfd, &ack, BUFFER_SIZE, 0,(struct sockaddr *)&client_addr, &client_len);

        if (bytes < 512)
            break;

        block++;
    }

    fclose(fp);
}

void receive_file(int sockfd, struct sockaddr_in client_addr,socklen_t client_len, char *filename)
{
    FILE *fp = fopen(filename, "wb");
    tftp_packet packet, ack;
    int bytes;

    if (fp == NULL)
        return;

    while (1) {
        bytes = recvfrom(sockfd, &packet, BUFFER_SIZE, 0,(struct sockaddr *)&client_addr, &client_len);

        fwrite(packet.body.data_packet.data, 1, bytes - 4, fp);

        ack.opcode = htons(ACK);
        ack.body.ack_packet.block_number = packet.body.data_packet.block_number;

        sendto(sockfd, &ack, 4, 0,(struct sockaddr *)&client_addr, client_len);

        if (bytes < BUFFER_SIZE)
            break;
    }

    fclose(fp);
}
