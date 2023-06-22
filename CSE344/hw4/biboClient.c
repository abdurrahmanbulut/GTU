#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%d"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)
#define QUEUE_FULL 1
#define BUFFER_SIZE 1024

static char clientFifoName[CLIENT_FIFO_NAME_LEN];
static struct sigaction sa;
static int serverFd;
static int clientFd;

struct request
{
    pid_t pid;
    char command[BUFFER_SIZE];
    int command_len;
    int connected; // 0 for not connected, 1 for connected
    int disconnect; // 0 for still connected, 1 for disconnect
};

struct response
{
    int status;
    char message[BUFFER_SIZE];
};

void handle_sigint(int sig)
{
    printf("Caught SIGINT, exiting...\n");

    // Clean up
    unlink(clientFifoName);
    close(serverFd);
    close(clientFd);

    exit(0);
}

int main(int argc, char *argv[])
{
    struct request req;
    struct response resp;

    char client_fifo_name[BUFFER_SIZE];
    int server_fifo_fd;
    int client_fifo_fd;

    if (argc != 3)
    {
        printf("Usage: %s <Connect/tryConnect> ServerPID\n", argv[0]);
        exit(1);
    }

    req.pid = getpid();
    req.connected = 1;
    req.disconnect = 0;

    int server_pid = atoi(argv[2]);

    // Set up signal handler for SIGINT
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    umask(0); /* So we get the permissions we want */

    /* Create our FIFO (before sending request, to avoid a race) */
    sprintf(client_fifo_name, "/tmp/seqnum_cl.%d", req.pid);
    mkfifo(client_fifo_name, 0777);

    // Open server FIFO to write
    server_fifo_fd = open(SERVER_FIFO, O_WRONLY);
    if (server_fifo_fd == -1)
    {
        fprintf(stderr, "Server fifo failure\n");
        return 1;
    }

    
    strcpy(req.command, argv[1]);
    write(server_fifo_fd, &req, sizeof(req));

    client_fifo_fd = open(client_fifo_name, O_RDONLY);
    if (client_fifo_fd != -1)
    {
        if (read(client_fifo_fd, &resp, sizeof(resp)) > 0)
        {
            printf("Received: %s\n", resp.message);
        }
        close(client_fifo_fd);
    }

    while (1)
    {
        printf(">> Enter command: ");
        fgets(req.command, BUFFER_SIZE, stdin);
        req.command_len = strlen(req.command);

        // If the last character is '\n', replace it with '\0'
        if (req.command[req.command_len - 1] == '\n')
        {
            req.command[req.command_len - 1] = '\0';
            req.command_len--;
        }

        write(server_fifo_fd, &req, sizeof(req));
        if (strcmp(req.command, "quit\n") == 0)
        {
            break;
        }

        client_fifo_fd = open(client_fifo_name, O_RDONLY);
        if (client_fifo_fd != -1)
        {
            if (read(client_fifo_fd, &resp, sizeof(resp)) > 0)
            {
                printf("Received: %s\n", resp.message);
            }
            close(client_fifo_fd);
        }
    }

    close(server_fifo_fd);
    unlink(client_fifo_name);

    return 0;
}
