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
    // TODO: add other members if you need to 
} manager_t;

// Instantiate a manager_t
manager_t* new_memory_manager(uint32_t page_num, uint32_t frame_num, uint32_t frame_size, uint32_t lru_parameter);

// Free manager_t
void deconstruct_manager(manager_t* self);

// TODO: return the physical address of the logical address 
uint32_t access(manager_t* self, uint32_t addr);
