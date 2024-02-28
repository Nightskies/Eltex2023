#include "fatal.h"

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_HOST_NAME_LENGTH    128
#define MAX_SERVICE_NAME_LENGTH 128
#define BUF_LENGTH              4096

#define CONNECT "CONNECT"
#define FULL    "FULL"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: name port\n");
        exit(EXIT_FAILURE);
    }

    int err_code;

    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_socktype = SOCK_STREAM;

    struct addrinfo *peer_addr = NULL;
    err_code = getaddrinfo(argv[1], argv[2], &hint, &peer_addr);
    if (err_code < 0) {
        ERROR(gai_strerror(errno));
    }

    char host[MAX_HOST_NAME_LENGTH] = {};
    char serv[MAX_SERVICE_NAME_LENGTH] = {};

    err_code = getnameinfo(peer_addr->ai_addr, peer_addr->ai_addrlen, host, 
        MAX_HOST_NAME_LENGTH, serv, MAX_SERVICE_NAME_LENGTH, NI_NUMERICHOST);
    if (err_code < 0) {
        ERROR(gai_strerror(errno));
    }

    puts("Remote address is:");
    printf("%s:%s\n", host, serv);

    puts("Create socket...");
    int socket_peer = socket(peer_addr->ai_family, peer_addr->ai_socktype, peer_addr->ai_protocol);
    if (socket_peer < 0) {
        ERROR(strerror(errno));
    }

    puts("Connecting...");
    err_code = connect(socket_peer, peer_addr->ai_addr, peer_addr->ai_addrlen);   
    if (err_code < 0) {
        ERROR(strerror(errno));
    }
    freeaddrinfo(peer_addr);

    char sbuf[BUF_LENGTH] = {};
    char rbuf[BUF_LENGTH] = {};

    ssize_t rbytes = recv(socket_peer, rbuf, BUF_LENGTH, 0);
    if (rbytes < 1) {
            if (rbytes == 0) {
                puts("Connection closed by peer\n");
                exit(EXIT_SUCCESS);
            }
            ERROR(strerror(errno));
    }

    if (strcmp(rbuf, FULL) == 0) {
        puts("Failed to connect: server is full");
        exit(EXIT_SUCCESS);
    } else if (strcmp(rbuf, CONNECT) == 0) {
        puts("Successfully connected...");
    } else {
        ERROR("Failed to connect");
    }

    for (;;) {
        puts("Enter lowercase string:");
        fgets(sbuf, BUF_LENGTH, stdin);
        size_t len = strlen(sbuf);
        if (len > 0) {
            sbuf[strlen(sbuf) - 1] = '\0';
        } else {
            continue;
        }

        ssize_t sbytes = send(socket_peer, sbuf, len, 0);
        if (sbytes < 0) {
            ERROR(strerror(errno));
        }

        printf("\nSent %ld bytes\n", sbytes);
        printf("Msg[%s]\n\n", sbuf);

        rbytes = recv(socket_peer, rbuf, sbytes, 0);
        if (rbytes < 1) {
            if (rbytes == 0) {
                puts("Connection closed by peer\n");
                break;
            }
            ERROR(strerror(errno));
        }

        puts("Get uppercase string from server...");
        printf("Received %ld bytes\n", rbytes);
        printf("Msg[%.*s]\n\n", (int)rbytes, rbuf);
    }

    return 0;
}