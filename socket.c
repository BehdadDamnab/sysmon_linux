#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "logger.h"
#include "file_writer.h"

#define PORT 9000

void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    free(arg);

    // printf("[%d] Client thread started!\n", client_fd);

    SystemStats stats = logger_starter();
    file_writer(stats);

    char response[64];
    snprintf(response, sizeof(response), "[%d] Logged, Open /tmp/sysmon.log\n", client_fd);
    send(client_fd, response, strlen(response), 0);

    close(client_fd);

    // printf("[%d] Client thread done!\n", client_fd);
    return NULL;
}

int create_socket(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);
    // printf("Server listening on port %d...\n", PORT);

    while (1)
    {

        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, NULL, NULL);
        // printf("[%d] New client connected!\n", *client_fd);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}