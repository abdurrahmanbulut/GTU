#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#include <pthread.h>

#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%d"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)
#define BUFFER_SIZE 1024
#define MAX_QUEUE_SIZE 1024
#define MAX_CLIENTS 1000
#define LOG_FILE "log.txt"

#define MAX_THREAD_POOL_SIZE 100

struct request
{
    pid_t pid;
    char command[BUFFER_SIZE];
    int command_len;
    int connected;  // 0 for not connected, 1 for connected
    int disconnect; // 0 for still connected, 1 for disconnect
};

struct response
{
    int status;
    char message[BUFFER_SIZE];
};

struct request_queue
{
    struct request req;
    struct request_queue *next;
};

struct request_queue *head = NULL;
struct request_queue *tail = NULL;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

int client_count = 0;

void handle_request(struct request);
void enqueue(struct request req);
struct request dequeue();
void *thread_function(void *arg);

typedef struct
{
    int pid;
    int connected; // 0 is waiting, 1 is connected, -1 is empty.

} Client;

Client clients[MAX_QUEUE_SIZE];

pid_t child_pids[MAX_CLIENTS]; // Array to store child process pids
FILE *log_file = NULL;         // Log file pointer

struct request requests[MAX_CLIENTS];
int request_count = 0;
int num_clients = 0; // Number of current clients
int *shared_clients; // Shared clients array pointer

int available_slot();
int number_of_connected();
void handle_sigint(int sig);
void setup_signals();
void initialize_clients();
int num_of_connected_clients();
int add_clients(int pid, int status);
int remove_client(int pid);


