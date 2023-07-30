#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/mman.h>

#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%d"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)
#define BUFFER_SIZE 1024
#define MAX_QUEUE_SIZE 1024
#define MAX_CLIENTS 2048
#define LOG_FILE "log.txt"
#define SHARED_MEM_SIZE (sizeof(sem_t) + MAX_CLIENTS * sizeof(int))

sem_t semaphore;

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

typedef struct
{
    int pid;
    int connected; // 0 is waiting, 1 is connected, -1 is empty.

} Client;

Client clients[MAX_QUEUE_SIZE];

pid_t child_pids[MAX_CLIENTS]; // Array to store child process pids
FILE *log_file = NULL;         // Log file pointer
int num_clients = 0;           // Number of current clients
sem_t *shared_semaphore;       // Shared semaphore pointer
int *shared_clients;           // Shared clients array pointer

int available_slot();
int number_of_connected();
void handle_sigint(int sig);
void setup_signals();
void handle_request(struct request req, int clientFd);

void initialize_clients()
{
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        clients[i].connected = -1;
    }
}
int num_of_connected_clients()
{
    int count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].connected)
        {
            count++;
        }
    }
    return count;
}
int add_clients(int pid, int status)
{

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (!clients[i].connected)
        {
            if (status == 0)
            {
                clients[i].connected = 0;
            }
            else
            {
                clients[i].connected = 1;
            }

            clients[i].pid = pid;
            return 1; // success
        }
    }
    return -1;
}
int remove_client(int pid)
{
    int flag = 0;

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].pid == pid || flag == 1)
        {
            if (i == MAX_CLIENTS - 1)
            {
                clients[i].connected = -1;
                clients[i].pid = -1;
                return 1; // success
            }
            else
            {
                clients[i].connected = clients[i + 1].connected;
                clients[i].pid = clients[i + 1].pid;
            }
        }
    }
    return -1;
}
int main(int argc, char *argv[])
{

    int max_clients;
    int curren_client = 0;
    int serverFd, dummyFd;
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;


    if (sem_init(&semaphore, 1, 1) == -1)
    {
        perror("sem_init");
        exit(1);
    }

    if (argc != 3)
    {
        printf("Usage: %s <dirname> <max. #ofClients>\n", argv[0]);
        exit(1);
    }

    log_file = fopen(LOG_FILE, "w");
    if (!log_file)
    {
        perror("fopen");
        exit(1);
    }

    initialize_clients();

    mkdir(argv[1], 0777);
    if (chdir(argv[1]) == -1)
    {
        perror("chdir");
        exit(1);
    }

    max_clients = atoi(argv[2]);

    setup_signals();

    umask(0); // So we get the permissions we want

    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(1);
    }
    printf("Request FIFO path: %s\n", SERVER_FIFO);
    printf("Server Started at PID %ld...\n", (long)getpid());
    printf("waiting for clients...\n\n");

    // Open server FIFO to write
    serverFd = open(SERVER_FIFO, O_RDONLY);
    if (serverFd == -1)
    {
        perror("open");
        exit(1);
    }

    memset(&req, 0, sizeof(struct request));

    while (1)
    {

        // Read requests and send responses
        if (read(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
        {
            continue; // Either partial read or error
        }

        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, req.pid);
        int clientFd = open(clientFifo, O_WRONLY);
        if (clientFd == -1)
        {
            perror("open");
            continue;
        }
        

        // connect and tryconnect handling
        if (strcmp(req.command, "Connect") == 0)
        {
            // eğer bağlı değilerse
            if (number_of_connected() < MAX_QUEUE_SIZE)
            {
                add_clients(req.pid, 1);
                resp.status = 1;
                strcpy(resp.message, "You are Connected");
                write(clientFd, &resp, sizeof(struct response));
            }
            else
            {
                add_clients(req.pid, 0);
                resp.status = 0;
                strcpy(resp.message, "You are in the queue.");
                write(clientFd, &resp, sizeof(struct response));
            }
            close(clientFd);
            continue;
        }
        else if (strcmp(req.command, "tryConnect") == 0)
        {
            // eğer bağlı değilerse
            if (number_of_connected() >= MAX_QUEUE_SIZE)
            {
                strcpy(resp.message, "Queue is full, exitting..");
                strcpy(req.command, "quit");
            }
        }

        printf("client pid: %d\n", req.pid);
        printf("server pid: %d\n", getpid());
        printf("client connected: %d\n", req.connected);
        printf("client command: %s\n", req.command);

        // Fork a new process for each client
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");

            continue;
        }
        else if (pid == 0)
        {
            // Child process
            close(serverFd);
            handle_request(req, clientFd);
            exit(0); // Exit the child process when done
        }
        else
        {
            // Parent process
            close(clientFd);
            if (num_clients < MAX_CLIENTS)
            {
                child_pids[num_clients] = pid; // Store child pid
            }
        }
        // Check if any child processes have terminated
        pid_t terminated_pid;
        int status;
        while ((terminated_pid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            // Find the corresponding client slot for the terminated child process
            int terminated_slot = -1;
            for (int i = 0; i < num_clients; i++)
            {
                if (child_pids[i] == terminated_pid)
                {
                    terminated_slot = shared_clients[i];
                    break;
                }
            }

            // Update the client status in the shared memory
            if (terminated_slot != -1)
            {
                remove_client(terminated_slot);
                printf("Client %d disconnected\n", clients[terminated_slot].pid);

                // Notify parent process about the disconnection
                sem_wait(shared_semaphore);
                shared_clients[terminated_slot] = -1; // Mark the slot as available
                sem_post(shared_semaphore);
            }
        }
    }

    // cleanup resources
    // Remove server FIFO
    unlink(SERVER_FIFO);

    // Close log file
    if (log_file)
    {
        fclose(log_file);
    }

    // Destroy semaphore
    sem_destroy(shared_semaphore);
    free(clients);

    return 0;
}

