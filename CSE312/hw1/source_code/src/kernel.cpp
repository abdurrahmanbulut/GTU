
#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <syscalls.h>
#include <random.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>
#include <time.h>


// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

myos::TaskManager* taskManagerPtr;
uint32_t seed = 1;

void printf(char* str);
void printfHex(uint8_t key);
char* int_to_str(int num);
class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};
class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};

/*
*************************************************************************
*/
int fork() {

    Task* currentTask = taskManagerPtr->GetCurrentTask();
    Task* newTask = taskManagerPtr->fork();
    
    if (currentTask->get_pid() == newTask->get_pid()) {
        return 0; // Child process returns 0
    } else {
        return newTask->get_pid(); // Parent process returns the child's PID
    }
}
int waitpid( int pid, int* status, int options) {
    return taskManagerPtr->WaitForProcess(pid, status, options);
}
int getpid() {
    return taskManagerPtr->GetCurrentTask()->get_pid();
}

int getrandom(int max_value){
    // seed initialized as global like this: uint32_t seed = 1;
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % max_value;

}
int execve(const char *name, char *const argv[], char *const envp[]) {
    //return load_new_process(name, argv, envp, taskManagerPtr); I don't know to implement image thing. 
    return 0;
}


/*
*************************************************************************
*/
/*
*************************************************************************
*/
void sysprintf(char* str)
{
    asm("int $0x80" : : "a" (4), "b" (str)); 
}

int sysfork(){
    int result;
    asm("int $0x80" : "=a" (result) : "a" (2));
    return result;
}
int sysgetpid(){
    int result;
    asm volatile("int $0x80" : "=a" (result) : "a" (20));
    return result;
}
int syswaitpid(int pid, int* status, int options) {
    int result;
    asm("int $0x80" : "=a" (result) : "a" (7), "b" (pid), "c" (status), "d" (options));
    return result;     
}
int sysexecve(const char *name, char *const argv[], char *const envp[]){
    int result;
    asm volatile("int $0x80" : "=a" (result) : "a" (11), "b" (name), "c" (argv), "d" (envp));
    return result;
}
int sysgetrandom(int max_value) {
    int result;
    asm("int $0x80" : "=a" (result) : "a" (7), "b" (max_value));
    return result;  
    
}
/*
*************************************************************************
*/
/**
 * Strategies
*/


void BinarySearch();
void LinearSearch();
void Collatz();

void strategy1(GlobalDescriptorTable &gdt, TaskManager &taskManager) {
    Task task1(&gdt, BinarySearch);
    Task task2(&gdt, LinearSearch);
    Task task3(&gdt, Collatz);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    taskManager.AddTask(&task3);
}

void strategy2(GlobalDescriptorTable &gdt, TaskManager &taskManager) {
    int chosenProgram = sysgetrandom(3) % 3;
    for (int i = 0; i < 10; ++i) {
        if (chosenProgram == 0) {
            Task task(&gdt, BinarySearch);
            taskManager.AddTask(&task);
        } else if (chosenProgram == 1) {
            Task task(&gdt, LinearSearch);
            taskManager.AddTask(&task);
        } else {
            Task task(&gdt, Collatz);
            taskManager.AddTask(&task);
        }
    }
}

void strategy3(GlobalDescriptorTable &gdt, TaskManager &taskManager) {
    int program1 = sysgetrandom(3) % 3;
    int program2;
    do {
        program2 = sysgetrandom(3) % 3;
    } while (program2 == program1);

    for (int i = 0; i < 3; ++i) {
        if (program1 == 0 || program2 == 0) {
            Task task(&gdt, BinarySearch);
            taskManager.AddTask(&task);
        }
        if (program1 == 1 || program2 == 1) {
            Task task(&gdt, LinearSearch);
            taskManager.AddTask(&task);
        }
        if (program1 == 2 || program2 == 2) {
            Task task(&gdt, Collatz);
            taskManager.AddTask(&task);
        }
    }
}