// ************************
// ******Main Function*****
// ************************
int main(int argc, char *argv[])
{

    struct request req;
    struct response resp;
    int max_clients;
    int curren_client = 0;
    int pool_size;

    int server_fifo_fd;
    int client_fifo_fd;

    char client_fifo_name[CLIENT_FIFO_NAME_LEN];

    max_clients = atoi(argv[2]);
    pool_size = atoi(argv[3]);
    pthread_t threads[pool_size];

    if (argc != 4)
    {
        printf("Usage: %s <dirname> <max. #ofClients> <poolSize>\n", argv[0]);
        exit(1);
    }


    initialize_clients();


    mkdir(argv[1], 0777);
    if (chdir(argv[1]) == -1)
    {
        perror("chdir");
        exit(1);
    }
    
    // LOG_FILE = "log.txt"
    log_file = fopen(LOG_FILE, "w");
    if (!log_file)
    {
        perror("fopen");
        exit(1);
    }

    setup_signals();

    umask(0); // So we get the permissions we want

    mkfifo(SERVER_FIFO, 0777);

    printf("Request FIFO path: %s\n", SERVER_FIFO);
    printf("Server Started at PID %ld...\n", (long)getpid());
    printf("waiting for clients...\n\n");

    // Open server FIFO to write
    server_fifo_fd = open(SERVER_FIFO, O_RDONLY);
    if (server_fifo_fd == -1)
    {
        fprintf(stderr, "Server fifo failure\n");
        return 1;
    }

    // Create threads
    for (int i = 0; i < pool_size; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    while (1)
    {

        // Read requests and send responses
        if (read(server_fifo_fd, &req, sizeof(req)) > 0)
        {
            pthread_mutex_lock(&queue_mutex);
            if (req.connected == 1 && client_count < MAX_CLIENTS)
            {
                enqueue(req);
            }
            pthread_mutex_unlock(&queue_mutex);
        }
    }

    // Join threads (in practice this code won't be reached because the server runs forever)
    for (int i = 0; i < pool_size; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // cleanup resources
    close(server_fifo_fd);
    unlink(SERVER_FIFO);

    // Close log file
    if (log_file)
    {
        fclose(log_file);
    }

    // free(clients);
    return 0;
}

void *thread_function(void *arg)
{
    pthread_t self_id = pthread_self(); // get current thread id

    while (1)
    {
        pthread_mutex_lock(&queue_mutex);
        struct request req = dequeue();
        pthread_mutex_unlock(&queue_mutex);

        if (req.pid != 0)
        {
            if (req.disconnect == 1)
            {
                pthread_mutex_lock(&client_mutex);
                --client_count;
                pthread_mutex_unlock(&client_mutex);
                continue;
            }

            pthread_mutex_lock(&client_mutex);
            ++client_count;
            pthread_mutex_unlock(&client_mutex);

            printf("Thread with id %lu is handling request from client %d\n", self_id, req.pid);
            char client_fifo_name[BUFFER_SIZE];
            int client_fifo_fd;
            struct response res;

            handle_request(req);

            printf("Thread id %lu has finished handling request from client %d\n\n", self_id, req.pid);
        }
    }
    return NULL;
}

void enqueue(struct request req)
{
    struct request_queue *newNode = malloc(sizeof(struct request_queue));
    newNode->req = req;
    newNode->next = NULL;
    if (tail == NULL)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
}

struct request dequeue()
{
    if (head == NULL)
    {
        // in case the queue is empty.
        return (struct request){0};
    }
    else
    {
        struct request_queue *temp = head;
        struct request req = temp->req;
        head = head->next;
        if (head == NULL)
        {
            tail = NULL;
        }
        free(temp);
        return req;
    }
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

void handle_request(struct request req)
{
    char client_fifo_name[BUFFER_SIZE];

    struct response resp;
    int client_fifo_fd;

    // Split the command into separate arguments
    char *command = strtok(req.command, " "); // Get the first word as the command
    char *args = strtok(NULL, "");            // Get the rest of the command as arguments
    printf("command is %s\n", command);
    printf("extra argumet is %s\n", args);

    sprintf(client_fifo_name, "/tmp/seqnum_cl.%d", req.pid);
    client_fifo_fd = open(client_fifo_name, O_WRONLY);

    if (client_fifo_fd != -1)
    {

        if (strcmp(req.command, "Connect") == 0)
        {
            snprintf(resp.message, sizeof(resp.message), "Connect dendi.");
            resp.status = 1;
        }

        else if (strcmp(command, "killServer") == 0)
        {
            // Perform the 'killServer' operation
            fprintf(log_file, "Kill server command received from client %d\n", req.pid);
            fflush(log_file);
            raise(SIGINT);
        }

        else if (strcmp(command, "list") == 0)
        {
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
                printf("%s", filesList);

                strncpy(resp.message, filesList, sizeof(resp.message) - 1);
                resp.message[sizeof(resp.message) - 1] = '\0'; // Ensure null-terminated
            }
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
        }
        else if (strncmp(command, "readF", 5) == 0)
        {

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

            // Log the request
            fprintf(log_file, "Request from client %d: %s\n", req.pid, req.command);
            fflush(log_file);
        }

        else if (strncmp(command, "writeT", 6) == 0)
        {

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

            // Log the request
            fprintf(log_file, "Request from client %d: %s\n", req.pid, req.command);
            fflush(log_file);
        }

        else if (strncmp(command, "upload", 6) == 0)
        {

            char *filename = strtok(req.command + 7, " "); // Get filename from command

            if (filename == NULL)
            {
                snprintf(resp.message, sizeof(resp.message), "Missing filename in upload command");
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
                    while ((count = read(client_fifo_fd, buffer, sizeof(buffer))) > 0)
                    {
                        fwrite(buffer, 1, count, file);
                    }

                    fclose(file);
                }
            }

            // Log the request
            fprintf(log_file, "Request from client %d: %s\n", req.pid, command);
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

                    // Read from file and write to client FIFO
                    char buffer[1024];
                    size_t count;
                    while ((count = fread(buffer, 1, sizeof(buffer), file)) > 0)
                    {
                        write(client_fifo_fd, buffer, count);
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

            // Terminate the server
            printf("Received kill request. Shutting down server...\n");
            exit(0);
        }
        else
        {
            snprintf(resp.message, strlen(resp.message), "Command Not Found!");
        }

        // Send response back to client
        write(client_fifo_fd, &resp, sizeof(resp));

        // Close client FIFO
        close(client_fifo_fd);

        // Log the request
        fprintf(log_file, "Request from client %d: %s\n", req.pid, req.command);
        fflush(log_file);
    }

    return;
}
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
