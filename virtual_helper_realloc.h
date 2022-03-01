#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

//Function to check if the block is allocated or not
int is_allocated(int index, unsigned char * free_block_list) {
    unsigned char * ptr = free_block_list;

    if (ptr[index] == 1) {
        return 1; //block is free
    }
    return 0; //is allocated
}

//Function to find the minimum value when comparing 2 integers
int compare(int x, int y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

//Recursive function to find allocated block
int find_allocated_block(int index, int current_heap_address, int max_heap_address,
int location, int min_size, unsigned char * free_block_list) {
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
    //checking if block is free or not
    int is_alloc_value = is_allocated(index, ptr);
    // initialising our value for recursive call
    int value = 0;

    if (location < min_size) {  
        return -1;
    }

    while (is_alloc_value != 1) {
        //block is allocated
        if (ptr[index] == 0) {
            if (current_heap_address == max_heap_address) {
                //return its location
                return location;
            }
        }
        //recursively calling the function
        value = find_allocated_block(previous_index, previous_heap_address, max_heap_address, location - 1, min_size, ptr);

        //if value returned from the function is -1 (ie: free block)
        if (value == -1) {
            return find_allocated_block(next_index, next_heap_address, max_heap_address, location - 1, min_size, ptr);;
        }
        else {
            return value;
        }
        is_alloc_value = 1;
    }
    return -1;
}

void re_alloc(int index, int current_heap_address, int max_heap_address, 
int location, int allocation_size, int min_size, unsigned char * free_block_list) {

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

    if (location < min_size) {
        return;
    }

    if (ptr[index] == 0) {
        return;
    }

    if (current_size == (1 << allocation_size)) {
        if (current_heap_address == max_heap_address) {
            ptr[index] = 0;
            return;
        }
    }
    if (current_heap_address + current_size > max_heap_address) {
        if (current_heap_address <= max_heap_address) {
            if (current_size > (1 << allocation_size)) {
                ptr[index] = 4;
                re_alloc(previous_index, previous_heap_address, max_heap_address, location - 1, allocation_size, min_size, ptr);
                re_alloc(next_index, next_heap_address, max_heap_address, location - 1, allocation_size, min_size, ptr);
            }
        }
    }
}
