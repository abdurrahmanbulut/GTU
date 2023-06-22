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
    unsigned int num_files; // Number of files and directories

} Superblock;

#pragma pack(pop)

unsigned short FAT[FAT_ENTRIES];

// Simulating Disk using file
FILE *disk;
FILE *logFile;

// Functions
void list_directory(const char *disk_name, char *path);
// Function to create a directory
void make_directory_recursive(FILE *disk, Superblock sb, unsigned short current_block, char *remaining_path);
// Function to create a directory
void make_directory(const char *disk_name, char *path);

void delete_directory_recursive(FILE *disk, Superblock sb, unsigned short current_block, char *remaining_path);

void delete_directory(const char *disk_name, char *path);


void make_file_recursive(FILE *disk, Superblock sb, unsigned short current_block, char *remaining_path, const char *source_file);

void make_file(const char *disk_name, char *path, const char *source_file);

void dump_filesystem(const char *disk_name);
void traverse_directory(Superblock sb, unsigned short current_block, int *occupied_blocks);


int main(int argc, char *argv[])
{
   

    const char *disk_name = argv[1];
    const char *operation = argv[2];
    char *parameters = argv[3];

    if (strcmp(operation, "dir") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s <filesystem_name> dir <path>\n", argv[0]);
            return 1;
        }
        list_directory(disk_name, parameters);
    }
    else if (strcmp(operation, "mkdir") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s <filesystem_name> mkdir <path>\n", argv[0]);
            return 1;
        }
        make_directory(disk_name, parameters);
    }
    else if (strcmp(operation, "rmdir") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s <filesystem_name> rmdir <path>\n", argv[0]);
            return 1;
        }
        delete_directory(disk_name, parameters);
    }
    else if (strcmp(operation, "dumpe2fs") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s <filesystem_name> dumpe2fs\n", argv[0]);
            return 1;
        }
        dump_filesystem(disk_name);
    }
    else if (strcmp(operation, "write") == 0)
    {
        if (argc != 5)
        {
            fprintf(stderr, "Usage: %s <filesystem_name> write <path> <source_file>\n", argv[0]);
            return 1;
        }
        make_file(disk_name, parameters, argv[4]);
    }
    else
    {
        fprintf(stderr, "Invalid operation.\n");
    }

    return 0;
}

