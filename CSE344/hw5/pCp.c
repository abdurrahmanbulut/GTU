#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <libgen.h>

typedef struct
{
    int sourceFileDescriptor;
    int destinationFileDescriptor;
    char filename[1024];

} FileItem;

FileItem *buffer = NULL;

int bufferCount = 0, bufferSize = 0, numConsumerThreads = 0;
int num_of_directory_copied = 0, num_of_regular_copied = 0, num_of_fifo_copied = 0;
long long bytes_copied = 0;
bool done = false;

pthread_cond_t bufferEmpty = PTHREAD_COND_INITIALIZER, bufferFull = PTHREAD_COND_INITIALIZER;
pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER, stdoutMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t doneMutex = PTHREAD_MUTEX_INITIALIZER, numOfDirectoryCopiedMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t numOfRegularCopiedMutex = PTHREAD_MUTEX_INITIALIZER, numOfFifoCopiedMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bytesCopiedMutex = PTHREAD_MUTEX_INITIALIZER;

static void copy_directory(char *readName, char *writeName, struct stat fileStat);
static void copy_file(char *readName, char *writeName, struct stat fileStat);
static void print_error(char *message, char *name);
static void add_to_buffer(char *readName, int rfd, int wfd);
static void *producer(void *arguments);
static void *consumer(void *p);
char *getSourceName(char *sourcePath);
void handleThreadError(int errorCode, char *msg);
void handleSigaction(int signo, siginfo_t *info, void *context);

