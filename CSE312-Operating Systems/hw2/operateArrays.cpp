#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define FILL 0
#define MULT1 1
#define MULT2 2
#define ARRSUM 3
#define LINEARSEARCH 4
#define BINARYSEARCH 5
#define INDEX 6
#define CHECK 7

unsigned long time_mult1;
unsigned long time_mult2;
unsigned long time_arrsum;
unsigned long time_linear;
unsigned long time_binary;

struct entry
{
    unsigned long recent_access_time;
    int lru_used;
    int holding_page;
    int reference_bit;
    int modified_bit;
    int is_present;
};

struct entry *physical_page_array;
struct entry *virtual_page_array;

struct statistics
{
    int reads;
    int writes;
    int disk_reads;
    int disk_writes;
    int page_misses;
    int page_replacement;
};

struct statistics statistics[8];

int second_chance_index[4], ws_clock_index[4];
int lru_counter;

int frame_size, num_physical, num_virtual, page_table_print_int;
char page_replacement[20], alloc_policy[20], disk_file_name[500];

int physical_mem_page_num, virtual_mem_page_num;
int frame_size_int, num_physical_int, num_virtual_int;

pthread_mutex_t mutex;
int thread_no[4];
pthread_t tid[4];

int memory_access;

int *virtual_memory;
int *physical_memory;

int *mult1arr;
int *mult2arr;

/* Functions */
void *thread_func(void *arg);
void linear_search(int *virtual_array, int start_index, int size, int target);
int binary_search(int *virtual_array, int start_index, int end_index, int target);
void bubble_sort(int *virtual_array, int start_index, int end_index);
int *mult1(int *virtual_array, int start_index, int end_index);
int *mult2(int *virtual_array, int start_index, int end_index);
void arr_sum(int *arr);
void print_statistics();
void printPageTable();
void initializeMemories();
int get_value_from_physical_mem(unsigned int index);
void fillMemory();
void SC(int disk_page_num, int quarter);
void LRU(int disk_page_num, int quarter);
void WSClock(int disk_page_num, int quarter);
void setValue(unsigned int index, int value, char *tName);
int get_disk(unsigned int index);
int get(unsigned int index, char *tName);
void get_page_in_disk(int disk_page_num, int physical_page_num);
int is_in_physical_mem(int index);
void free_memories();
int get_direct_disk(unsigned int index);
unsigned long getMicroseconds();