void list_directory(const char *disk_name, char *path)
{
    // Open the file representing the file system
    disk = fopen(disk_name, "rb");
    if (!disk)
    {
        perror("Error opening filesystem");
        exit(1);
    }

    printf("Opened filesystem %s successfully.\n", disk_name);

    // Read Superblock
    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, disk);

    printf("Read Superblock.\n");

    // It only support listing the root directory
    if (strcmp(path, "\\") != 0)
    {
        fprintf(stderr, "Only the root directory is supported.\n");
        fclose(disk);
        exit(1);
    }

    // Locate the root directory
    fseek(disk, sb.root_directory_block * sb.block_size, SEEK_SET);

    // List contents of the root directory
    DirectoryEntry entry;
    printf("Filename    Extension    Size       Date        Time       Attributes\n");
    printf("----------------------------------------------------------------------------\n");

    while (fread(&entry, sizeof(DirectoryEntry), 1, disk))
    {
        if (entry.filename[0] == 0)
        {
            break; // End of directory entries
        }
        printf("%-8.8s    %-7.3s        %-2d    %02d-%02d-%02d    %02d:%02d:%02d        %02X\n",
               entry.filename, entry.extension, entry.size,
               ((entry.date >> 5) & 0x0F), (entry.date & 0x1F), ((entry.date >> 9) + 1980),
               (entry.time >> 11), ((entry.time >> 5) & 0x3F), ((entry.time & 0x1F) * 2),
               entry.attributes);
    }

    fclose(disk);
}
// Function to create a directory
void make_directory_recursive(FILE *disk, Superblock sb, unsigned short current_block, char *remaining_path)
{
    // Read the current directory block
    DirectoryEntry entry;
    fseek(disk, current_block * sb.block_size, SEEK_SET);

    while (fread(&entry, sizeof(DirectoryEntry), 1, disk))
    {
        if (entry.filename[0] == 0)
        {
            // Empty entry found, create new directory entry
            memset(&entry, 0, sizeof(DirectoryEntry));
            strncpy(entry.filename, remaining_path, FILENAME_SIZE);
            entry.attributes = 0x10; // Directory attribute

            // Set time and date
            time_t current_time = time(NULL);
            struct tm *tm_info = localtime(&current_time);
            entry.time = (tm_info->tm_hour << 11) | (tm_info->tm_min << 5) | (tm_info->tm_sec / 2);
            entry.date = ((tm_info->tm_year - 80) << 9) | ((tm_info->tm_mon + 1) << 5) | tm_info->tm_mday;

            // Write the new directory entry
            fseek(disk, -sizeof(DirectoryEntry), SEEK_CUR);
            fwrite(&entry, sizeof(DirectoryEntry), 1, disk);

            return; // Directory created
        }

        if (strcmp(entry.filename, remaining_path) == 0 && entry.attributes == 0x10)
        {
            printf("Directory already exists, no need to create it..\n");
            return;
        }
    }

    // No empty entry found, need to allocate a new block for the directory
    unsigned short next_block = 0;
    for (unsigned short i = 2; i < FAT_ENTRIES; i++)
    {
        if (FAT[i] == 0)
        {
            next_block = i;
            FAT[current_block] = next_block;
            FAT[next_block] = 0xFFF;
            break;
        }
    }

    if (next_block == 0)
    {
        fprintf(stderr, "No space left in directory.\n");
        exit(1);
    }

    // Create new directory entry
    memset(&entry, 0, sizeof(DirectoryEntry));
    strncpy(entry.filename, remaining_path, FILENAME_SIZE);
    entry.attributes = 0x10; // Directory attribute
    entry.first_block = next_block;

    // Set time and date
    time_t current_time = time(NULL);
    struct tm *tm_info = localtime(&current_time);
    entry.time = (tm_info->tm_hour << 11) | (tm_info->tm_min << 5) | (tm_info->tm_sec / 2);
    entry.date = ((tm_info->tm_year - 80) << 9) | ((tm_info->tm_mon + 1) << 5) | tm_info->tm_mday;

    // Write the new directory entry
    fseek(disk, current_block * sb.block_size, SEEK_SET);
    fwrite(&entry, sizeof(DirectoryEntry), 1, disk);

    // Create nested directory
    make_directory_recursive(disk, sb, next_block, "");

    // Create the remaining subdirectories recursively
    char subdirectory[FILENAME_SIZE + EXTENSION_SIZE + 2];
    char *next_slash = strchr(remaining_path, '\\');

    while (next_slash)
    {
        strncpy(subdirectory, remaining_path, next_slash - remaining_path);
        subdirectory[next_slash - remaining_path] = '\0';

        make_directory_recursive(disk, sb, next_block, subdirectory);

        remaining_path = next_slash + 1;
        next_slash = strchr(remaining_path, '\\');
    }

    // Create the last subdirectory
    make_directory_recursive(disk, sb, next_block, remaining_path);
}

// Function to create a directory
void make_directory(const char *disk_name, char *path)
{
    disk = fopen(disk_name, "r+b");
    if (!disk)
    {
        perror("Error opening filesystem");
        exit(1);
    }

    // Read Superblock
    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, disk);

    // Read FAT
    fseek(disk, sb.fat_start_block * sb.block_size, SEEK_SET);
    fread(&FAT, sizeof(FAT), 1, disk);

    // Create the directory recursively
    make_directory_recursive(disk, sb, sb.root_directory_block, path);

    fclose(disk);
}


void delete_directory_recursive(FILE *disk, Superblock sb, unsigned short current_block, char *remaining_path)
{
    // Read the current directory block
    DirectoryEntry entry;
    fseek(disk, current_block * sb.block_size, SEEK_SET);

    while (fread(&entry, sizeof(DirectoryEntry), 1, disk))
    {
        // If directory entry matches the remaining path
        if (strcmp(entry.filename, remaining_path) == 0 && entry.attributes == 0x10)
        {
            // Zero out the directory entry
            memset(&entry, 0, sizeof(DirectoryEntry));

            // Write back the modified directory entry
            fseek(disk, -sizeof(DirectoryEntry), SEEK_CUR);
            fwrite(&entry, sizeof(DirectoryEntry), 1, disk);

            // Mark the blocks used by the directory as free in the FAT
            unsigned short block = entry.first_block;
            while (block != 0xFFF)
            {
                unsigned short next_block = FAT[block];
                FAT[block] = 0;
                block = next_block;
            }

            return;
        }
    }

    // If the desired directory wasn't found, look in subdirectories
    char subdirectory[FILENAME_SIZE + EXTENSION_SIZE + 2];
    char *next_slash = strchr(remaining_path, '\\');

    if (next_slash)
    {
        strncpy(subdirectory, remaining_path, next_slash - remaining_path);
        subdirectory[next_slash - remaining_path] = '\0';

        remaining_path = next_slash + 1;
        delete_directory_recursive(disk, sb, current_block, subdirectory);
        delete_directory_recursive(disk, sb, entry.first_block, remaining_path);
    }
}

