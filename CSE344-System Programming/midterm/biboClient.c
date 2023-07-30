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

static char clientFifo[CLIENT_FIFO_NAME_LEN];
static struct sigaction sa;
static int serverFd;
static int clientFd;

struct request
{
    pid_t pid;
    char command[BUFFER_SIZE];
    int command_len;
    int connected; // 0 for not connected, 1 for connected
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
    unlink(clientFifo);
    close(serverFd);
    close(clientFd);

    exit(0);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: %s <Connect/tryConnect> ServerPID\n", argv[0]);
        exit(1);
    }

    int server_pid = atoi(argv[2]);
    char *server_command = argv[1];
    struct request req;
    struct response resp;

    umask(0); /* So we get the permissions we want */

    /* Create our FIFO (before sending request, to avoid a race) */
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, getpid());
    if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
        perror("mkfifo");

    fprintf(stdout, ">> fifo path for this client: %s\n", clientFifo);

    /* Construct request message, open server FIFO, and send message */
    req.pid = getpid(); // client pid

    server_command[strcspn(server_command, "\n")] = '\0'; // Remove trailing newline
    snprintf(req.command, sizeof(req.command), "%s", server_command);

    // Open server FIFO to write
    serverFd = open(SERVER_FIFO, O_WRONLY);
    if (serverFd == -1)
        perror("open server FIFO\n");

    // printf("server_command %s", server_command);
    // printf("pid  %d", server_pid);
    // printf("server_command %s", req.command);
    // printf("pid  %d", req.pid);

    if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
        perror("write to server FIFO\n");
    

    // Now open our FIFO to read response from server
    clientFd = open(clientFifo, O_RDONLY);
    if (clientFd == -1)
        perror("open client FIFO\n");

    // Receive and display response
    if (read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
    {
        perror("Can't read response from server\n");
    }
    close(clientFd);

    printf("%s\n", resp.message);
    printf("Status :  %d\n", resp.status);

    // Set up signal handler for SIGINT
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    // If the response status indicates that the queue is full and the command was 'tryConnect', exit
    if (resp.status == QUEUE_FULL && req.connected == 2)
    {
        printf("Server queue is full, exiting...\n");
        exit(0);
    }

    char command[1024];
    // Clear response message
    memset(req.command, 0, sizeof(req.command));

    while (1)
    {
        // Read user command
        printf(">> Enter command: ");

        if (scanf("%[^\n]%*c", req.command) == EOF)
        {
            printf("EOF or read error occurred\n");
            break;
        }

        req.command[strcspn(req.command, "\n")] = '\0'; // Remove trailing newline

        printf("command: %s\n", req.command);

        // Handle 'quit' command
        if (strcmp(req.command, "quit") == 0)
        {
            // Send request to server
            if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
            {
                perror("Can't write to server");
                continue;
            }

            printf("Disconnecting from server...\n");
            break; // Break the loop and exit the program
        }

        // Handle 'killServer' command
        if (strcmp(req.command, "killServer") == 0)
        {
            // Send request to server
            if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
            {
                perror("Can't write to server");
            }

            printf("Sent kill request to server...\n");
            break; // Break the loop and exit the program
        }

        // Check if the server PID is correct
        if (kill(server_pid, 0) == -1)
        {
            printf("Server with PID %d does not exist.\n", server_pid);
            break;
        }

        // Handle 'upload' command
        if (strncmp(req.command, "upload", 6) == 0)
        {
            char *filename = strtok(req.command + 7, " "); // Get filename from command
            if (filename == NULL)
            {
                printf("Missing filename in upload command\n");
            }
            else
            {
                // Open file for reading
                FILE *file = fopen(filename, "r");
                if (file == NULL)
                {
                    printf("Failed to open file '%s'\n", filename);
                }
                else
                {
                    // Open client FIFO for writing
                    int client_write_fd = open(clientFifo, O_WRONLY);
                    if (client_write_fd < 0)
                    {
                        perror("open client write FIFO");
                    }
                    else
                    {
                        // Read from file and write to client FIFO
                        char buffer[1024];
                        size_t count;
                        while ((count = fread(buffer, 1, sizeof(buffer), file)) > 0)
                        {
                            write(client_write_fd, buffer, count);
                        }

                        close(client_write_fd);
                    }

                    fclose(file);
                }
            }

            continue; // Skip the response from server for this iteration
        }

        // Handle 'download' command
        if (strncmp(req.command, "download", 8) == 0)
        {
            char *filename = strtok(req.command + 9, " "); // Get filename from command
            if (filename == NULL)
            {
                printf("Missing filename in download command\n");
            }
            else
            {
                // Open file for writing
                FILE *file = fopen(filename, "w");
                if (file == NULL)
                {
                    printf("Failed to open file '%s'\n", filename);
                }
                else
                {
                    // Read from client FIFO and write to file
                    char buffer[1024];
                    int count;

                    // Now open our FIFO to read response from server
                    clientFd = open(clientFifo, O_RDONLY);
                    if (clientFd == -1)
                        perror("open client FIFO\n");

                    while ((count = read(clientFd, buffer, sizeof(buffer))) > 0)
                    {
                        fwrite(buffer, 1, count, file);
                    }
                    close(clientFd);
                    fclose(file);
                }
            }

            continue; // Skip the response from server for this iteration
        }

        // Send request to server
        if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
        {
            perror("Can't write to server");
            continue;
        }

        // Now open our FIFO to read response from server
        clientFd = open(clientFifo, O_RDONLY);
        if (clientFd == -1)
            perror("open client FIFO\n");

        // Receive and display response
        if (read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
        {
            perror("Can't read response from server");
            continue;
        }
        close(clientFd);

        printf("Server responded with status %d and message: %s \n", resp.status, resp.message);
    }

    printf(">> bye\n");

    // After breaking the loop
    strcpy(req.command, "quit");
    if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
    {
        perror("Can't write to server");
    }

    // Clean up
    unlink(clientFifo);
    close(serverFd);

    return 0;
}
