# File System Project

This project implements a simple file system using the File Allocation Table (FAT12) structure. The file system allows users to perform various operations such as creating directories, listing directory contents, removing directories, creating files, and examining the file system structure.

## Project Structure

The project is divided into three parts:

1. **Part 1: File System Design**
   - Includes my report file.
   - Defines the directory table, directory entries, and the superblock structure.
   - Determines how and where the free blocks are stored.
   - Creates the file system data file with the specified block size.

2. **Part 2: File System Operation Program**
   - Implements the program `fileSystemOper` to perform file system operations.
   - Uses the file system data file created in Part 1 to modify and access the file system.
   - Supports operations such as listing directory contents, creating directories, removing directories, and examining the file system.

3. **Part 3: File System Operation Functions**
   - Implements the actual functionality for file system operations.
   - Provides functions to perform directory listing, directory creation, directory removal, file creation, and file examination.

## Getting Started

To use the file system and perform operations, follow these steps:

1. **Compile the Program**
   - Run the appropriate makefile provided in the project directory to compile the program. ( command is "make").
   - Ensure that you have the necessary dependencies installed (C++ compiler, standard libraries).
   - This will generate the executable file (`fileSystemOper`).

2. **Create the File System**
   - Run the command `./fileSystemOper <block_size_kb>  mySystem.dat ` to create the file system.
   - Replace `<block_size_kb>` with the desired block size in kilobytes (e.g., 1, 2, 4).

3. **Perform File System Operations**
   - Use the command `./fileSystemOper mySystem.dat <operation> <parameters>` to perform various file system operations.
   - Replace `<operation>` with the desired operation (e.g., `dir`, `mkdir`, `rmdir`, `makefile`, `dumpe2fs`).
   - Replace `<parameters>` with the necessary parameters for the operation.

4. **Examine File System Information**
   - The `dir` operation lists all directories and files in the file system. 
   - The `dumpe2fs` operation can be used to display detailed information about the file system.

## Supported Operations

The program supports the following file system operations:

- `dir <path>`: Lists the contents of the directory at the specified path.
- `mkdir <path>`: Creates a new directory at the specified path.
- `rmdir <path>`: Removes the directory at the specified path.
- `dumpe2fs`: Displays detailed information about the file system.
- `write`: Creates a file in the file system.
- It currently does not support `read` and `del`

**Note:** All operations are performed on the same file system data file (`mySystem.dat`) created in Part 2.

## Additional Notes

- The file system supports a maximum partition size based on the block size chosen during creation.
- The project provides additional helper functions and structures to handle file system operations.

## Authors

- [Abdurrahman Bulut](https://github.com/abdurrahmanbulut)

