#include "heap.h"
#include "status.h"
#include "kernel.h"
#include "memory/memory.h"

static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    int res = 0;
    size_t table_size =  (size_t)(end - ptr);
    size_t total_blocks = table_size / PEACHOS_HEAP_BLOCK_SIZE;
    if (table->total != total_blocks)
    {
        res = -EINVARG;
        goto out;
    }
out:
    return res;
}

static int heap_validate_alignment(void* ptr)
{
    return ((unsigned int)ptr % PEACHOS_HEAP_BLOCK_SIZE) == 0;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{
    int res = 0;
    if (!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
    {
        res = -EINVARG;
        goto out;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    res = heap_validate_table(ptr, end, table);
    if (res < 0) 
    {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);


out: 
    return res;
}

static uint32_t heap_align_value_to_upper(uint32_t val)
{
    if ((val % PEACHOS_HEAP_BLOCK_SIZE) == 0)
    {
        return val;
    }

    val = (val - (val % PEACHOS_HEAP_BLOCK_SIZE)) + PEACHOS_HEAP_BLOCK_SIZE;
    return val;
}

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
    return entry & 0x0f;
}
int heap_get_start_block(struct heap* heap, uint32_t total_blocks)
{
    struct heap_table* table = heap->table;
    int bc = 0;     // current block
    int bs = -1;    // first block that is free
    
    for (int i = 0; i < table->total; i++)
    {
        if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            bc = 0;
            bs = -1;
            continue;
        }

        // If this is the first block
        if (bs == -1) {
            bs = i;
        }
        bc++;

        // If we have the found the requested amount of blocks 
        if (bc == total_blocks)
        {
            break;
        }
    } 

    if (bs == -1)
    {
        return -ENOMEM;
    }

    return bs;
}

void* heap_block_to_address(struct heap* heap, uint32_t block)
{
    return heap->saddr + block * PEACHOS_HEAP_BLOCK_SIZE;
}

int heap_address_to_block(struct heap* heap, void* address)
{
    return ((uint32_t)(address - heap->saddr)) / PEACHOS_HEAP_BLOCK_SIZE;
}

void heap_mark_blocks_taken(struct heap* heap, uint32_t start_block, uint32_t total_blocks)
{
    int end_block = start_block + total_blocks - 1;
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if (total_blocks > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    for (uint32_t i = start_block; i <= end_block; i++)
    {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if(i != end_block - 1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

void heap_mark_blocks_free(struct heap* heap, int start_block)
{
    struct heap_table* table = heap->table;
    for (int i = start_block; i < table->total; i++)
    {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if (!(entry & HEAP_BLOCK_HAS_NEXT))
        {
            break;
        }
    }
}

void* heap_malloc_blocks(struct heap* heap, uint32_t total_blocks)
{
    void* address = NULL;
    int start_block =  heap_get_start_block(heap, total_blocks);
    if (start_block < 0)
    {
        goto out;
    }

    address = heap_block_to_address(heap, start_block);

    // Mark blocks as taken
    heap_mark_blocks_taken(heap, start_block, total_blocks);
out:
    return address;
}

void* heap_malloc(struct heap* heap, size_t size)
{
    size_t aligned_size = heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / PEACHOS_HEAP_BLOCK_SIZE;
    return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap* heap, void* ptr)
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
}