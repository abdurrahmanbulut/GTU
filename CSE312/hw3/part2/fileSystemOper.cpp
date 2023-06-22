#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
#define TOTAL_BLOCKS 4096
#define FAT_ENTRIES (TOTAL_BLOCKS)
#define FILENAME_SIZE 32
#define EXTENSION_SIZE 3

// Structures
#pragma pack(push, 1)

typedef struct
{
    char filename[FILENAME_SIZE];
    char extension[EXTENSION_SIZE];
    unsigned char attributes;
    char reserved[10];
    unsigned short time;
    unsigned short date;
    unsigned short first_block;
    unsigned int size;
} DirectoryEntry;


typedef struct
{
    unsigned int signature;
    unsigned int total_block_count;
    unsigned int free_block_count;
    unsigned short root_directory_block;
    unsigned short fat_start_block;
    unsigned short fat_block_count;
    unsigned int block_size;
} Superblock;

#pragma pack(pop)

unsigned short FAT[FAT_ENTRIES];

// Simulating Disk using file
FILE *disk;
FILE *logFile;

// Functions
void makeFileSystem(int block_size_kb, const char *disk_name)
{
    int block_size = block_size_kb * 1024;

    unsigned int file_system_size;
    switch (block_size_kb)
    {
    case 0:
        file_system_size = 2 * 1024 * 1024; // 2MB
        break;
    case 1:
        file_system_size = 4 * 1024 * 1024; // 4MB
        break;
    case 2:
        file_system_size = 8 * 1024 * 1024; // 8MB
        break;
    case 4:
        file_system_size = 16 * 1024 * 1024; // 16MB
        break;
    default:
        fprintf(stderr, "Invalid block size: %dKB\n", block_size_kb);
        exit(1);
    }

    disk = fopen(disk_name, "wb+");
    if (!disk)
    {
        perror("Error creating filesystem");
        exit(1);
    }

    // Initialize Superblock
    Superblock sb;
    sb.signature = 0xABCD1234;
    sb.total_block_count = file_system_size / block_size;
    sb.free_block_count = sb.total_block_count - 2; // Superblock + FAT
    sb.root_directory_block = 2;                    // Superblock at 0, FAT at 1
    sb.fat_start_block = 1;
    sb.fat_block_count = 1;
    sb.block_size = block_size;

    // Write Superblock to disk
    fwrite(&sb, sizeof(Superblock), 1, disk);

    // Initialize FAT
    memset(FAT, 0, sizeof(FAT));
    FAT[0] = 0xFFF; // Mark superblock as used
    FAT[1] = 0xFFF; // Mark FAT as used

    // Log Superblock to a text file
    logFile = fopen("log.txt", "w");
    if (!logFile)
    {
        perror("Error creating log file");
        exit(1);
    }

    fprintf(logFile, "Superblock:\n");
    fprintf(logFile, "  Signature: 0x%X\n", sb.signature);
    fprintf(logFile, "  Total Block Count: %u\n", sb.total_block_count);
    fprintf(logFile, "  Free Block Count: %u\n", sb.free_block_count);
    fprintf(logFile, "  Root Directory Block: %u\n", sb.root_directory_block);
    fprintf(logFile, "  FAT Start Block: %u\n", sb.fat_start_block);
    fprintf(logFile, "  FAT Block Count: %u\n", sb.fat_block_count);
    fprintf(logFile, "  Block Size: %u\n", sb.block_size);

    // Write FAT to disk
    fseek(disk, block_size, SEEK_SET);
    fwrite(FAT, sizeof(FAT), 1, disk);


    // Log FAT to a text file
    fprintf(logFile, "FAT:\n");
    for (int i = 0; i < FAT_ENTRIES; i++)
    {
        fprintf(logFile, "  Entry %d: 0x%X\n", i, FAT[i]);
    }
    float MB = file_system_size / 1048576.0f;

    printf("Maximum Partition Size: %.2f MB\n", MB);


    // Ensure that the file size is exactly the maximum for the block size
    char zero = 0;
    fseek(disk, file_system_size - 1, SEEK_SET);
    fwrite(&zero, 1, 1, disk);

    fclose(disk);
    fclose(logFile);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <block_size_kb> <disk_name>\n", argv[0]);
        return 1;
    }

    int block_size_kb = atoi(argv[1]);
    const char *disk_name = argv[2];

    makeFileSystem(block_size_kb, disk_name);

    printf("File sistem is created succesfully\n");

    printf("Now you can see the results in the created log.txt file...\n\n");

    return 0;
}