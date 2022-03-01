#include "virtual_alloc.h"
#include "virtual_sbrk.h"
#include "virtual_helper_malloc.h"
#include "virtual_helper_free.h"
#include "virtual_helper_realloc.h"
#include "virtual_helper_print.h"
#include <stdio.h>
#include <math.h>

/* assigned blocks labelled 0, free blocks labelled 1,
* partially assigned blocks labelled 4 (4 for death from COMP2017)
*/

void init_allocator(void * heapstart, uint8_t initial_size, uint8_t min_size) {
    data_structure ds;
    ds.heap = heapstart;
   
    //calculates size for list of free blocks
    uint8_t free_blocks_size = initial_size - min_size + 1;

    //sbrk's space for min size and initial size, list of free blocks, and size for your initial block to be malloc'd
    virtual_sbrk(2 + (1 << free_blocks_size) - 1 + (1 << initial_size)); 

    //assigning index 0 of heap array to min_size, and index 1 of heap array to initial size
    ds.heap[0] = min_size;
    ds.heap[1] = initial_size;  

    //labelling all free blocks to 1
    for (int i = 0; i < (1 << free_blocks_size) - 1; i ++) {
        ds.heap[2 + i] = 1;
    }

    //assigns starting index of array to be manipulated to 0.
    for (int i = 0; i < (1 << initial_size); i ++) {
        ds.heap[2 + (1 << free_blocks_size) - 1 + i] = 0;
    }
}

void * virtual_malloc(void * heapstart, uint32_t size) {
    if (size == 0) {
        return NULL;
    } else {
        data_structure ds;
        ds.heap = heapstart;
        int current_address = 0;
        int allocation_size = 0;
        int power_allocation_size = (1 << allocation_size);
        while (power_allocation_size < size) {
            allocation_size ++;
            power_allocation_size = (1 << allocation_size);
        }

        //assigning index 0 of heap array to min_size, and index 1 of heap array to initial size
        unsigned char temp_min_size = ds.heap[0];
        unsigned char temp_initial_size = ds.heap[1];

        if (allocation_size < temp_min_size) {
            allocation_size = temp_min_size;
        }

        //calculates size for list of free blocks
        uint8_t free_blocks_size = temp_initial_size - temp_min_size + 1;

        //assigning index 2 of heap array to list of free blocks, and the correct index of heap array to allocatable space
        ds.free_blocks = &ds.heap[2];
        ds.allocatable_heap = &ds.heap[2 + ((1 << (free_blocks_size)))];

        int value = find_free_block(0, current_address, temp_initial_size, allocation_size, temp_min_size, ds.free_blocks);

        if (value == -1) {
            return NULL;
        } else {
            merge1(0, 0, value, temp_initial_size, allocation_size, temp_min_size, ds.free_blocks);
            return &ds.allocatable_heap[value];
        }
    }
}

int virtual_free(void * heapstart, void * ptr) {
    data_structure ds;
    ds.heap = heapstart;

    unsigned char * ptr_free = ptr;
    
    unsigned char temp_min_size = ds.heap[0];
    unsigned char temp_initial_size = ds.heap[1];

    //calculates size for list of free blocks
    uint8_t free_blocks_size = temp_initial_size - temp_min_size + 1;

    //assigning index 2 of heap array to list of free blocks, and the correct index of heap array to allocatable space
    ds.free_blocks = &ds.heap[2];
    ds.allocatable_heap = &ds.heap[2 + ((1 << (free_blocks_size)))];
    int current_address = ptr_free - ds.allocatable_heap;
    int value = free_allocated_block(0, 0, current_address, temp_initial_size, temp_min_size, ds.free_blocks);
    int allocation_size = temp_initial_size - value;

    if (value == -1) {
        return 1;
    } else {
        for (int i = 0; i < value; i ++) {
            merge2(0, 0, current_address, temp_initial_size, allocation_size, temp_min_size, ds.free_blocks);
        }
        return 0;
    }
}

void * virtual_realloc(void * heapstart, void * ptr, uint32_t size) {
    if (size == 0) {
        virtual_free(heapstart, ptr);
        return NULL;
    } else if (ptr == NULL) {
        return virtual_malloc(heapstart, size);
    } else {
        data_structure ds;
        ds.heap = heapstart;

        unsigned char * ptr_realloc = ptr;
        
        unsigned char temp_min_size = ds.heap[0];
        unsigned char temp_initial_size = ds.heap[1];

        //calculates size for list of free blocks
        uint8_t free_blocks_size = temp_initial_size - temp_min_size + 1;

        //assigning index 2 of heap array to list of free blocks, and the correct index of heap array to allocatable space
        ds.free_blocks = &ds.heap[2];
        ds.allocatable_heap = &ds.heap[2 + ((1 << (free_blocks_size)))];
        int current_address = ptr_realloc - ds.allocatable_heap;
        int value = find_allocated_block(0, 0, current_address, temp_initial_size, temp_min_size, ds.free_blocks);

        if (value == -1) {
            return NULL;
        } else {
            virtual_free(heapstart, ptr);
            void * ptr_malloc = virtual_malloc(heapstart, size);

            if (ptr_malloc != NULL) {
                int alloc_size = 0;
                int power_alloc_size = (1 << alloc_size);
                while (power_alloc_size < size) {
                    alloc_size ++;
                    power_alloc_size = (1 << alloc_size);
                }
                unsigned char * temp = ptr_malloc;
                for (int j = 0; j < compare((1 << alloc_size), (1 << value)); j ++) {
                    temp[j] = ptr_realloc[j];
                }
                return ptr_malloc;
            } else {
                re_alloc(0, 0, current_address, temp_initial_size, value, temp_min_size, ds.free_blocks);
                return NULL;
            }
        }
        return NULL;
    }
}

void virtual_info(void * heapstart) {
    data_structure ds;

    ds.heap = heapstart;
    ds.min_size = ds.heap[0];
    ds.initial_size = ds.heap[1];
    ds.free_blocks = &ds.heap[2];

    helper(0, 0, ds.initial_size, ds.min_size, ds.free_blocks);

}