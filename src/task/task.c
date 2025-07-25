#include "task.h"
#include "kernel.h"
#include "status.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "process.h"

// The current task that is running
struct task* current_task = NULL;

// Task linked list
struct task* task_tail = NULL;
struct task* task_head = NULL;

static int task_init(struct task* task, struct process* process)
{
    memset(task, 0, sizeof(struct task));
    // Map the entire 4GB address space to itself
    task->page_directory = paging_new_4gb(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    if(!task->page_directory)
    {
        return -ENOMEM;
    }

    task->registers.eip = PEACHOS_PROGRAM_VIRTUAL_ADDRESS;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.esp = PEACHOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START;
    task->process = process;

    return 0;
}

static void task_list_remove(struct task* task)
{
    if(task->prev)
    {
        task->prev->next = task->next;
    }

    if(task == task_head)
    {
        task_head = task->next;
    }

    if(task == task_tail)
    {
        task_tail = task->prev;
    }

    if(task == current_task)
    {
        current_task = task_get_next();
    }
}

struct task* task_current()
{
    return current_task;
}

struct task* task_new(struct process* process)
{
    int res = 0;
    struct task* task = kzalloc(sizeof(struct task));
    if(!task)
    {
        res = -ENOMEM;
        goto out;
    }

    res = task_init(task, process);
    if(res != PEACHOS_ALL_OK)
    {
        goto out;
    }

    if(!task_head)
    {
        task_head = task;
        task_tail = task;
        goto out;
    }

    task_tail->next = task;
    task->prev = task_tail;
    task_tail = task;

out:
    if(ISERR(res))
    {
        task_free(task);
        return ERROR(res);
    }
    
    return task;;
}

struct task* task_get_next()
{
    if(!current_task->next)
    {
        return task_head;
    }

    return current_task->next;
}

int task_free(struct task* task)
{
    paging_free_4gb(task->page_directory);
    task_list_remove(task);

    // Finally free the task data
    kfree(task);

    return 0;
}
