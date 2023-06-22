// Abdurrahman Bulut
// 1901042258

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>


int dup(int oldfd);

// *************** Main Function ******************//
int main() {

    int fd1, fd2;
    off_t off1, off2;


    fd1 = open("test.txt", O_RDONLY);
    if (fd1 == -1)
    {
        perror("Error when opening the file!");
        exit(1);
    }


    fd2 = dup(fd1);
    if (fd1 == -1)
    {
        perror("Error when duplicating file descriptor! - dup()");
        exit(1);
    }

    // Whether fds share the same file
    printf("File descriptors are %s\n", fd1 == fd2 ? "same" : "different");


    // *************** Offsets of fd1 and fd2 ******************//
    off1 = lseek(fd1, 0, SEEK_CUR);
    off2 = lseek(fd2, 0, SEEK_CUR);
    printf("Offset of fd1: %ld\nOffset of fd2: %ld\n", (long) off1, (long) off2);


    // *************** Read from the file using both file descriptors ******************//
    char buffer[6];
    if (read(fd1, buffer, 5) < 0 ) {
        perror("Error when reading the file!");
        exit(1);
    }
    printf("Contents of fd1: %s\n", buffer);

    if (read(fd2, buffer, 5) < 0) {
        perror("Error when reading the file!");
        exit(1);
    }
    printf("Contents of fd2: %s\n", buffer);


    // *************** New Offsets of fd1 and fd2 ******************//
    off1 = lseek(fd1, 0, SEEK_CUR);
    off2 = lseek(fd2, 0, SEEK_CUR);

    printf("Offset of fd1: %ld\nOffset of fd2: %ld\n", (long) off1, (long) off2);



    if (close(fd1) == -1 || close(fd2) == -1 )
    {
        perror("Error when closing the file!");
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
