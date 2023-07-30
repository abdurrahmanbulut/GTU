
#include <multitasking.h>

using namespace myos;
using namespace myos::common;


void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}
void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

char* int_to_str(int num);
Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState *)(stack + 4096 - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */

    // cpustate -> error = 0;

    // cpustate->esp = ;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate->ss =;
    cpustate -> eflags = 0x202;
}


Task::~Task()
{
    
}

/**************************************************
                    TASK MANAGER
**************************************************/

        
TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
  
}


bool TaskManager::AddTask(Task *task)
{
    if (numTasks < 256) {
        tasks[numTasks] = task;
        tasks[numTasks]->pid = numTasks+1;
        tasks[numTasks]->state = READY;
        tasks[numTasks]->memory_alloc = 4096;

        for (int j = 0; j < 10; j++) {
            tasks[numTasks]->open_files[j] = 0;
        }
        numTasks++;
        return true;
    }
    return false;
}
Task* TaskManager::copyTask(Task* source) {
    
    Task* newTask = source;

    // Copy the source task's stack and CPU state to the new task
    for (unsigned int i = 0; i < sizeof(source->stack); i++) {
        newTask->stack[i] = source->stack[i];
    }

    newTask->cpustate = source->cpustate;

    
    newTask->set_pid(next_pid());

    
    newTask->set_ppid(source->get_pid());

    for (unsigned int i = 0; i < sizeof(source->name); i++) {
        newTask->name[i] = source->name[i];
    }

    newTask->set_priority(source->get_priority());
    newTask->set_memory_alloc(source->get_memory_alloc());
    for (unsigned int i = 0; i < sizeof(source->open_files) / sizeof(source->open_files[0]); i++) {
        newTask->open_files[i] = source->open_files[i];
    }

    return newTask;
}

Task* TaskManager::fork() {
    Task* currentTsk = tasks[currentTask];
    Task* newTask = copyTask(currentTsk);
    AddTask(newTask);
    return newTask;
}
int TaskManager::WaitForProcess(int pid, int* status, int options) {
    
    Task* target = get_process_by_pid(pid);
    if (!target) {
        return -1; 
    }

    Task* currentTask = GetCurrentTask();

    // Suspend the current task until the target process has completed
    while (target->getState() != TERMINATED) {
        asm("int $0x20"); // This will trigger a timer interrupt, which we will call TaskManager::Schedule
    }

    if (status) {
        *status = 0; // successful completion
    }

    // Return the PID of the terminated process
    return pid;
}
Task* TaskManager::get_process_by_pid(int pid) {
    for (int i = 0; i < numTasks; i++) {
        if (tasks[i]->pid == pid) {
            return tasks[i];
        }
    }
}

Task *TaskManager::GetCurrentTask()
{
    if (currentTask >= 0 && currentTask < numTasks)
        return tasks[currentTask];
    
}
// CPUState *TaskManager::Schedule(CPUState *cpustate)
// {
//     if (numTasks <= 0)
//         return cpustate;

//     if (currentTask >= 0)
//         tasks[currentTask]->cpustate = cpustate;

//     if (++currentTask >= numTasks)
//         currentTask %= numTasks;

//     // Print process table information when a context switch occurs
//     printProcessTable();
    
//     return tasks[currentTask]->cpustate;
// }
// Round Robin
CPUState *TaskManager::Schedule(CPUState *oldstate)
{
    if (numTasks <= 1) {
        return oldstate;
    }

    tasks[currentTask]->cpustate = oldstate;

    currentTask = (currentTask + 1) % numTasks;
    while (tasks[currentTask]->state != READY) {
        currentTask = (currentTask + 1) % numTasks;
    }
    // Print process table information when a context switch occurs
    printProcessTable();

    return tasks[currentTask]->cpustate;
}

bool TaskManager::update_process_state(int pid, ProcessState state){
    for (int i = 0; i < numTasks; i++) {
        if (tasks[i]->pid == pid) {
            tasks[i]->state = state;
            return true;
        }
    }
    return false;
}
void TaskManager::printProcessTable()
{
    printf("Process table:\n");
    for (int i = 0; i < numTasks; ++i) {
        Task *task = tasks[i];

        char task_number[2];
        task_number[0] = i + '0';
        task_number[1] = '\0';

        printf("Task ");
        printf(task_number);
        printf(": [State: ");

        if (i == currentTask) {
            printf("Running");
        } else {
            printf("Ready");
        }
        printf(", EIP: 0x");
        printfHex((task->cpustate->eip >> 4) & 0xFF);
        printfHex(task->cpustate->eip & 0xFF);
        printf(", ESP: 0x");
        printfHex((task->cpustate->esp >> 4) & 0xFF);
        printfHex(task->cpustate->esp & 0xFF);

        printf("]\n");
        printTaskProperties(tasks[i]);
    }
}


void TaskManager::printTaskProperties(Task* task) {
    printf("Task ID: ");
    printfHex(task->pid);
    printf("\nTask Name: ");
    printf(task->name);
    printf("\nTask PC: ");
    printfHex(task->pc);
    printf("\nTask SP: ");
    printfHex(task->sp);
    printf("\nTask Memory Allocation: ");
    printfHex(task->memory_alloc);
    printf("\nTask Open Files: ");
    for (int i = 0; i < 10; i++) {
        printfHex(task->open_files[i]);
        if (i != 9) printf(", ");
    }
    printf("\nTask Priority: ");
    printfHex(task->priority);
    printf("\nTask State: ");
    printfHex((uint8_t)task->state);
    printf("\nTask CPUState: ");
    printfHex((uint32_t)task->cpustate);
    printf("\nTask Memory Usage: ");
    printfHex(task->memoryUsage);
    printf("\n\n");
}
void TaskManager::SwitchTask()
{
    // If there is only one task, nothing to switch
    if (numTasks <= 1) {
        return;
    }

    CPUState* old_state = tasks[currentTask]->get_CPUState();

    // Calculate the next task to switch to using Round Robin scheduling
    int nextTask = (currentTask + 1) % numTasks;
    while (tasks[nextTask]->getState() != READY) {
        nextTask = (nextTask + 1) % numTasks;
    }

    currentTask = nextTask;

    printProcessTable();

    // Set the new task's CPU state and switch to it
    CPUState* new_state = (CPUState*)tasks[currentTask]->get_CPUState();

}


char* int_to_str(int num) {
    static char str[20];
    int i = 0;
    int is_negative = 0;

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    do {
        str[i++] = '0' + num % 10;
        num /= 10;
    } while (num != 0);

    if (is_negative) {
        str[i++] = '-';
    }

    // Reverse the string to get correct order of digits
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    str[i] = '\0';

    return str;
}
