#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

//Function to check if the block is free or not
int is_free(int index, unsigned char * free_block_list) {
    unsigned char * ptr = free_block_list;

    if (ptr[index] == 0) {
        return 0; //block has been allocated
    }
    return 1; //is free
}

//Recursive function to find the next free block
int find_free_block(int index, int current_heap_address, int location, 
int allocation_size, int min_size, unsigned char * free_block_list) {

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
    int is_free_value = is_free(index, ptr);
    //initialising our value for recursive call
    int value = 0;

    while (is_free_value != 0) {
        //if block is partially allocated
        if (ptr[index] == 4) {
            if ((1 << allocation_size) < current_size) {
                if (ptr[previous_index] == 4) {
                    value = find_free_block(previous_index, previous_heap_address, location - 1, allocation_size, min_size, ptr);
                    if (value != -1) {
                        return value;
                    }
                }
                if (ptr[next_index] == 4) {
                    value = find_free_block(next_index, next_heap_address, location - 1, allocation_size, min_size, ptr);
                    if (value != -1) {
                        return value;
                    }
                }
            }
        }
        // if block is free
        if (ptr[index] == 1) {
            if ((1 << allocation_size) == current_size) {
                //now it is not empty and it has been allocated
                ptr[index] = 0;
                //return its address
                return previous_heap_address;
            }
        }
        //recursively calling the function
        value = find_free_block(previous_index, previous_heap_address, location - 1, allocation_size, min_size, ptr);

        //if value returned from the function is -1 (ie: allocated block)
        if (value == -1) {
            return find_free_block(next_index, next_heap_address, location - 1, allocation_size, min_size, ptr);
        } else {
            return value;
        }
        is_free_value = 0;
    }
    return -1;
}

//Function to merge blocks
void merge1(int index, int current_heap_address, int max_heap_address, 
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
    //checking if block is free or not
    int is_free_value = is_free(index, ptr);
    
    //while ptr points to a block that's free
    while (is_free_value != 0) {
        //if block is free
        if (ptr[index] == 1) {
            if (current_size > (1 << allocation_size)) {
                if (current_heap_address + current_size >= max_heap_address && current_heap_address <= max_heap_address) {
                    ptr[index] = 4; //block is now partially allocated
                    merge1(previous_index, previous_heap_address, max_heap_address, location - 1, allocation_size, min_size, ptr);
                    merge1(next_index, next_heap_address, max_heap_address, location - 1, allocation_size, min_size, ptr);
                }
            }
        }
        //if block is partially allocated
        if (ptr[index] == 4) {
            if (current_size > (1 << allocation_size)) {
                if (current_heap_address + current_size >= max_heap_address && current_heap_address <= max_heap_address) {
                    merge1(next_index, next_heap_address, max_heap_address, location - 1, allocation_size, min_size, ptr);
                    merge1(previous_index, previous_heap_address, max_heap_address, location - 1, allocation_size, min_size, ptr);            
                }
            }
        }
        is_free_value = 0;
    }
    return;
}