int main(int argc, char *argv[])
{

    // Argument checking
    if (argc != 5)
    {
        printf("Usage: %s [consumer number] [buffer size] [source path] [destination path]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int errorCode;
    void *bytesCopied;
    struct timeval startTime, endTime;
    struct stat fileStat;
    pthread_t producerThread, *consumerThreads;

    numConsumerThreads = atoi(argv[1]);
    bufferSize = atoi(argv[2]);

    if (bufferSize <= 0 || numConsumerThreads <= 0)
    {
        printf("number of consumer and buffer size must be possitive integer!\n");
        exit(EXIT_FAILURE);
    }

    // Setup signal handling
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handleSigaction;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    size_t stacksize;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stacksize);
    pthread_attr_setstacksize(&attr, 5 * stacksize);

    buffer = (FileItem *)malloc(bufferSize * sizeof(FileItem));

    char **arguments = (char **)malloc(2 * sizeof(char *));

    for (int j = 0; j < 2; ++j)
        arguments[j] = (char *)malloc(512 * sizeof(char));

    strcpy(arguments[0], argv[3]);
    strcpy(arguments[1], argv[4]);


    struct stat st = {0};
    if (stat(arguments[0], &st) == -1) {
        printf("Source folder %s does not exist. Check it. Exitting..\n", arguments[0]);
        exit(EXIT_FAILURE);
    }

    // Check if destination directory exists and create it if it doesn't.
    if (stat(arguments[1], &st) == -1) {
        printf("Destination folder %s does not exist. It is created..\n", arguments[1]);
        mkdir(arguments[1], 0700);
    }


    strcat(arguments[1], "/");
    char *temp = getSourceName(arguments[0]);
    strcat(arguments[1], temp);
    free(temp);


    // File stats and folder creation
    lstat(arguments[0], &fileStat) == -1 ? mkdir(arguments[1], 0666) : mkdir(arguments[1], fileStat.st_mode);

    // Start timer
    gettimeofday(&startTime, NULL);

    // Thread creation and joining
    errorCode = pthread_create(&producerThread, &attr, producer, (void *)arguments);

    if (errorCode)
        handleThreadError(errorCode, "pthread_create error for Producer!");

    consumerThreads = (pthread_t *)malloc(numConsumerThreads * sizeof(pthread_t));

    for (int i = 0; i < numConsumerThreads; ++i)
    {
        errorCode = pthread_create(&consumerThreads[i], NULL, consumer, NULL);
        if (errorCode)
            handleThreadError(errorCode, "pthread_create error for Consumer!");
    }

    pthread_attr_destroy(&attr);

    errorCode = pthread_join(producerThread, NULL);

    if (errorCode)
        handleThreadError(errorCode, "pthread_join error for Producer!");

    pthread_mutex_lock(&doneMutex);
    done = true;
    pthread_mutex_unlock(&doneMutex);

    pthread_cond_broadcast(&bufferFull);

    for (int i = 0; i < numConsumerThreads; ++i)
    {
        errorCode = pthread_join(consumerThreads[i], &bytesCopied);
        if (errorCode)
            handleThreadError(errorCode, "pthread_join error for Consumer!");

        pthread_mutex_lock(&bytesCopiedMutex);
        bytes_copied += *(long long *)bytesCopied;
        pthread_mutex_unlock(&bytesCopiedMutex);

        free(bytesCopied);
    }

    // End timer
    gettimeofday(&endTime, NULL);

    // Print results
    printf("\n");
    printf("|-----------------------------------------\n");
    printf("|   Copying finished!                     \n");
    printf("|-----------------------------------------\n");
    printf("| Directories copied:   %d                 \n", num_of_directory_copied);
    printf("| Regular files copied: %d                 \n", num_of_regular_copied);
    printf("| FIFO files copied:    %d                 \n", num_of_fifo_copied);
    printf("| Total bytes copied:   %lld    \n", bytes_copied);
    printf("| Time taken:      %.0lf μs                 \n", (double)(endTime.tv_usec - startTime.tv_usec) + (double)(endTime.tv_sec - startTime.tv_sec) * 1000000);
    printf("|-----------------------------------------\n");
    printf("\n");

    // Cleanup
    pthread_mutex_destroy(&doneMutex);
    pthread_mutex_destroy(&numOfDirectoryCopiedMutex);
    pthread_mutex_destroy(&bufferMutex);
    pthread_mutex_destroy(&stdoutMutex);
    pthread_mutex_destroy(&numOfRegularCopiedMutex);
    pthread_mutex_destroy(&numOfFifoCopiedMutex);
    pthread_mutex_destroy(&bytesCopiedMutex);

    for (int j = 0; j < 2; ++j)
        free(arguments[j]);

    free(arguments);
    free(consumerThreads);
    free(buffer);

    return 0;
}

static void copy_directory(char *readName, char *writeName, struct stat fileStat)
{
    pthread_mutex_lock(&numOfDirectoryCopiedMutex);
    num_of_directory_copied++;
    pthread_mutex_unlock(&numOfDirectoryCopiedMutex);

    char **passArgv = (char **)malloc(2 * sizeof(char *));

    for (int j = 0; j < 2; ++j)
    {
        passArgv[j] = (char *)malloc(512 * sizeof(char));
    }

    strcpy(passArgv[0], readName);
    strcpy(passArgv[1], writeName);

    if (mkdir(writeName, fileStat.st_mode) == -1)
    {
        printf("\nDirectory %s exist. It will be replaced.\n", writeName);
    }

    producer(passArgv);

    for (int j = 0; j < 2; ++j)
        free(passArgv[j]);
    free(passArgv);
}

static void copy_file(char *readName, char *writeName, struct stat fileStat)
{
    int rfd;
    if (S_ISREG(fileStat.st_mode))
    {

        rfd = open(readName, O_RDONLY);
    }
    else if (S_ISFIFO(fileStat.st_mode))
        rfd = open(readName, O_RDONLY | O_NONBLOCK);
    else
        return;

    if (rfd == -1)
    {
        print_error(" %s can not copied !\n", readName);
        return;
    }

    int wfd = open(writeName, O_WRONLY | O_TRUNC | O_CREAT, fileStat.st_mode);

    if (wfd == -1)
    {
        print_error(" %s can not copied !\n", readName);
        close(rfd);
        return;
    }
    else if (wfd > 1024)
    {
        pthread_mutex_lock(&doneMutex);
        done = true;
        pthread_mutex_unlock(&doneMutex);

        print_error("number of open file descriptors limit exceed!", "");
        return;
    }
    add_to_buffer(readName, rfd, wfd);
}

static void print_error(char *message, char *name)
{
    pthread_mutex_lock(&stdoutMutex);
    printf(message, name);
    pthread_mutex_unlock(&stdoutMutex);
}

static void add_to_buffer(char *readName, int rfd, int wfd)
{
    FileItem produced;
    produced.sourceFileDescriptor = rfd;
    produced.destinationFileDescriptor = wfd;
    strcpy(produced.filename, readName);

    // bufferMutex is used in add_to_buffer to ensure thread-safety when modifying shared data.
    pthread_mutex_lock(&bufferMutex);
    while (bufferCount == bufferSize)
    {
        pthread_cond_wait(&bufferEmpty, &bufferMutex);
    }
    buffer[bufferCount] = produced;
    ++bufferCount;
    pthread_cond_broadcast(&bufferFull);
    pthread_mutex_unlock(&bufferMutex);
}

static void *producer(void *arguments)
{
    struct dirent *pDirent;
    DIR *dir;

    struct stat fileStat;

    char readName[512], writeName[512];
    char **params = (char **)arguments;

    lstat(params[1], &fileStat);

    if (!(S_ISDIR(fileStat.st_mode)))
    {
        pthread_mutex_lock(&doneMutex);
        done = true;
        pthread_mutex_unlock(&doneMutex);

        print_error("%s is not a directory! But it will be created..\n", params[1]);
        printf("%s\n", params[1]);
    }

    dir = opendir(params[0]);

    if (dir != NULL)
    {
        while ((pDirent = readdir(dir)) != NULL && done == false)
        {
            if (0 != strcmp(pDirent->d_name, ".") && 0 != strcmp(pDirent->d_name, ".."))
            {
                strcpy(readName, params[0]);
                strcat(readName, "/");
                strcat(readName, pDirent->d_name);

                lstat(readName, &fileStat);

                strcpy(writeName, params[1]);   
                strcat(writeName, "/");
                strcat(writeName, pDirent->d_name);

                if ((S_ISDIR(fileStat.st_mode)))
                {
                    copy_directory(readName, writeName, fileStat);
                }
                else
                {
                    copy_file(readName, writeName, fileStat);
                }
            }
        }
    }
    else
    {
        pthread_mutex_lock(&doneMutex);
        done = true;
        pthread_mutex_unlock(&doneMutex);

        print_error("%s is not a directory!\n", params[0]);
        return NULL;
    }

    closedir(dir);

    return NULL;
}

static void *consumer(void *p)
{
    float *totalBytesCopied = (float *)malloc(sizeof(float));
    *totalBytesCopied = 0;

    struct stat fileStat;

    char *content = NULL;

    while (1)
    {
        pthread_mutex_lock(&bufferMutex);

        // Wait for the buffer to become non-empty
        while (!done && bufferCount == 0)
        {
            pthread_cond_wait(&bufferFull, &bufferMutex);
        }

        // If the buffer is still empty, we're done
        if (bufferCount == 0)
        {
            pthread_mutex_unlock(&bufferMutex);
            break;
        }

        // Remove an item from the buffer
        --bufferCount;
        FileItem item = buffer[bufferCount];
        pthread_cond_signal(&bufferEmpty);

        pthread_mutex_unlock(&bufferMutex);

        // Process the item
        lstat(item.filename, &fileStat);

        // Reallocate content buffer if necessary
        content = realloc(content, fileStat.st_size);
        if (!content)
        {
            close(item.sourceFileDescriptor);
            close(item.destinationFileDescriptor);
            continue;
        }

        ssize_t bytesRead = read(item.sourceFileDescriptor, content, fileStat.st_size);
        if (bytesRead == -1)
        {
            print_error("Failed to read file", item.filename);
            close(item.sourceFileDescriptor);
            close(item.destinationFileDescriptor);
            continue;
        }

        ssize_t bytesWritten = write(item.destinationFileDescriptor, content, bytesRead);
        if (bytesWritten == -1)
        {
            print_error("Failed to write file", item.filename);
            close(item.sourceFileDescriptor);
            close(item.destinationFileDescriptor);
            continue;
        }

        // Success
        *totalBytesCopied += bytesRead;
        print_error("%s is copied successfully!\n", item.filename);

        pthread_mutex_t *copyMutex;
        if (S_ISREG(fileStat.st_mode))
        {
            copyMutex = &numOfRegularCopiedMutex;
            pthread_mutex_lock(copyMutex);
            num_of_regular_copied += 1;
            pthread_mutex_unlock(copyMutex);
        }
        else if (S_ISFIFO(fileStat.st_mode))
        {
            copyMutex = &numOfFifoCopiedMutex;
            pthread_mutex_lock(copyMutex);
            num_of_fifo_copied += 1;
            pthread_mutex_unlock(copyMutex);
        }

        if (close(item.sourceFileDescriptor) == -1)
        {
            print_error("Failed to close source file", item.filename);
        }
        if (close(item.destinationFileDescriptor) == -1)
        {
            print_error("Failed to close destination file", item.filename);
        }
    }

    free(content);
    return (void *)totalBytesCopied;
}

char *getSourceName(char *sourcePath)
{
    char *baseName = basename(sourcePath);
    if (baseName == NULL)
    {
        perror("Error: can't extract file name");
        exit(EXIT_FAILURE);
    }

    char *result = (char *)malloc((strlen(baseName) + 1) * sizeof(char));
    if (result == NULL)
    {
        perror("Fail: allocate memory");
        exit(EXIT_FAILURE);
    }

    strcpy(result, baseName);
    return result;
}
void handleThreadError(int errorCode, char *msg)
{
    errno = errorCode;
    perror(msg);
    exit(EXIT_FAILURE);
}

void handleSigaction(int signo, siginfo_t *info, void *context)
{
    if (signo == SIGINT)
    {
        pthread_mutex_lock(&doneMutex);
        done = true;
        pthread_mutex_unlock(&doneMutex);
     
        printf("Interrupt signal received. Leaving..\n");
    }
}
