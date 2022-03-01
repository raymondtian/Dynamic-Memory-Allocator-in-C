#include <stddef.h>
#include <stdint.h>

typedef struct Data_Structure {
    uint8_t initial_size;
    uint8_t min_size;
    unsigned char * free_blocks;
    unsigned char * allocatable_heap;
    unsigned char * heap;
} data_structure;

void init_allocator(void * heapstart, uint8_t initial_size, uint8_t min_size);

void * virtual_malloc(void * heapstart, uint32_t size);

int virtual_free(void * heapstart, void * ptr);

void * virtual_realloc(void * heapstart, void * ptr, uint32_t size);

void virtual_info(void * heapstart);