/* Main function */
int main(int argc, char *argv[])
{

    if (argc != 8)
    {
        perror("There should be 8 arguments..");
        exit(EXIT_FAILURE);
    }

    frame_size = atoi(argv[1]);
    num_physical = atoi(argv[2]);
    num_virtual = atoi(argv[3]);
    strcpy(page_replacement, argv[4]);
    strcpy(alloc_policy, argv[5]);
    page_table_print_int = atoi(argv[6]);
    strcpy(disk_file_name, argv[7]);

    if (num_physical > num_virtual)
    {
        printf("Error: Invalid number of arguments. Please provide 8 arguments.\n");
        printf("Usage: ./operateArrays <frame_size> <num_physical> <num_virtual> <page_replacement> <alloc_policy> <page_table_print_int> <file_name>\n");
        exit(EXIT_FAILURE);
    }

    printf("System Info:\n");
    printf("Frame Size: %d (%d integers)\n", frame_size, frame_size * 32);
    printf("Number of Physical Frames: %d (%d integers)\n", num_physical, frame_size * num_physical);
    printf("Number of Virtual Frames: %d (%d integers)\n", num_virtual, frame_size * num_virtual);
    printf("Page Replacement Algorithm: %s\n", page_replacement);
    printf("Allocation Policy: %s\n", alloc_policy);
    printf("Page Table Print Interval: %d\n", page_table_print_int);
    printf("Disk File Name: %s\n", disk_file_name);
    printf("\n");

    /* Initializes the memory and disk */
    initializeMemories();
    setValue(0, 0, "fill");

    /* Initializing the posix mutex */
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("Mutex create error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 4; ++i)
    {
        thread_no[i] = i;
        if (pthread_create(&tid[i], NULL, thread_func, (void *)&thread_no[i]) != 0)
        {
            perror("Thread create error");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (pthread_join(tid[i], NULL) == -1)
        {
            perror("Thread join error");
            exit(EXIT_FAILURE);
        }
    }

    /* Destorying the mutexes */
    if (pthread_mutex_destroy(&mutex) != 0)
    {
        perror("Mutex destroy error");
        exit(EXIT_FAILURE);
    }

    // Printing virtual memory
    for (int i = 0; i < num_virtual_int; ++i)
    {

        printf("index:%d   ----   value:%d \n", virtual_memory[i], get_disk(i));
    }

    print_statistics();
    free_memories();

    return 0;
}

/* Gets time as microseconds */
unsigned long getMicroseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

/* Prints the page table */
void printPageTable()
{
    int i;
    printf("***PAGE TABLE PRINT***\n");
    for (i = 0; i < virtual_mem_page_num; ++i)
    {
        printf("index:%d,holding page:%d,reference bit:%d,modified bit %d,present bit:%d,recent access time %ld\n", i, virtual_page_array[i].holding_page, virtual_page_array[i].reference_bit, virtual_page_array[i].modified_bit, virtual_page_array[i].is_present, virtual_page_array[i].recent_access_time);
    }
    printf("\n");
}

// Initializes the memories
void initializeMemories()
{
    int i;
    /* Memory variables initializes */
    frame_size_int = pow(2, frame_size);
    physical_mem_page_num = pow(2, num_physical);
    virtual_mem_page_num = pow(2, num_virtual);
    num_physical_int = frame_size_int * physical_mem_page_num;
    num_virtual_int = frame_size_int * virtual_mem_page_num;

    /* Memory initializes */
    virtual_memory = (int *)calloc(num_virtual_int, sizeof(int));
    physical_memory = (int *)calloc(num_physical_int, sizeof(int));
    physical_page_array = (struct entry *)calloc(physical_mem_page_num, sizeof(struct entry));
    virtual_page_array = (struct entry *)calloc(virtual_mem_page_num, sizeof(struct entry));

    /* Statistic variable's initializes */
    for (i = 0; i < 6; ++i)
    {
        statistics[i].disk_reads = 0;
        statistics[i].disk_writes = 0;
        statistics[i].page_misses = 0;
        statistics[i].page_replacement = 0;
        statistics[i].reads = 0;
        statistics[i].writes = 0;
    }

    /* Indexes and memory_access initializes */
    second_chance_index[0] = 0;
    second_chance_index[1] = 0;
    second_chance_index[2] = 0;
    second_chance_index[3] = 0;
    ws_clock_index[0] = 0;
    ws_clock_index[1] = 0;
    ws_clock_index[2] = 0;
    ws_clock_index[3] = 0;
    memory_access = 0;
    lru_counter = 0;
}

/* Fills memory randomly */
void fillMemory()
{
    int i, j;
    /* Opens the disk file for writing disk */
    FILE *disk_file = fopen(disk_file_name, "w+");
    if (disk_file == NULL)
    {
        perror("Fopen error.");
        exit(EXIT_FAILURE);
    }
    srand(1000);
    /* Fills the virtual memory addresses */
    for (i = 0; i < (num_virtual_int / 4) * 3; ++i)
    {
        virtual_memory[i] = i;
        ++statistics[FILL].writes;
    }
    for (j = num_virtual_int - 1; i < num_virtual_int; ++i, --j)
    {
        virtual_memory[i] = j;
        ++statistics[FILL].writes;
    }

    /* Fills virtual memory values(in disk and physical memory) randomly */
    for (i = 0; i < num_physical_int; ++i)
    {
        physical_memory[i] = rand();
        fprintf(disk_file, "%.10d\n", physical_memory[i]);
        ++statistics[FILL].writes;
    }
    for (; i < num_virtual_int; ++i)
    {
        fprintf(disk_file, "%.10d\n", rand());
        ++statistics[FILL].disk_writes;
    }

    /* Fills page tables */
    for (i = 0; i < physical_mem_page_num; ++i)
    {
        physical_page_array[i].lru_used = 0;
        physical_page_array[i].holding_page = i;
        physical_page_array[i].modified_bit = 0;
        physical_page_array[i].recent_access_time = getMicroseconds();
        physical_page_array[i].reference_bit = 0;
    }

    for (i = 0; i < virtual_mem_page_num; ++i)
    {
        virtual_page_array[i].lru_used = 0;
        virtual_page_array[i].holding_page = i;
        virtual_page_array[i].modified_bit = 0;
        virtual_page_array[i].recent_access_time = getMicroseconds();
        virtual_page_array[i].reference_bit = 0;
    }

    /* Closes disk file */
    if (fclose(disk_file) != 0)
    {
        perror("Fclose error.");
        exit(EXIT_FAILURE);
    }
}

/* It applies the second chance page replacement algorithm based on the sc index. If it is local,
 it replaces only 1/4 of the physical memory, if it is global, it replaces all of it. */
void SC(int disk_page_num, int quarter)
{
    /* Local policy */
    if (strcmp(alloc_policy, "local") == 0)
    {
        int quarter_of_physicals = (physical_mem_page_num / 4);
        int start_index = quarter * quarter_of_physicals;
        if (second_chance_index[quarter] == 0)
        {
            second_chance_index[quarter] = start_index;
        }
        while (physical_page_array[second_chance_index[0]].reference_bit != 0)
        {
            physical_page_array[second_chance_index[0]].reference_bit = 0;
            ++second_chance_index[0];
            second_chance_index[0] %= physical_mem_page_num;
            if (second_chance_index[quarter] == 0)
            {
                second_chance_index[quarter] = start_index;
            }
        }
        get_page_in_disk(disk_page_num, second_chance_index[quarter]);
        ++second_chance_index[quarter];
        second_chance_index[quarter] %= (start_index + quarter_of_physicals);
    }
    else if (strcmp(alloc_policy, "global") == 0)
    {
        while (physical_page_array[second_chance_index[0]].reference_bit != 0)
        {
            physical_page_array[second_chance_index[0]].reference_bit = 0;
            ++second_chance_index[0];
            second_chance_index[0] %= physical_mem_page_num;
        }
        get_page_in_disk(disk_page_num, second_chance_index[0]);
        ++second_chance_index[0];
        second_chance_index[0] %= physical_mem_page_num;
    }
    /* Inverted policy */
    else if (strcmp(alloc_policy, "inverted") == 0)
    {
        int inverted_index = physical_mem_page_num - 1;
        while (physical_page_array[inverted_index].reference_bit != 0)
        {
            physical_page_array[inverted_index].reference_bit = 0;
            --inverted_index;
            if (inverted_index < 0)
            {
                inverted_index = physical_mem_page_num - 1;
            }
        }
        get_page_in_disk(disk_page_num, inverted_index);
        inverted_index--;
        if (inverted_index < 0)
        {
            inverted_index = physical_mem_page_num - 1;
        }
    }
}

/* Resets all page numbers used for a specified amount of time */
void reset_lru_time()
{
    int i;
    for (i = 0; i < physical_mem_page_num; ++i)
    {
        physical_page_array[i].lru_used = 0;
    }
}

/* Applies last recently used algorithm. Finds the oldest and replaces it by looking at the recent access time in the page tables in the physical memory.
It uses 1/4 of Local memory and uses all if it is global. */
void LRU(int disk_page_num, int quarter)
{
    int i;
    int min = 999999999;
    int min_index = 0;
    /* Local policy */
    if (strcmp(alloc_policy, "local") == 0)
    {
        int quarter_of_physicals = (physical_mem_page_num / 4);
        int start_index = quarter * quarter_of_physicals;
        for (i = 0; i < quarter_of_physicals; ++i)
        {
            if (physical_page_array[i + start_index].lru_used < min)
            {
                min = physical_page_array[i + start_index].lru_used;
                min_index = i + start_index;
            }
        }
        ++lru_counter;
        if (lru_counter % 1000 == 0)
            reset_lru_time();
        get_page_in_disk(disk_page_num, min_index);
    }
    /* Global policy */
    else if (strcmp(alloc_policy, "global") == 0)
    {
        for (i = 0; i < physical_mem_page_num; ++i)
        {
            if (physical_page_array[i].lru_used < min)
            {
                min = physical_page_array[i].lru_used;
                min_index = i;
            }
        }
        ++lru_counter;
        if (lru_counter % 1000 == 0)
            reset_lru_time();
        get_page_in_disk(disk_page_num, min_index);
    }
    /* Inverted policy */
    else if (strcmp(alloc_policy, "inverted") == 0)
    {
        int oldest_time = 999999999;
        int oldest_page = -1;
        for (i = 0; i < physical_mem_page_num; ++i)
        {
            int holding_page = physical_page_array[i].holding_page;
            if (holding_page >= 0 && virtual_page_array[holding_page].recent_access_time < oldest_time)
            {
                oldest_time = virtual_page_array[holding_page].recent_access_time;
                oldest_page = i;
            }
        }
        if (oldest_page != -1)
        {
            ++lru_counter;
            if (lru_counter % 1000 == 0)
                reset_lru_time();
            get_page_in_disk(disk_page_num, oldest_page);
        }
    }
}

/* It circulates circularly according to the wsclock index and pulls 0 to 0 if reference is 1,
 and replaces that page. If it is local, it uses 1/4 of physical memory. If it is global, it uses the whole memory. */
void WSClock(int disk_page_num, int quarter)
{
    /* Local policy */
    if (strcmp(alloc_policy, "local") == 0)
    {
        int quarter_of_physicals = (physical_mem_page_num / 4);
        int start_index = quarter * quarter_of_physicals;
        if (ws_clock_index[quarter] == 0)
        {
            ws_clock_index[quarter] = start_index;
        }
        while (physical_page_array[ws_clock_index[0]].reference_bit != 0)
        {
            physical_page_array[ws_clock_index[0]].reference_bit = 0;
            ++ws_clock_index[0];
            ws_clock_index[0] %= physical_mem_page_num;
            if (ws_clock_index[quarter] == 0)
            {
                ws_clock_index[quarter] = start_index;
            }
        }
        get_page_in_disk(disk_page_num, ws_clock_index[quarter]);
        ++ws_clock_index[quarter];
        ws_clock_index[quarter] %= (start_index + quarter_of_physicals);
    }
    /* Global policy */
    else if (strcmp(alloc_policy, "global") == 0)
    {
        while (physical_page_array[ws_clock_index[0]].reference_bit != 0)
        {
            physical_page_array[ws_clock_index[0]].reference_bit = 0;
            ++ws_clock_index[0];
            ws_clock_index[0] %= physical_mem_page_num;
        }
        get_page_in_disk(disk_page_num, ws_clock_index[0]);
        ++ws_clock_index[0];
        ws_clock_index[0] %= physical_mem_page_num;
    }
    /* Inverted policy */
    else if (strcmp(alloc_policy, "inverted") == 0)
    {
        int oldest_page = -1;
        int i = ws_clock_index[0];
        while (true)
        {
            if (physical_page_array[i].reference_bit == 0)
            {
                oldest_page = i;
                break;
            }
            else
            {
                physical_page_array[i].reference_bit = 0;
                i = (i + 1) % physical_mem_page_num;
            }
        }
        if (oldest_page != -1)
        {
            ws_clock_index[0] = (oldest_page + 1) % physical_mem_page_num;
            get_page_in_disk(disk_page_num, oldest_page);
        }
    }
}

/* If called with fill tName, it fills the memory and replaces the line given as index on other options. */
void setValue(unsigned int index, int value, char *tName)
{
    ++memory_access;
    /* fills the memory */
    if (strcmp(tName, "fill") == 0)
    {
        fillMemory();
    }
    /* Changes the disks indexth line with value */
    else
    {
        int line_number = virtual_memory[index], page_index;
        FILE *disk = fopen(disk_file_name, "r+");
        pthread_mutex_lock(&mutex);
        if (line_number < 0 || line_number >= num_virtual_int)
        {
            perror("set | index wrong");
            exit(EXIT_FAILURE);
        }
        if ((page_index = is_in_physical_mem(index)) != -1)
        {
            int idx = page_index * frame_size_int;
            int offset = virtual_memory[index] % frame_size_int;
            physical_memory[idx + offset] = value;
            physical_page_array[page_index].recent_access_time = getMicroseconds();
            physical_page_array[page_index].modified_bit = 1;
            virtual_page_array[physical_page_array[page_index].holding_page].recent_access_time = getMicroseconds();
            virtual_page_array[physical_page_array[page_index].holding_page].modified_bit = 1;
        }
        fseek(disk, line_number * 11, SEEK_SET);
        fprintf(disk, "%.10d\n", value);
        pthread_mutex_unlock(&mutex);
        fclose(disk);
    }
    /* prints if the memory address equal to page table print int */
    if (memory_access % page_table_print_int == 0)
    {
        printPageTable();
    }
}

/* It reads and returns the line given as an index directly from the disk. */
int get_direct_disk(unsigned int index)
{
    char line[100];
    FILE *disk = fopen(disk_file_name, "r");
    if ((int)index < 0 || (int)index >= num_virtual_int)
    {
        perror("get_direct_disk | index wrong");
        exit(EXIT_FAILURE);
    }
    fseek(disk, index * 11, SEEK_SET);
    fgets(line, sizeof(line), disk);
    fclose(disk);
    return atoi(line);
}

/* It reads and returns the line given as an virtual_memory[index] directly from the disk. */
int get_disk(unsigned int index)
{
    int line_number = virtual_memory[index];
    char line[100];
    FILE *disk = fopen(disk_file_name, "r");
    if (line_number < 0 || line_number >= num_virtual_int)
    {
        perror("get_disk | index wrong");
        exit(EXIT_FAILURE);
    }
    fseek(disk, line_number * 11, SEEK_SET);
    fgets(line, sizeof(line), disk);
    fclose(disk);
    return atoi(line);
}

/* Finds and returns the address of the given virtual memory index. If the address's values are in physical memory, it returns directly. Otherwise, it copies the page from the disk with the page replacement algorithm and takes the value from there. */
int get(unsigned int index, char *tName)
{
    int ret_val;
    pthread_mutex_lock(&mutex);
    /* if tName is index directly gets the virtual_memory address */
    if (strcmp(tName, "index") == 0)
    {
        ret_val = virtual_memory[index];
        ++statistics[INDEX].reads;
    }
    /* if tName is check directly gets the value without page replacement */
    else if (strcmp(tName, "check") == 0)
    {
        if ((ret_val = get_value_from_physical_mem(index)) == -1)
        {
            ret_val = get_disk(index);
            ++statistics[CHECK].disk_reads;
            ++statistics[CHECK].page_misses;
        }
        else
        {
            ++statistics[CHECK].reads;
        }
    }
    /* if tName is fill directly gets the value without page replacement */
    else if (strcmp(tName, "fill") == 0)
    {
        if ((ret_val = get_value_from_physical_mem(index)) == -1)
        {
            ret_val = get_disk(index);
            ++statistics[FILL].disk_reads;
            ++statistics[FILL].disk_writes;
            ++statistics[FILL].page_misses;
        }
        else
        {
            ++statistics[FILL].reads;
        }
    }
    /* Not hitted */
    else if ((ret_val = get_value_from_physical_mem(index)) == -1)
    {
        /* Page replacement part */
        int disk_page = (int)virtual_memory[index] / (int)frame_size_int;
        if (strcmp(tName, "mult1") == 0)
        {
            ++statistics[MULT1].page_replacement;
            ++statistics[MULT1].page_misses;
            ++statistics[MULT1].disk_reads;
            ++statistics[MULT1].disk_writes;
            statistics[MULT1].reads += 2;
        }
        else if (strcmp(tName, "mult2") == 0)
        {
            ++statistics[MULT2].page_replacement;
            ++statistics[MULT2].page_misses;
            ++statistics[MULT2].disk_reads;
            ++statistics[MULT2].disk_writes;
            statistics[MULT2].reads += 2;
        }
        else if (strcmp(tName, "linearsearch") == 0)
        {
            ++statistics[LINEARSEARCH].page_replacement;
            ++statistics[LINEARSEARCH].page_misses;
            ++statistics[LINEARSEARCH].disk_reads;
            ++statistics[LINEARSEARCH].disk_writes;
            statistics[LINEARSEARCH].reads += 2;
        }
        else if (strcmp(tName, "binarysearch") == 0)
        {
            ++statistics[BINARYSEARCH].page_replacement;
            ++statistics[BINARYSEARCH].page_misses;
            ++statistics[BINARYSEARCH].disk_reads;
            ++statistics[BINARYSEARCH].disk_writes;
            statistics[BINARYSEARCH].reads += 2;
        }

        if (strcmp(page_replacement, "SC") == 0)
        {
            if (strcmp(tName, "mult1") == 0)
            {
                SC(disk_page, 0);
            }
            else if (strcmp(tName, "mult2") == 0)
            {
                SC(disk_page, 1);
            }
            else if (strcmp(tName, "arrsum") == 0)
            {
                SC(disk_page, 2);
            }
            else if (strcmp(tName, "linearsearch") == 0)
            {
                SC(disk_page, 3);
            }
            else if (strcmp(tName, "binarysearch") == 0)
            {
                SC(disk_page, 4);
            }
        }
        else if (strcmp(page_replacement, "LRU") == 0)
        {
            if (strcmp(tName, "mult1") == 0)
            {
                LRU(disk_page, 0);
            }
            else if (strcmp(tName, "mult2") == 0)
            {
                LRU(disk_page, 1);
            }
            else if (strcmp(tName, "arrsum") == 0)
            {
                LRU(disk_page, 2);
            }
            else if (strcmp(tName, "linearsearch") == 0)
            {
                LRU(disk_page, 3);
            }
            else if (strcmp(tName, "binarysearch") == 0)
            {
                LRU(disk_page, 4);
            }
        }
        else if (strcmp(page_replacement, "WSClock") == 0)
        {
            if (strcmp(tName, "mult1") == 0)
            {
                WSClock(disk_page, 0);
            }
            else if (strcmp(tName, "mult2") == 0)
            {
                WSClock(disk_page, 1);
            }
            else if (strcmp(tName, "arrsum") == 0)
            {
                WSClock(disk_page, 2);
            }
            else if (strcmp(tName, "linearsearch") == 0)
            {
                WSClock(disk_page, 3);
            }
            else if (strcmp(tName, "binarysearch") == 0)
            {
                WSClock(disk_page, 4);
            }
        }
        ret_val = get_value_from_physical_mem(index);
    }
    /* Hitted */
    else
    {
        int page_index = is_in_physical_mem(index);
        if (strcmp(tName, "mult1") == 0)
        {
            ++statistics[MULT1].reads;
        }
        else if (strcmp(tName, "mult2") == 0)
        {
            ++statistics[MULT2].reads;
        }
        else if (strcmp(tName, "arrsum") == 0)
        {
            ++statistics[ARRSUM].reads;
        }
        else if (strcmp(tName, "linearsearch") == 0)
        {
            ++statistics[LINEARSEARCH].reads;
        }
        else if (strcmp(tName, "binarysearch") == 0)
        {
            ++statistics[BINARYSEARCH].reads;
        }
        physical_page_array[page_index].recent_access_time = getMicroseconds();
        physical_page_array[page_index].modified_bit = 1;

        virtual_page_array[physical_page_array[page_index].holding_page].recent_access_time = getMicroseconds();
        virtual_page_array[physical_page_array[page_index].holding_page].modified_bit = 1;
    }
    /* if memory_access equals page table print int then prints page table */
    if (memory_access % page_table_print_int == 0)
    {
        printPageTable();
    }
    pthread_mutex_unlock(&mutex);
    return ret_val;
}

/* For page replacement, moves the content of a page on disk to a page in the physical memory. */
void get_page_in_disk(int disk_page_num, int physical_page_num)
{
    int disk_index = frame_size_int * disk_page_num;
    int physical_page_index = frame_size_int * physical_page_num;

    /* Moving values on disk to physical memory */
    for (int i = 0; i < frame_size_int; ++i)
    {
        physical_memory[i + physical_page_index] = get_direct_disk(i + disk_index);
    }

    /* Marks old page's present bit to 0 */
    virtual_page_array[physical_page_array[physical_page_num].holding_page].is_present = 0;

    /* Update page tables */
    physical_page_array[physical_page_num].lru_used = 1;
    physical_page_array[physical_page_num].holding_page = disk_page_num;
    physical_page_array[physical_page_num].modified_bit = 0;
    physical_page_array[physical_page_num].recent_access_time = getMicroseconds();
    physical_page_array[physical_page_num].reference_bit = 1;
    physical_page_array[physical_page_num].is_present = 1;

    virtual_page_array[disk_page_num].lru_used = 1;
    virtual_page_array[disk_page_num].holding_page = disk_page_num;
    virtual_page_array[disk_page_num].modified_bit = 0;
    virtual_page_array[disk_page_num].recent_access_time = getMicroseconds();
    virtual_page_array[disk_page_num].reference_bit = 1;
    virtual_page_array[disk_page_num].is_present = 1;
}

/* Checks whether the index entered is in physical memory. */
int is_in_physical_mem(int index)
{
    int page_number = (int)virtual_memory[index] / (int)frame_size_int, i;
    for (i = 0; i < physical_mem_page_num; ++i)
    {
        if (physical_page_array[i].holding_page == page_number)
            return i;
    }
    return -1;
}

/* If the index parameter exists in physical memory, it returns its value. Returns -1 */
int get_value_from_physical_mem(unsigned int index)
{
    int page_index;
    ++memory_access;
    if ((page_index = is_in_physical_mem(index)) == -1 || virtual_memory[index] < 0)
    {
        printf("Error: Index not found in physical memory or value is negative.\n");
        return -1;
    }
    else
    {
        int idx = page_index * frame_size_int;
        int offset = virtual_memory[index] % frame_size_int;
        return physical_memory[idx + offset];
    }
}

/* frees memory */
void free_memories()
{
    free(virtual_memory);
    free(physical_memory);
    free(physical_page_array);
    free(virtual_page_array);
}

int *mult1(int *virtual_array, int start_index, int size)
{
    int matrix_rows = size / 3; // Number of rows in the matrix
    int matrix_cols = 3;        // Number of columns in the matrix
    int vector_size = 3;        // Size of the vector

    int *result = (int *)malloc(matrix_rows * sizeof(int)); // Allocate memory for the result vector

    // Perform matrix-vector multiplication
    for (int i = 0; i < matrix_rows; i++)
    {
        int sum = 0;

        for (int j = 0; j < matrix_cols; j++)
        {
            int matrix_element = virtual_array[start_index + (i * matrix_cols) + j]; // Access matrix element
            ++statistics[MULT1].disk_reads;
            int vector_element = virtual_array[start_index + (matrix_rows * matrix_cols) + j]; // Access vector element
            ++statistics[MULT1].disk_reads;
            sum += matrix_element * vector_element; // Multiply matrix element with vector element and accumulate the sum
        }

        result[i] = sum; // Store the result in the result vector
    }

    return result; // Return the result vector
}
int *mult2(int *virtual_array, int start_index, int size)
{
    int vector_size = size; // Size of the vectors

    int *result = (int *)malloc(vector_size * sizeof(int)); // Allocate memory for the result vector

    // Perform vector-vector transpose multiplication
    for (int i = 0; i < vector_size; i++)
    {
        int vector_element = virtual_array[start_index + i]; // Access element from the first vector
        ++statistics[MULT2].disk_reads;

        int transpose_element = virtual_array[start_index + vector_size + i]; // Access element from the second vector (transposed)
        ++statistics[MULT2].disk_reads;

        result[i] = vector_element * transpose_element; // Multiply the elements and store the result in the result vector
    }

    return result; // Return the result vector
}

void linear_search(int *virtual_array, int start_index, int size, int target)
{
    int i;
    for (i = start_index; i < start_index + size; i++)
    {
        if (virtual_array[i] == target)
        {
            printf("Target value %d found at index %d\n", target, i);
            return;
        }
    }
    printf("Target value %d not found in the array\n", target);
}

void bubble_sort(int *virtual_array, int start_index, int size)
{
    int i, j;
    for (i = start_index; i < (start_index + size) - 1; i++)
    {
        for (j = start_index; j < (start_index + size) - i - 1; j++)
        {
            if (virtual_array[j] > virtual_array[j + 1])
            {
                int temp = virtual_array[j];
                virtual_array[j] = virtual_array[j + 1];
                virtual_array[j + 1] = temp;
                statistics[BINARYSEARCH].writes += 2;
            }
        }
    }
}

int binary_search(int *virtual_array, int start_index, int end_index, int target)
{
    while (start_index <= end_index)
    {
        int mid = start_index + (end_index - start_index) / 2;

        if (virtual_array[mid] == target)
        {
            return mid; // Target value found at index mid
        }
        else if (virtual_array[mid] < target)
        {
            start_index = mid + 1; // Adjust start_index for right half
        }
        else
        {
            end_index = mid - 1; // Adjust end_index for left half
        }
    }

    return -1; // Target value not found in the array
}

void arr_sum(int *arr)
{

    int sum = 0; // Variable to store the sum

    // Calculate the sum of the array elements
    for (int i = 0; arr[i] != 0; i++)
    {
        sum += arr[i];
    }

    printf("Sum: %d\n", sum); // Print the sum
}

/* Prints statistics */
void print_statistics()
{
    const char *stat_names[] = {
        "*FILL STATISTICS*",
        "*MULT1 STATISTICS*",
        "*MULT2 STATISTICS*",
        "*ARRSUM STATISTICS*",
        "*LINEAR STATISTICS*",
        "*BINARYSEARCH STATISTICS*",
        "*INDEX STATISTICS*",
        "*CHECK STATISTICS*"};

    int i;
    for (i = 0; i < 8; ++i)
    {
        if (i >= sizeof(statistics) / sizeof(statistics[0]))
        {
            printf("Error: Invalid statistics index.\n");
            break;
        }

        printf("\n%s\n", stat_names[i]);
        printf("Number of reads: %d\n", statistics[i].reads);
        printf("Number of writes: %d\n", statistics[i].writes);
        printf("Number of page misses: %d\n", statistics[i].page_misses);
        printf("Number of page replacements: %d\n", statistics[i].page_replacement);
        printf("Number of disk writes: %d\n", statistics[i].disk_writes);
        printf("Number of disk reads: %d\n", statistics[i].disk_reads);
    }

}

/* Thread function, Runs the corresponding sorts by quarter. */
void *thread_func(void *arg)
{
    unsigned long time_arrsum;
    unsigned long time_linear;
    unsigned long time_binary;
    unsigned long temp = getMicroseconds();

    int quarter = *((int *)arg);
    if (quarter == 0)
    {
        temp = getMicroseconds();
        mult1arr = mult1(virtual_memory, 0, num_virtual_int / 4);
        time_mult1 = getMicroseconds() - temp;
    }
    else if (quarter == 1)
    {
        temp = getMicroseconds();
        mult2arr = mult2(virtual_memory, 0, num_virtual_int / 4);
        time_mult2 = getMicroseconds() - temp;

    }
    else if (quarter == 2)
    {
        temp = getMicroseconds();
        bubble_sort(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1);

        int search_result = binary_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 1745795202);
        search_result == -1 ? printf("5 is not in the this part of the virtual memory. binary search\n") : printf("1745795202 is in the this part of the virtual memory.");

        search_result = 0;
        search_result = binary_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 7);
        search_result == -1 ? printf("7 is not in the this part of the virtual memory. binary search\n") : printf("5 is in the this part of the virtual memory.");

        search_result = 0;
        search_result = binary_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 247538188);
        search_result == -1 ? printf("9 is not in the this part of the virtual memory. binary search\n") : printf("247538188 is in the this part of the virtual memory.");

        search_result = 0;
        search_result = binary_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 11);
        search_result == -1 ? printf("11 is not in the this part of the virtual memory. binary search\n") : printf("5 is in the this part of the virtual memory.");

        search_result = 0;
        search_result = binary_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 13);
        search_result == -1 ? printf("13 is not in the this part of the virtual memory. binary search\n") : printf("5 is in the this part of the virtual memory.");

        time_binary = getMicroseconds() - temp;
        temp = getMicroseconds();

        linear_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 1745795202);
        linear_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 7);
        linear_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 247538188);
        linear_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 11);
        linear_search(virtual_memory, num_virtual_int / 2, ((num_virtual_int / 4) * 3) - 1, 13);
        time_linear = getMicroseconds() - temp;

    }
    else if (quarter == 3)
    {
        temp = getMicroseconds();
        printf("\nMultiplication 1 result: ");
        arr_sum(mult1arr);
        printf("\nMultiplication 2 result: ");
        arr_sum(mult2arr);
        time_arrsum = getMicroseconds() - temp;

    }
    return NULL;
}