void delete_directory(const char *disk_name, char *path)
{
    FILE *disk = fopen(disk_name, "r+b");
    if (!disk)
    {
        perror("Error opening filesystem");
        exit(1);
    }

    // Read Superblock
    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, disk);

    // Read FAT
    fseek(disk, sb.fat_start_block * sb.block_size, SEEK_SET);
    fread(&FAT, sizeof(FAT), 1, disk);

    // Delete the directory recursively
    delete_directory_recursive(disk, sb, sb.root_directory_block, path);

    fclose(disk);
}

void make_file_recursive(FILE *disk, Superblock sb, unsigned short current_block, char *remaining_path, const char *source_file)
{
    // Read the current directory block
    DirectoryEntry entry;
    fseek(disk, current_block * sb.block_size, SEEK_SET);

    while (fread(&entry, sizeof(DirectoryEntry), 1, disk))
    {
        if (entry.filename[0] == 0)
        {
            // Empty entry found, create new file entry
            memset(&entry, 0, sizeof(DirectoryEntry));
            strncpy(entry.filename, remaining_path, FILENAME_SIZE);
            entry.attributes = 0x00; // Regular file attribute

            // Set time and date
            time_t current_time = time(NULL);
            struct tm *tm_info = localtime(&current_time);
            entry.time = (tm_info->tm_hour << 11) | (tm_info->tm_min << 5) | (tm_info->tm_sec / 2);
            entry.date = ((tm_info->tm_year - 80) << 9) | ((tm_info->tm_mon + 1) << 5) | tm_info->tm_mday;

            // Write the new file entry
            fseek(disk, -sizeof(DirectoryEntry), SEEK_CUR);
            fwrite(&entry, sizeof(DirectoryEntry), 1, disk);

            // Copy contents of source file into the new file
            FILE *src_file = fopen(source_file, "rb");
            if (!src_file)
            {
                fprintf(stderr, "Error opening source file: %s\n", source_file);
                exit(1);
            }

            fseek(src_file, 0, SEEK_END);
            unsigned int file_size = ftell(src_file);
            fseek(src_file, 0, SEEK_SET);

            unsigned short current_block = entry.first_block;
            unsigned int remaining_bytes = file_size;

            while (current_block != 0xFFF && remaining_bytes > 0)
            {
                unsigned int bytes_to_write = (remaining_bytes > sb.block_size) ? sb.block_size : remaining_bytes;

                fseek(disk, current_block * sb.block_size, SEEK_SET);
                fread(disk, bytes_to_write, 1, src_file);

                remaining_bytes -= bytes_to_write;
                current_block = FAT[current_block];
            }

            fclose(src_file);

            return; // File created
        }

        if (strcmp(entry.filename, remaining_path) == 0 && entry.attributes == 0x00)
        {
            printf("File already exists, no need to create it..\n");
            return;
        }
    }

    // No empty entry found, need to allocate a new block for the file
    unsigned short next_block = 0;
    for (unsigned short i = 2; i < FAT_ENTRIES; i++)
    {
        if (FAT[i] == 0)
        {
            next_block = i;
            FAT[current_block] = next_block;
            FAT[next_block] = 0xFFF;
            break;
        }
    }

    if (next_block == 0)
    {
        fprintf(stderr, "No space left in directory.\n");
        exit(1);
    }

    // Create new file entry
    memset(&entry, 0, sizeof(DirectoryEntry));
    strncpy(entry.filename, remaining_path, FILENAME_SIZE);
    entry.attributes = 0x00; // Regular file attribute
    entry.first_block = next_block;
    entry.size = 0; // Set initial size to 0

    // Set time and date
    time_t current_time = time(NULL);
    struct tm *tm_info = localtime(&current_time);
    entry.time = (tm_info->tm_hour << 11) | (tm_info->tm_min << 5) | (tm_info->tm_sec / 2);
    entry.date = ((tm_info->tm_year - 80) << 9) | ((tm_info->tm_mon + 1) << 5) | tm_info->tm_mday;

    // Write the new file entry
    fseek(disk, current_block * sb.block_size, SEEK_SET);
    fwrite(&entry, sizeof(DirectoryEntry), 1, disk);

    // Copy contents of source file into the new file
    FILE *src_file = fopen(source_file, "rb");
    if (!src_file)
    {
        fprintf(stderr, "Error opening source file: %s\n", source_file);
        exit(1);
    }

    fseek(src_file, 0, SEEK_END);
    unsigned int file_size = ftell(src_file);
    fseek(src_file, 0, SEEK_SET);

    current_block = entry.first_block;
    unsigned int remaining_bytes = file_size;

    while (current_block != 0xFFF && remaining_bytes > 0)
    {
        unsigned int bytes_to_write = (remaining_bytes > sb.block_size) ? sb.block_size : remaining_bytes;

        fseek(disk, current_block * sb.block_size, SEEK_SET);
        fread(disk, bytes_to_write, 1, src_file);

        remaining_bytes -= bytes_to_write;
        current_block = FAT[current_block];
    }

    fclose(src_file);

    // Create the remaining subdirectories recursively
    char subdirectory[FILENAME_SIZE + EXTENSION_SIZE + 2];
    char *next_slash = strchr(remaining_path, '\\');

    while (next_slash)
    {
        strncpy(subdirectory, remaining_path, next_slash - remaining_path);
        subdirectory[next_slash - remaining_path] = '\0';

        make_directory_recursive(disk, sb, next_block, subdirectory);

        remaining_path = next_slash + 1;
        next_slash = strchr(remaining_path, '\\');
    }

    // Create the last subdirectory
    make_directory_recursive(disk, sb, next_block, remaining_path);
}

