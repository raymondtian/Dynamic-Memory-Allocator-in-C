#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void helper(int index, int current_heap_address, int location, int min_size, unsigned char * free_block_list) {
    //size of the current block
    int current_size = (1 << location);
    //address of block on the right of current block
    int next_heap_address = current_heap_address + (current_size / 2);
    //address of block on the left of current block
    int previous_heap_address = current_heap_address;
    //index of block on the right of current block
    int next_index = 2 * (index + 1);
    //index of block on the left of current block
    int previous_index = 2 * (index + 1) - 1;
    //list of free blocks
    unsigned char * ptr = free_block_list;

    //if block is free
    if (ptr[index] == 1) {
        printf("free %d\n", current_size);
    }

    //if block isn't free
    if (ptr[index] == 0) {
        printf("allocated %d\n", current_size);
    }

    //if block is partially allocated
    if (ptr[index] == 4) {
        helper(previous_index, previous_heap_address, location - 1, min_size, ptr);
        helper(next_index, next_heap_address, location - 1, min_size, ptr);
    }
    return;
}
