/*
Name:Sreelakshmi kv
This project implements a simple UDP-based file transfer system using the TFTP protocol, demonstrating client-server communication, socket programming, and file transfer mechanisms.
*/
#include "tftp.h"
#include "tftp_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>  

int main()
{
    tftp_client_t client;
    char command[100];

    memset(&client, 0, sizeof(client));

    // Main loop for command-line interface
    while (1) 
    {
        // print the main menu
        printf("\nconnect <ip> <port>\n");
        printf("get <filename>\n");
        printf("put <filename>\n");
        printf("bye\n");

        // read the choice from the user
        scanf(" %[^\n]", command);

        // based on the choice perform operation
        process_command(&client, command);
    }

    return 0;
}

void process_command(tftp_client_t *client, char *command)
{
    char cmd[20], arg[256];
    int port;

    sscanf(command, "%s", cmd);

    if (strcmp(cmd, "connect") == 0) 
    {
        sscanf(command, "%s %s %d", cmd, arg, &port);
        connect_to_server(client, arg, port);
    }
    else if (strcmp(cmd, "get") == 0) 
    {
        sscanf(command, "%s %s", cmd, arg);
        get_file(client, arg);
    }
    else if (strcmp(cmd, "put") == 0) 
    {
        sscanf(command, "%s %s", cmd, arg);
        put_file(client, arg);
    }
    else if (strcmp(cmd, "bye") == 0 || strcmp(cmd, "quit") == 0) 
    {
        disconnect(client);
        exit(0);
    }
    else 
    {
        printf("Invalid command\n");
    }
}

// Initialize socket with given server IP, no packets sent here
void connect_to_server(tftp_client_t *client, char *ip, int port)
{
    // Create UDP socket
    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // read the server address and port no
    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &client->server_addr.sin_addr);

    // validate the information
    client->server_len = sizeof(client->server_addr);
    strncpy(client->server_ip, ip, INET_ADDRSTRLEN);

    // bind
    // client does not explicitly bind
    printf("Connected to %s:%d\n", ip, port);
}

void put_file(tftp_client_t *client, char *filename)
{
    // validate the file is exist or not
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) 
    {
        printf("File not found\n");
        return;
    }
    fclose(fp);

    // Send WRQ request and send file
    send_request(client->sockfd, client->server_addr, filename, WRQ);
    send_file(client->sockfd, client->server_addr,client->server_len, filename);
}

void get_file(tftp_client_t *client, char *filename)
{
    // Send RRQ and receive file
    send_request(client->sockfd, client->server_addr, filename, RRQ);
    receive_file(client->sockfd, client->server_addr,client->server_len, filename);
}

void disconnect(tftp_client_t *client)
{
    // close fd
    close(client->sockfd);
}

void send_request(int sockfd, struct sockaddr_in server_addr,char *filename, int opcode)
{
    tftp_packet packet;
    memset(&packet, 0, sizeof(packet));

    packet.opcode = htons(opcode);
    strcpy(packet.body.request.filename, filename);
    strcpy(packet.body.request.mode, "octet");

    sendto(sockfd, &packet, sizeof(packet), 0,(struct sockaddr *)&server_addr, sizeof(server_addr));
}

void receive_request(int sockfd, struct sockaddr_in server_addr,char *filename, int opcode)
{
    if (opcode == RRQ)
        receive_file(sockfd, server_addr, sizeof(server_addr), filename);
    else if (opcode == WRQ)
        send_file(sockfd, server_addr, sizeof(server_addr), filename);
}
