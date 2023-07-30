/////////////////////////7/
// Abdurrahman Bulut
//    1901042258
///////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_COMMANDS 20
#define BUFFER_SIZE 1024

typedef struct
{
    char *cmd;
    pid_t pid;
} CmdInfo;

CmdInfo cmd_info[MAX_COMMANDS];
int num_child_processes = 0;

int count_args(char **args) {
    int count = 0;
    while (args[count] != NULL) {
        count++;
    }
    return count;
}

void kill_child_processes(int);
void signal_handler(int, siginfo_t *, void *);
void setup_signal_handlers();
char ***parse_user_input(char *input);
char **parse_cmd_args(char *command);
void run_commands(char ***, FILE *);

// ################################
// ######## MAIN FUNCTİON #########
// ################################
int main()
{

    setup_signal_handlers();
    char input[BUFFER_SIZE];
    char ***commands;
    time_t t;

    while (1)
    {
        printf("terminal> ");
        fgets(input, BUFFER_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, ":q") == 0)
        {
            kill_child_processes(SIGTERM);
            return 0;
        }

        t = time(NULL);
        char log_filename[256];
        strftime(log_filename, sizeof(log_filename), "%Y%m%d%H%M%S.log", localtime(&t));
        FILE *log_file = fopen(log_filename, "a");

        commands = parse_user_input(input);

        run_commands(commands, log_file);
        fclose(log_file);

        for (int i = 0; i < num_child_processes; ++i)
        {
            free(commands[i]);
        }
        free(commands);
    }

    return 0;
}

void kill_child_processes(int signum)
{
    for (int i = 0; i < num_child_processes; i++)
    {
        if (cmd_info[i].pid > 0)
        {
            printf("Terminating child process with Pid : %d\n", cmd_info[i].pid);
            kill(cmd_info[i].pid, signum);
        }
    }
    int status;
    pid_t pid;
    // :q çalışsın diye.  will not block if no child processes have changed state. 
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (int i = 0; i < num_child_processes; i++)
        {
            if (cmd_info[i].pid == pid)
            {
                printf("Child process with PID %d exited, status code: %d\n", pid, status);
                num_child_processes--;
                break;
            }
        }
    }
}

void signal_handler(int signum, siginfo_t *siginfo, void *context)
{

    if (signum == SIGINT)
    {
        printf("\nReceived signal %d, stopping child processes...\n", signum);
        kill_child_processes(signum);
        return;
    }

    printf("Received signal %d, stopping child processes and exiting...\n", signum);
    kill_child_processes(signum);
    exit(EXIT_FAILURE);
}

void setup_signal_handlers()
{

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = signal_handler;

    if (sigaction(SIGINT, &act, NULL) < 0)
    {
        perror("sigaction (SIGINT) error");
        exit(1);
    }

    if (sigaction(SIGTERM, &act, NULL) < 0)
    {
        perror("sigaction (SIGTERM) error");
        exit(1);
    }
}

char ***parse_user_input(char *input)
{
    char ***commands = malloc(20 * sizeof(char **));
    char *command;
    int index = 0;

    char *saveptr1;

    command = strtok_r(input, "|", &saveptr1);

    while (command != NULL && index < MAX_COMMANDS)
    {
        commands[index++] = parse_cmd_args(command);
        command = strtok_r(NULL, "|", &saveptr1);
    }

    if (index <= 20)
    {
        num_child_processes = index;
    }

    return commands;
}
char **parse_cmd_args(char *command)
{

    char **args = malloc(64 * sizeof(char *));
    char *arg;
    int index = 0;

    arg = strtok(command, " ");
    while (arg != NULL)
    {
        args[index++] = arg;
        arg = strtok(NULL, " ");
    }

    args[index] = NULL;

    return args;
}

void run_commands(char ***commands, FILE *log_file)
{
    int i;
    int in_fd = 0;
    int out_fd = 1;
    int pipe_fd[2];

    for (i = 0; i < num_child_processes; ++i)
    {
        char **args = commands[i];

        if (i != 0)
        {
            in_fd = pipe_fd[0];
        }

        if (i != num_child_processes - 1)
        {
            pipe(pipe_fd);
            out_fd = pipe_fd[1];
        }
        else
        {
            out_fd = 1;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            if (in_fd != 0)
            {
                dup2(in_fd, 0);
                close(in_fd);
            }

            if (out_fd != 1)
            {
                dup2(out_fd, 1);
                close(out_fd);
            }

            for (int j = 0; args[j]; ++j)
            {
                if (strcmp(args[j], "<") == 0)
                {
                    int file_fd = open(args[j + 1], O_RDONLY);
                    
                    if (file_fd == -1)
                    {
                        perror("Error opening input file");
                        exit(EXIT_FAILURE);
                    }

                    dup2(file_fd, 0);
                    args[j] = NULL;
                    close(file_fd);
                }
                else if (strcmp(args[j], ">") == 0)
                {
                    int file_fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (file_fd == -1)
                    {
                        perror("Error opening output file");
                        exit(EXIT_FAILURE);
                    }
                    dup2(file_fd, 1);
                    args[j] = NULL;
                    close(file_fd);
                }
            }

            // execvp kullanmak yerine
            int arg_count = count_args(args);
            char *cmd_str = NULL;
            int cmd_str_len = 0;
            for (int j = 0; j < arg_count; ++j) {
                cmd_str_len += strlen(args[j]) + 1;
                cmd_str = realloc(cmd_str, cmd_str_len);
                if (j == 0) {
                    strcpy(cmd_str, args[j]);
                } else {
                    strcat(cmd_str, " ");
                    strcat(cmd_str, args[j]);
                }
            }

            if (execl("/bin/sh", "sh", "-c", cmd_str, (char *)NULL) == -1) {
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
            free(cmd_str);
        }
        else if (pid < 0)
        {
            perror("Error forking");
        }
        else
        {
            cmd_info[i].cmd = args[0];
            cmd_info[i].pid = pid;
            
            int status;
            waitpid(pid, &status, 0);

            fprintf(log_file, "command: %s , pid: %d\n", args[0], pid);

            if (in_fd != 0)
            {
                close(in_fd);
            }

            if (out_fd != 1)
            {
                close(out_fd);
            }
        }
    }
}