/**
 * Strategies
*/
void taskA()
{
    while(true){
        int pid1 = sysfork();
        sysprintf("gtu, PID: ");
        sysprintf(int_to_str(pid1));
        int pid = sysgetpid(); // yukarıdaki interrupt ı çağırıyor. Hardware e erişim kernel tarafından yapılıyor. 
        sysprintf("gtu, PID: ");
        sysprintf(int_to_str(pid));
    }
}   
void taskB()
{
    while(true)
        sysprintf("os");
}
void taskC()
{
    while(true)
        sysprintf("hw1");
}
void taskD(){
    while (true)
    {
       //
    }
    
    printf("hello\n");
}
void taskE(){
    while (true)
    {
        int a = 4;
        syswaitpid(1, &a, 1);
    }
}
void taskF(){
    while (true)
    {
        int a = 2;
        int random_number = sysgetrandom(a);
        printf(int_to_str(random_number));
    }

}

void taskG()
{
    int arr[] = {10, 20, 80, 30, 60, 50, 110, 100, 130, 170};
    int n = sizeof(arr) / sizeof(arr[0]);
    int x = 175;

    //char *result = linearSearch(arr, n, x);
    while (true)
        sysprintf("2");
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}


extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{

    printf("Hello World! --- http://www.AlgorithMan.de\n");
    

    GlobalDescriptorTable gdt;
    
    TaskManager taskManager;
    taskManagerPtr = &taskManager;

    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    Task task3(&gdt, taskC);
    Task task4(&gdt, taskD);
    Task task5(&gdt, taskE);
    Task task6(&gdt, taskF);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    taskManager.AddTask(&task3);
    taskManager.AddTask(&task4);
    taskManager.AddTask(&task5);
    taskManager.AddTask(&task6);


    // printf("Task 1 properties:\n")
    // printf("deneme1");
    // taskManager.printTaskProperties(&task1);

    // printf("Task 2 properties:\n");
    // taskManager.printTaskProperties(&task2);
    // printf("deneme2");

    // printf("Task 3 properties:\n");
    // taskManager.printTaskProperties(&task3);


    int chosenStrategy = 0;

    if (chosenStrategy == 0) {
        strategy1(gdt, taskManager);
    } else if (chosenStrategy == 1) {
        strategy2(gdt, taskManager);
    } else {
        strategy3(gdt, taskManager);
    }
    
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    SyscallHandler syscalls(&interrupts, 0x80); // software interrupt  added



    printf("Initializing Hardware, Stage 1\n");
    
    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    
    DriverManager drvManager;
    
        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);
        
    
        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);
        
        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        VideoGraphicsArray vga;
        
    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();
        
    printf("Initializing Hardware, Stage 3\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif


    interrupts.Activate();  
    
    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}

int binarySearch(int arr[], int size, int x) {
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == x)
            return mid;

        if (arr[mid] < x)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

void BinarySearch() {
    while(true){
        int arr[] = {10, 20, 80, 30, 60, 50, 110, 100, 130, 170};
        int n = sizeof(arr) / sizeof(arr[0]);
        int x = 110;
        int result = binarySearch(arr, n, x);
        sysprintf(int_to_str(result));
    }
    
}

int linearSearch(int arr[], int size, int x) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == x)
            return i;
    }
    return -1;
}

void LinearSearch() {
     while(true){
        int arr[] = {10, 20, 80, 30, 60, 50, 110, 100, 130, 170};
        int n = sizeof(arr) / sizeof(arr[0]);
        int x = 175;
        int result = linearSearch(arr, n, x);
        sysprintf(int_to_str(result));
     }    
}

void Collatz() {
while(true){
    for (int n = 1; n <= 25; n++) {

        while (n != 1) {
            sysprintf(int_to_str(n));
            sysprintf(": ");
            if (n % 2 == 0)
                n = n / 2;
            else
                n = 3 * n + 1;
        }
        sysprintf(int_to_str(n));
        sysprintf("\n");
    }
}
}