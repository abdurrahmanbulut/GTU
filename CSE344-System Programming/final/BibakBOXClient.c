#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libgen.h>
#include <pthread.h>
#include <sys/inotify.h>
#include <limits.h>
#include <pthread.h>


typedef struct {
    const char *directoryPath;
    int clientSocket;
} ThreadParams;

typedef struct
{
    char filename[512];
    mode_t mode;
    char content[4096];
    int bytesRead;
    int flag;
    time_t lastModification;

} FileEntry;

int doneFlag = 0;
int clientSocket;
int lenOfClientDir;
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

void signalHandler(int signo);
void sendClientToServer(int fd, char *name);
void sendClientFiles(int fd, char *clientDir);
void delay(double msec);
void readServer(char* clientSocket, char* clientPath);


void MonitorDirectoryChangesRecursively(const char *directoryPath, int fileDescriptor);
void MonitorDirectoryChanges(const char *directoryPath, int clientSocket);

void *threadFunction(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    MonitorDirectoryChanges(params->directoryPath, params->clientSocket);
    return NULL;
}

int main(int argc, char **argv)
{
    struct stat fileStat;
    int wfd = -1;
    struct sockaddr_in serverAddress;
    socklen_t serverLen;
    char clientPath[512], clientBase[512];
    char *serverIP;

    if (argc < 3 || argc > 4)
    {
        printf("Usage: %s [clientDirPath] [portnumber] (optional)[server ip]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int portnumber = atoi(argv[2]);

    DIR *pDir = opendir(argv[1]);
    if (portnumber <= 2000 || pDir == NULL)
    {
        printf("Usage: %s [clientDirPath] [portnumber] (optional)[server ip]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    closedir(pDir);

    serverIP = (argc == 4) ? argv[3] : "127.0.0.1";

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    strcpy(clientBase, argv[1]);
    strcpy(clientPath, dirname(argv[1]));

    if (strcmp(clientPath, ".") != 0)
    {
        strcat(clientPath, "/");
    }

    strcat(clientPath, basename(clientBase));
    printf("clientPath: %s \n", clientPath); // /home/bulut/Documents/System/Server/Client

    lenOfClientDir = strlen(argv[1]) + 1;

    // CLİENT SOCKET
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("client socket");
        exit(EXIT_FAILURE);
    }

    serverLen = sizeof(serverAddress);
    memset(&serverAddress, 0, serverLen);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);
    serverAddress.sin_port = htons(portnumber);

    // CONNECT TO THE SOCKET
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, serverLen) == -1)
    {
        perror("connect to the server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    FileEntry each;
    strcpy(each.filename, basename(clientBase));
    char message[10];

    // Dosya adı gönder.
    write(clientSocket, &each, sizeof(FileEntry)); // 101w

    memset(&each, 0, sizeof each);
    strcpy(each.content, "Client dir sent.");

    read(clientSocket,  &each, sizeof(FileEntry));  // 101r

    printf("Server response for connection: %s\n", each.content);

    ThreadParams params;
    params.directoryPath = clientPath;
    params.clientSocket = clientSocket;

    pthread_t monitorThread;
    int threadStatus = pthread_create(&monitorThread, NULL, threadFunction, &params);
    if (threadStatus != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    int i = 0;   
    while (doneFlag == 0 && i++<100)
    {
        i++;
        sendClientToServer(clientSocket, clientPath);
        delay(2000);
        readServer(clientSocket, clientPath);
    }

    pthread_join(monitorThread, NULL); // This will wait for the thread to finish.

    return 0;
}
void readServer(char* clientSocket, char* clientPath){

        FileEntry each, resp;
        char message[10];
        struct stat fileStat;
        int wfd = -1;

        // read server dir 4Kb by 4Kb until disconnected
        while (read(clientSocket, &each, sizeof(FileEntry)) > 0)
        {   
            if (each.flag == 3)
            {
                printf("server sent all files..");
                return;
            }

            pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

            char *name = (char *)malloc(512 * sizeof(char));

            strcpy(name, clientPath);
            strcat(name, "/");
            strcat(name, each.filename);
            // name: /home/bulut/Documents/Client/deneme2
            // mode: 16893
            // flag: 0
            
            if (S_ISDIR(each.mode))
            {
                mkdir(name, 0700);
            }
            else
            {
                if (lstat(name, &fileStat) != -1) // file exist
                {
                    if (wfd == -1) // open only once
                    {

                        if (each.lastModification > fileStat.st_mtime)
                        {
                            wfd = open(name, O_WRONLY | O_TRUNC, each.mode);
                        }
                        else // nothing changed
                        {
                            free(name);
                            
                            strcpy(resp.content, "continue");
                            write(clientSocket, &resp, sizeof(FileEntry)); // 103w
					        memset(&resp, 0, sizeof resp); //respo


                            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
                            continue;
                        }
                    }
                    write(wfd, each.content, each.bytesRead);
                }
                else // file doesnt exist
                {
                    if (wfd == -1)
                    {
                        wfd = open(name, O_WRONLY | O_CREAT | O_TRUNC, each.mode);
                    }
                    write(wfd, each.content, each.bytesRead);
                }
            }

            if (each.flag == 1) // file done
            {
                if (!S_ISDIR(each.mode))
                {
                    close(wfd);
                    wfd = -1;
                }
            }
            strcpy(resp.content, "continue");
            write(clientSocket, &resp, sizeof(FileEntry)); // 103w
			memset(&resp, 0, sizeof resp); //respo

            free(name);
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        }
}

void sendClientToServer(int fd, char *name)
{
    char message[100];
    sendClientFiles(fd, name);
    FileEntry each, resp;
    
    memset(&each, 0, sizeof each);
    each.flag = 2;
    
    write(fd, &each, sizeof(FileEntry));

    memset(&resp, 0, sizeof resp);
    read(fd,  &resp, sizeof(FileEntry));  // 101r

    if (strcmp(resp.content, "shutdown") == 0)
    {
        printf("Server shutted down.\n");
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void sendClientFiles(int fd, char *clientDir)
{

    DIR *pDir;
    struct dirent *pDirent;
    struct stat fileStat;
    FileEntry resp;
    char name[512], message[10];
    int size, rSize;

    pDir = opendir(clientDir);
    if (pDir != NULL)
    {

        while ((pDirent = readdir(pDir)) != NULL)
        {

            if (0 != strcmp(pDirent->d_name, ".") && 0 != strcmp(pDirent->d_name, ".."))
            {
                strcpy(name, clientDir);
                strcat(name, "/");
                strcat(name, pDirent->d_name);
                lstat(name, &fileStat);

                if ((S_ISDIR(fileStat.st_mode)))
                {
                    FileEntry each;
                    memset(&each, 0, sizeof each);
                    memset(each.filename, '\0', sizeof(each.filename));
                    memcpy(each.filename, &name[lenOfClientDir], strlen(name) - lenOfClientDir);

                    
                    // Remove the first directory name "Client" from the path
                    char *relativePath = strchr(each.filename, '/'); 
                    if (relativePath != NULL)
                    {
                        relativePath++; // Skip the leading slash
                        strncpy(each.filename, relativePath, sizeof(each.filename) - 1);
                    }

                    each.flag = 1;

                    each.mode = fileStat.st_mode;

                    write(fd, &each, sizeof(FileEntry)); // 102w

                    memset(&resp, 0, sizeof resp);
                    read(fd,  &resp, sizeof(FileEntry));  // 101r                    

                    if (strcmp(resp.content, "shutdown") == 0)
                    {
                        printf("Server shutted down.\n");
                        closedir(pDir);
                        close(fd);
                        exit(EXIT_FAILURE);
                    }
                    strcpy(message, "");
                    sendClientFiles(fd, name);
                }

                else
                {
                    int rfd;
                    if (S_ISREG(fileStat.st_mode))
                    {
                        rfd = open(name, O_RDONLY);
                    }
                    else if (S_ISFIFO(fileStat.st_mode))
                    {
                        rfd = open(name, O_RDONLY | O_NONBLOCK);
                    }
                    else
                    {
                        continue;
                    }
                    if (rfd == -1)
                    {
                        continue;
                    }

                    FileEntry each;
                    memset(&each, 0, sizeof each);
                    size = (int)fileStat.st_size;
                    memset(each.filename, '\0', sizeof(each.filename));
                    memcpy(each.filename, &name[lenOfClientDir], strlen(name) - lenOfClientDir);

                    // Remove the first directory name "Client" from the path
                    char *relativePath = strchr(each.filename, '/');
                    if (relativePath != NULL)
                    {
                        relativePath++; // Skip the leading slash
                        strncpy(each.filename, relativePath, sizeof(each.filename) - 1);
                    }

                    each.mode = fileStat.st_mode;
                    each.flag = 0;
                    each.lastModification = fileStat.st_mtime;

                    do
                    {
                        rSize = read(rfd, each.content, sizeof(each.content));
                        if (rSize < 0)
                            break;
                        size -= sizeof(each.content);
                        if (size <= 0)
                            each.flag = 1; // this file done

                        each.bytesRead = rSize;
                        write(fd, &each, sizeof(FileEntry)); // 102w

                        memset(&resp, 0, sizeof resp);
                        read(fd,  &resp, sizeof(FileEntry));  // 101r 

                        strcpy(each.content, "");            // clear content

                        if (strcmp(resp.content, "shutdown") == 0)
                        {
                            printf("Server shutted down.\n");
                            closedir(pDir);
                            close(fd);
                            exit(EXIT_FAILURE);
                        }
                        strcpy(message, "");
                    } while (rSize > 0);
                    close(rfd);
                }
            }
        }
    }

    closedir(pDir);
}

void delay(double msec)
{
    double dif = 0.0;
    struct timeval start, end;

    gettimeofday(&start, NULL);
    while (dif <= msec)
    {
        gettimeofday(&end, NULL);
        dif = (double)(end.tv_usec - start.tv_usec) / 1000 + (double)(end.tv_sec - start.tv_sec) * 1000;
    }
}

void signalHandler(int signo)
{
    if (signo == SIGINT)
        printf("SIGINT handled.\n");
    else if (signo == SIGTERM)
        printf("SIGTERM handled.\n");

    exit(0);
}

void MonitorDirectoryChangesRecursively(const char *directoryPath, int fileDescriptor)
{
    int watchDescriptor = inotify_add_watch(fileDescriptor, directoryPath, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE);
    if (watchDescriptor == -1)
    {
        perror("inotify_add_watch");
        return;
    }

    DIR *dir = opendir(directoryPath);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char filePath[PATH_MAX];
        snprintf(filePath, PATH_MAX, "%s/%s", directoryPath, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            MonitorDirectoryChangesRecursively(filePath, fileDescriptor);
        }
    }

    closedir(dir);
}

void MonitorDirectoryChanges(const char *directoryPath, int clientSocket)
{
    int fileDescriptor = inotify_init();
    if (fileDescriptor == -1)
    {
        perror("inotify_init");
        return;
    }

    MonitorDirectoryChangesRecursively(directoryPath, fileDescriptor);

    FileEntry resp;
    char buffer[BUF_LEN];
    while (1)
    {
        int bytesRead = read(fileDescriptor, buffer, BUF_LEN);
        if (bytesRead <= 0)
        {
            perror("read");
            break;
        }

        int offset = 0;
        while (offset < bytesRead)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[offset];
            if (event->len)
            {
                char filePath[PATH_MAX];
                snprintf(filePath, PATH_MAX, "%s/%s", event->wd == 1 ? directoryPath : "", event->name);

                if (event->mask & IN_CREATE)
                {

                    sendClientToServer(clientSocket, directoryPath);
                    readServer(clientSocket, directoryPath);
                }
                else if (event->mask & IN_MODIFY)
                {
                   sendClientToServer(clientSocket, directoryPath);
                    readServer(clientSocket, directoryPath);
                }
                else if (event->mask & IN_DELETE)
                {
                    sendClientToServer(clientSocket, directoryPath);
                    readServer(clientSocket, directoryPath);
                }
                else if (event->mask & IN_MOVED_FROM)
                {
                    sendClientToServer(clientSocket, directoryPath);
                    readServer(clientSocket, directoryPath);
                }
                else if (event->mask & IN_MOVED_TO)
                {
                    sendClientToServer(clientSocket, directoryPath);
                    readServer(clientSocket, directoryPath);
                }
            }
            offset += EVENT_SIZE + event->len;
        }
    }

    close(fileDescriptor);
}