int available_slot()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (!clients[i].connected)
        {
            return i;
        }
    }
    return -1; // No available slot
}

int number_of_connected()
{
    int count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].connected == 1)
        {
            count++;
        }
    }
    return count;
}

void handle_sigint(int sig)
{
    fprintf(log_file, "Kill signal received, terminating...\n");
    fflush(log_file);

    // Send kill signal to all child processes
    for (int i = 0; i < num_clients; i++)
    {
        kill(child_pids[i], SIGKILL);
    }

    // Remove server FIFO
    unlink(SERVER_FIFO);

    if (log_file)
    {
        fclose(log_file);
    }
    sem_destroy(&semaphore);
    exit(0);
}

void setup_signals()
{
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}

void handle_request(struct request req, int clientFd)
{
    char clientFifo[CLIENT_FIFO_NAME_LEN];

    struct response resp;

    // Split the command into separate arguments
    char *command = strtok(req.command, " "); // Get the first word as the command
    char *args = strtok(NULL, "");            // Get the rest of the command as arguments
    printf("command %s", command);
    printf("args %s", args);

    if (strcmp(req.command, "killServer") == 0)
    {
        // Perform the 'killServer' operation
        fprintf(log_file, "Kill server command received from client %d\n", req.pid);
        fflush(log_file);
        raise(SIGINT);
    }

    if (strcmp(command, "list") == 0)
    {
        sem_wait(&semaphore); // Lock
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d)
        {
            char filesList[1024] = ""; // Initialize to empty
            while ((dir = readdir(d)) != NULL)
            {
                strcat(filesList, dir->d_name);
                strcat(filesList, "\n");
            }
            closedir(d);

            // Send response
            resp.status = 0; // Success
            strncpy(resp.message, filesList, sizeof(resp.message) - 1);
            resp.message[sizeof(resp.message) - 1] = '\0'; // Ensure null-terminated

            if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
            {
                perror("write to client FIFO");
            }
        }
        sem_post(&semaphore); // Unlock
    }

    else if (strcmp(command, "help") == 0)
    {
     
        if (strcmp(args, "list") == 0)
        {
            snprintf(resp.message, sizeof(resp.message), "list\n"
                                                         "Sends a request to display the list of files in Servers directory (also displays the list received from the Server)");
        }
        else if (strcmp(args, "readF") == 0)
        {
            snprintf(resp.message, sizeof(resp.message), "readF <file> <line #>\n"
                                                         "Requests to display the #th line of the <file>, if no line number is given the whole contents of the file is requested (and displayed on the client side)");
        }
        else if (strncmp(args, "writeT", 6) == 0)
        {
            snprintf(resp.message, sizeof(resp.message), "writeT <file> <line #> <string>\n"
                                                         "Requests to write the content of 'string' to the #th line of the <file>, if the line # is not given, writes to the end of the file. If the file does not exist in Servers directory, creates and edits the file at the same time");
        }
        else if (strcmp(args, "upload") == 0)
        {
            snprintf(resp.message, sizeof(resp.message), "upload <file>\n"
                                                         "Uploads the file from the current working directory of client to the Servers directory (beware of the cases no file in clients current working directory and file with the same name on Servers side)");
        }
        else if (strcmp(args, "download") == 0)
        {
            snprintf(resp.message, sizeof(resp.message), "download <file>\n"
                                                         "Requests to receive <file> from Servers directory to client side");
        }
        else if (strcmp(args, "quit") == 0)
        {
            snprintf(resp.message, sizeof(resp.message), "quit\n"
                                                         "Send write request to Server side log file and quit");
        }
        else
        {
            snprintf(resp.message, sizeof(resp.message), "Available comments are :\n"
                                                         "help, list, readF, writeT, upload, download, quit, killServer");
        }

        // Send response back to client
        if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
            perror("write to client FIFO");
    }
    else if (strncmp(command, "readF", 5) == 0)
    {
        sem_wait(&semaphore); // Lock

        char *filename = strtok(req.command + 6, " "); // Get filename from command
        char *line_str = strtok(NULL, " ");            // Get line number from command

        if (filename == NULL)
        {
            snprintf(resp.message, sizeof(resp.message), "Missing filename argument in readF command");
        }
        else
        {
            FILE *file = fopen(filename, "r");
            if (file == NULL)
            {
                snprintf(resp.message, sizeof(resp.message), "Failed to open file '%s'", filename);
            }
            else
            {
                if (line_str == NULL)
                {
                    // If no line number is given, read the whole file
                    char line[256];
                    while (fgets(line, sizeof(line), file))
                    {
                        strcat(resp.message, line); // Append line to message
                    }
                }
                else
                {
                    // If a line number is given, read only that line
                    int line_num = atoi(line_str);
                    char line[256];
                    for (int i = 1; i <= line_num; i++)
                    {
                        if (fgets(line, sizeof(line), file) == NULL)
                        {
                            break;
                        }
                        if (i == line_num)
                        {
                            strcpy(resp.message, line); // Copy line to message
                        }
                    }
                }
                fclose(file);
            }
        }

        sem_post(&semaphore); // Unlock

        // Send response back to client
        if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
            perror("write to client FIFO");

        close(clientFd); // Close the client FD when done

        // Log the request
        fprintf(log_file, "Request from client %d: %s\n", req.pid, req.command);
        fflush(log_file);
    }

    else if (strncmp(command, "writeT", 6) == 0)
    {
        sem_wait(&semaphore); // Lock

        char *filename = strtok(req.command + 7, " "); // Get filename from command
        char *line_str = strtok(NULL, " ");            // Get line number from command
        char *content = strtok(NULL, "");              // Get content from command

        if (filename == NULL || content == NULL)
        {
            snprintf(resp.message, sizeof(resp.message), "Missing arguments in writeT command");
        }
        else
        {
            FILE *file = fopen(filename, "a+"); // Open for reading and appending
            if (file == NULL)
            {
                snprintf(resp.message, sizeof(resp.message), "Failed to open file '%s'", filename);
            }
            else
            {
                if (line_str == NULL)
                {
                    // If no line number is given, append to the end of the file
                    fputs(content, file);
                    fputs("\n", file); // Add newline character
                }
                else
                {
                    // If a line number is given, write to that line (overwriting existing content)
                    int line_num = atoi(line_str);
                    char line[256];
                    char temp_filename[] = "temp.txt";
                    FILE *temp_file = fopen(temp_filename, "w");

                    for (int i = 1; fgets(line, sizeof(line), file); i++)
                    {
                        if (i == line_num)
                        {
                            fputs(content, temp_file);
                            fputs("\n", temp_file); // Add newline character
                        }
                        else
                        {
                            fputs(line, temp_file);
                        }
                    }

                    fclose(file);
                    fclose(temp_file);

                    remove(filename);                // Remove the original file
                    rename(temp_filename, filename); // Rename the temporary file
                }
            }
        }

        sem_post(&semaphore); // Unlock

        // Send response back to client
        if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
            perror("write to client FIFO");

        close(clientFd); // Close the client FD when done

        // Log the request
        fprintf(log_file, "Request from client %d: %s\n", req.pid, req.command);
        fflush(log_file);
    }

    else if (strncmp(command, "upload", 6) == 0)
    {
        sem_wait(&semaphore); // Lock

        char *filename = strtok(req.command + 7, " "); // Get filename from command

        if (filename == NULL)
        {
            snprintf(resp.message, sizeof(resp.message), "Missing filename in upload command");
        }
        else
        {
            char client_fifo[256];
            snprintf(client_fifo, sizeof(client_fifo), CLIENT_FIFO_TEMPLATE, req.pid);

            // Open client FIFO for reading
            int client_read_fd = open(client_fifo, O_RDONLY);
            if (client_read_fd < 0)
            {
                perror("open client read FIFO");
            }
            else
            {
                // Open or create file for writing
                FILE *file = fopen(filename, "w");
                if (file == NULL)
                {
                    snprintf(resp.message, sizeof(resp.message), "Failed to open file '%s'", filename);
                }
                else
                {
                    // Read from client FIFO and write to file
                    char buffer[1024];
                    ssize_t count;
                    while ((count = read(client_read_fd, buffer, sizeof(buffer))) > 0)
                    {
                        fwrite(buffer, 1, count, file);
                    }

                    fclose(file);
                }
                close(client_read_fd);
            }
        }

        sem_post(&semaphore); // Unlock

        close(clientFd); // Close the client FD when done

        // Log the request
        fprintf(log_file, "Request from client %d: %s\n", req.pid, req.command);
        fflush(log_file);
    }

    else if (strncmp(command, "download", 8) == 0)
    {
        char *filename = strtok(req.command + 9, " "); // Get filename from command

        if (filename == NULL)
        {
            printf("Missing filename in download command\n");
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
    }

    else if (strcmp(command, "quit") == 0)
    {
        // Write to log file
        FILE *logfile = fopen("server_log.txt", "a");
        if (logfile == NULL)
        {
            printf("Failed to open log file\n");
        }
        else
        {
            fprintf(logfile, "Client %d disconnected\n", req.pid);
            fclose(logfile);
        }

        // Close client FIFO
        close(clientFd);
        clientFd = -1;

        printf("Client %d disconnected\n", req.pid);
    }
    else if (strcmp(command, "killServer") == 0)
    {
        // Write to log file
        FILE *logfile = fopen("server_log.txt", "a");
        if (logfile == NULL)
        {
            printf("Failed to open log file\n");
        }
        else
        {
            fprintf(logfile, "Received kill request. Shutting down server...\n");
            fclose(logfile);
        }

        // Close all connections and free resources here...

        // Terminate the server
        printf("Received kill request. Shutting down server...\n");
        exit(0);
    }
    else
    {
        snprintf(resp.message, strlen(resp.message), "Command Not Found!");
    }

    // Send response back to client
    if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
        perror("write to client FIFO");

    close(clientFd); // Close the client FD when done

    // Log the request
    fprintf(log_file, "Request from client %d: %s\n", req.pid, req.command);
    fflush(log_file);

    return;
}
