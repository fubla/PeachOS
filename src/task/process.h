#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "config.h"
#include "task.h"

struct process
{
    // Process id
    uint16_t pid;
    char filename[PEACHOS_MAX_PATH];

    // Main process task (thread?)
    struct task* task;

    // Keep track of all allocations (malloc) made by the process so we can clean up and prevent leaks
    void* allocations[PEACHOS_MAX_PROGRAM_ALLOCATIONS];

    // Pointer to process memory (physical)
    void* ptr;

    // Pointer to process stack memory (physical)
    void* stack;

    // Size of data pointed to by "ptr"
    uint32_t size;
};

int process_load_for_slot(const char* filename, struct process** process, int process_slot);

#endif