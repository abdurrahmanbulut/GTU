
#include <syscalls.h>
 
using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;
 
SyscallHandler::SyscallHandler(InterruptManager* interruptManager, uint8_t InterruptNumber)
:    InterruptHandler(interruptManager, InterruptNumber  + interruptManager->HardwareInterruptOffset())
{
}

SyscallHandler::~SyscallHandler()
{
}
int getpid();
int fork();
void printf(char*);
void waitpid(int pid, int* status, int options);
int execve(const char *name, char *const argv[], char *const envp[]);
int getpid();
int getrandom(int);

// Round Robin scheduling variables
int currentProcessIndex = 0;
uint32_t timeSliceCounter = 0;


uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    CPUState* cpu = (CPUState*)esp;
    

    switch(cpu->eax)
    {
        case 2:
             cpu->eax = fork();
             break;

        case 4:
            printf((char*)cpu->ebx);
            break;
        case 7:
            waitpid((int)cpu->ebx, (int*)cpu->ecx, cpu->edx);
            break;
        case 11: 
            cpu->eax = execve((const char*)cpu->ebx, (char *const *)cpu->ecx, (char *const *)cpu->edx);
            break;
        case 20:
            cpu->eax = getpid();
            break;
        case 318:
            cpu->eax = getrandom((int)cpu->ebx);
        break;
        
        default:
            break;
    }

    
    return esp;
}


