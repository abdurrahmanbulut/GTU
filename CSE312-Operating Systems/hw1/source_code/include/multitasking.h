
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>


#define MAX_PROCESSES 256

namespace myos
{

    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;
    } __attribute__((packed));

    enum ProcessState
    {
        READY,
        RUNNING,
        BLOCKED,
        TERMINATED
    };

    class Task
    {
        friend class TaskManager;

    private:
        common::uint8_t stack[4096]; // 4 KiB
        char name[25];           
        int pid;
        int ppid;
        int pc; 
        int sp; 
        int priority;               
        int memoryUsage;
        int memory_alloc;
        int open_files[10];
        CPUState *cpustate;
        ProcessState state;         
        Task *children;
    public:
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
        int get_pid() const
        {
            return this->pid;
        }
        void set_pid(int pid)
        {
            this->pid = pid;
        }
        void* GetEntryPoint() {
            return (void*)cpustate->eip;
        }
        int get_ppid() const
        {
            return ppid;;
        }
        void set_ppid(int ppid)
        {
            this->ppid = ppid;
        }
        int get_state() const
        {
            return state;
        }
        void set_state(ProcessState state)
        {
            this->state = state;
        }
        int get_pc() const
        {
            return this->pc;
        }
        int get_sp() const
        {
            return this->sp;
        }
        void set_pc(int pc)
        {
            this->pc = pc;
        }
        void set_sp(int sp)
        {
            this->sp = sp;
        }
        int get_memory_alloc() const
        {
            return this->memory_alloc;
        }
        int get_file_status(int fd) const
        {
            if (fd >= 0 && fd < 10) // file descriptor is within a valid range of values.
            {
                return this->open_files[fd]; 
            }
            return -1;
        }
        void set_memory_alloc(int memory_alloc)
        {
            this->memory_alloc = memory_alloc;
        }
        void set_file_status(int fd, int status)
        {
            if (fd >= 0 && fd < 10)
            {
                this->open_files[fd] = status;
            }
        }
        const char *get_name() const
        {
            return name;
        }
        int get_priority() const
        {
            return priority;
        }
        void set_priority(int priority)
        {
            this->priority = priority;
        }
        ProcessState getState() const
        {
            return state;
        }
        void setState(ProcessState state)
        {
            this->state = state;
        }
        CPUState *get_CPUState() const
        {
            return cpustate;
        }
    };

    class TaskManager
    {

    private:
        Task *tasks[256];
        int numTasks;
        int currentTask;
        GlobalDescriptorTable *gdt;
        
    public:
        TaskManager();
        bool AddTask(Task* task);
        Task* copyTask(Task* source);
        Task* fork();
        int WaitForProcess(int pid, int* status, int options);
        ~TaskManager();
        Task* get_process_by_pid(int pid);
        bool update_process_state(int pid, ProcessState state);
        Task *GetCurrentTask();
        int next_pid(){
            return this->numTasks+1;
        }
        void SwitchTask();
        CPUState *Schedule(CPUState *cpustate);
        void printTaskProperties(Task* task);
        void printProcessTable();
    };
}

#endif