void make_file(const char *disk_name, char *path, const char *source_file)
{
    disk = fopen(disk_name, "r+b");
    if (!disk)
    {
        perror("Error opening filesystem");
        exit(1);
    }

    // Read Superblock
    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, disk);

    // Read FAT
    fseek(disk, sb.fat_start_block * sb.block_size, SEEK_SET);
    fread(&FAT, sizeof(FAT), 1, disk);

    // Create the file recursively
    make_file_recursive(disk, sb, sb.root_directory_block, path, source_file);

    fclose(disk);
}

void dump_filesystem(const char *disk_name)
{
    disk = fopen(disk_name, "rb");
    if (!disk)
    {
        perror("Error opening filesystem");
        exit(1);
    }

    // Read Superblock
    Superblock sb;
    fread(&sb, sizeof(Superblock), 1, disk);

    // Print file system information
    printf("Block count: %u\n", sb.total_block_count);
    printf("Free blocks: %u\n", sb.free_block_count);
    printf("Number of files and directories: %u\n", sb.num_files);
    printf("Block size: %u bytes\n", sb.block_size);

    // Traverse the file system to identify occupied blocks
    int occupied_blocks[sb.total_block_count];
    memset(occupied_blocks, 0, sizeof(occupied_blocks));

    // Traverse the root directory and subdirectories
    traverse_directory(sb, sb.root_directory_block, occupied_blocks);

    // Print occupied block information with corresponding file names
    printf("\nOccupied Blocks:\n");
    for (unsigned int i = 2; i < sb.total_block_count; i++)
    {
        if (occupied_blocks[i])
        {
            // Print file name
            char filename[FILENAME_SIZE + EXTENSION_SIZE + 1];
            fseek(disk, i * sb.block_size, SEEK_SET);
            fread(filename, sizeof(filename), 1, disk);
            printf("Block %u: %s\n", i, filename);
        }
    }

    fclose(disk);
}

void traverse_directory(Superblock sb, unsigned short current_block, int *occupied_blocks)
{
    fseek(disk, current_block * sb.block_size, SEEK_SET);

    DirectoryEntry entry;
    while (fread(&entry, sizeof(DirectoryEntry), 1, disk))
    {
        if (entry.filename[0] == 0)
        {
            break; // End of directory entries
        }

        // Check if the entry is a file or a directory
        if ((entry.attributes & 0x10) == 0x10)
        {
            // Directory found, recursively traverse it
            if (strcmp(entry.filename, ".") != 0 && strcmp(entry.filename, "..") != 0)
            {
                unsigned short block = entry.first_block;
                traverse_directory(sb, block, occupied_blocks);
            }
        }
        else
        {
            // File found, mark its block as occupied
            occupied_blocks[entry.first_block] = 1;
        }
    }
}
