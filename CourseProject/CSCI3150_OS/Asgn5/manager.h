#pragma once

#include <stdint.h>
#include <math.h>

// Define a structure named 'manager_t'  
// !!! Do NOT modify the name of 'manager_t'
// The suffix '_t' identifies that this is a structure
typedef struct {
    uint32_t _page_num; // page number
    uint32_t _frame_size; // frame size
    uint32_t _frame_num; // frame number
    uint32_t _lru_parameter; // value of parameter in LRU-parameter 
    /* new variables  */
    uint32_t _count;
    uint32_t _k;
    uint32_t *_ref_num;     // [page_num] array
    uint32_t **_time;       // [page_num][lru_parameter+1] array
    uint8_t *_used;         // [frame_num] array; 1 -- used, 0 -- unused
    int32_t *_page_table;   // [page_num] array

    // TODO: add other members if you need to 
} manager_t;

// Instantiate a manager_t
manager_t* new_memory_manager(uint32_t page_num, uint32_t frame_num, uint32_t frame_size, uint32_t lru_parameter);

// Free manager_t
void deconstruct_manager(manager_t* self);

// TODO: return the physical address of the logical address 
uint32_t access(manager_t* self, uint32_t addr);
