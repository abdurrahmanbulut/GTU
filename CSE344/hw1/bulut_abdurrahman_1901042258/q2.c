// Abdurrahman Bulut
// 1901042258

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>


int dup(int oldfd);
int dup2(int oldfd, int newfd);


// *************** Main Function ******************//
int main()
{

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int fd = open("test.txt", O_CREAT | O_RDWR | O_APPEND, mode); // 0-1-2, fd = 3

    if (fd == -1)
    {
        perror("Error when opening the file!");
        exit(1);
    }

    printf("Original file descriptor: %d\n", fd);

    // Duplicate the file descriptor using dup()
    int fd1 = dup(fd); // fd1 = 4

    if (fd1 == -1)
    {
        perror("Error when duplicating file descriptor! - dup()");
        exit(1);
    }

    printf("Duplicate file descriptor using dup(): %d\n", fd1);

    if (close(fd1) == -1)
    {
        perror("Error when closing the file!");
        exit(1);
    }

    // Dup2() with a new value
    int fd2 = dup2(fd, 5);
    if (fd2 == -1)
    {
        perror("Error when duplicating file descriptor! - dup2()");
        exit(1);
    }
    printf("Duplicate file descriptor using dup2() with new value: %d\n", fd2);

    // Dup2() with the same value
    int fd3 = dup2(fd, fd);
    if (fd3 == -1)
    {
        perror("Error when duplicating file descriptor! - dup2()");
        exit(1);
    }
    printf("Duplicate file descriptor using dup2() with the same value: %d\n", fd3);
    printf("Closed file tests:\n");

    
    // *************** Closed files with dup() ******************//
    int closed_fd = open("test.txt", O_CREAT | O_RDWR | O_APPEND, mode);

    if (closed_fd == -1)
    {
        perror("Error when opening the file!");
    }
    else
    {
        if (close(closed_fd) == -1)
        {
            perror("Error when closing the file!");
        }
        int fd4 = dup(closed_fd);
        if (fd4 == -1)
        {
            perror("Error when duplicating file descriptor! - dup()");
            if (errno == EBADF)
            {
                printf("errno is set to EBADF\n");
            }
        }
        else
        {
            printf("Duplicate of closed file descriptor using dup(): %d\n", fd4);
            if (close(fd4) == -1)
            {
                perror("Error when closing the file!");
            }
        }
    }

    // *************** Closed files with dup2() ******************//
    int closed_fd2 = open("abs.txt", O_CREAT | O_RDWR | O_APPEND, mode);
    if (closed_fd2 == -1)
    {
        perror("Error when opening the file!");
    }
    else
    {
        if (close(closed_fd2) == -1)
        {
            perror("Error when closing the file!");
        }
        int fd5 = dup2(closed_fd2, 10);
        if (fd5 == -1)
        {
            perror("Error when duplicating file descriptor! - dup2()");
            if (errno == EBADF)
            {
                printf("errno is set to EBADF\n");
            }
        }
        else
        {
            printf("Duplicate of closed file descriptor using dup2(): %d\n", fd5);
            if (close(fd5) == -1)
            {
                perror("Error when closing the file!");
            }
        }
    }

    if (close(fd) == -1) 
    {
        perror("Failed to close file");
        exit(1);
    }

    return 0;
}

// *************** dup() implementation ******************//
int dup(int oldfd)
{
    if (fcntl(oldfd, F_GETFL) == -1) {
        perror("Invalid file descriptor");
        return -1;
    }

    int newfd = fcntl(oldfd, F_DUPFD, 0);

    if (newfd == -1)
    {
        perror("Duplication error!");
        return -1;
    }
    return newfd;
}

// *************** dup2() implementation ******************//
int dup2(int oldfd, int newfd)
{
    if (fcntl(oldfd, F_GETFL) == -1) {
        errno = EBADF;
        perror("Invalid old file descriptor");
        return -1;
    }

    if (oldfd == newfd)
    {

        if (fcntl(oldfd, F_GETFL) == -1)
        {
            errno = EBADF;
            perror("Invalid file descriptor");
            return -1;
        }
        return oldfd;
    }

    int result = fcntl(oldfd, F_DUPFD, newfd);

    if (result == -1)
    {
        return -1;
    }
    if (result != newfd)
    {
        printf("newfd (2nd argument) file descriptor is closed");
        close(newfd);
    }

    return result;
}
