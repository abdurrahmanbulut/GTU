// Abdurrahman Bulut
// 1901042258

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// *************** Main Function ******************//
int main(int argc, char *argv[]) {

    if (argc < 3 || argc > 4) 
    {
        fprintf(stderr, "Usage: %s filename num-bytes [x]\n", argv[0]);
        exit(1);
    }

    if (argc == 4 && (argv[3][0] != 'x' && argv[3][0] != 'X') )
    {
        fprintf(stderr, "4th argument should be X or x letters!\n");
        exit(1);
    }
    
    int fd, flags, i;
    long num_bytes;
    char *filename;
    
    // *************** Determine whether or not to use lseek ******************//
    int use_lseek = (argc == 4 && (*argv[3] == 'x' || *argv[3] == 'X'));

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if (use_lseek) 
        flags = O_WRONLY | O_CREAT;
    else 
        flags = O_WRONLY | O_CREAT | O_APPEND;
    

    filename = argv[1];
    fd = open(filename, flags, mode); 

    if (fd < 0) 
    {
        perror("Failed to open input file");
        exit(1);
    }

    // I used 'a' character to write to the files.
    char byte = 'a';
    num_bytes = atoi(argv[2]);


    for (i = 0; i < num_bytes; ++i) 
    {
        // If lseek is used, seek to the end of the file
        if (use_lseek) {
            if (lseek(fd, 0, SEEK_END) == -1) {
                perror("Error when calling lseek call");
                exit(1);
            }
        }
        if (write(fd, &byte, 1) != 1) {
            perror("Error when calling write call");
            exit(1);
        }
    }

    if (close(fd) == -1) 
    {
        perror("Failed to close file");
        exit(1);
    }
    
    return 0;

}