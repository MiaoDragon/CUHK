#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "manager.h"

// Instantiate a manager_t
manager_t* new_memory_manager(uint32_t page_num, uint32_t frame_num, uint32_t frame_size, uint32_t lru_parameter) {
    manager_t* self = malloc(sizeof(manager_t));
    self->_page_num = page_num;
    self->_frame_num = frame_num;
    self->_frame_size = frame_size;
    self->_lru_parameter = lru_parameter;
    // TODO: initiate other members you add
    return self;
}

// Free manager_t
void deconstruct_manager(manager_t* self) {
    free(self);
    // TODO: free other members you add if in need
}

// TODO: return the physical address of the logical address 
uint32_t access(manager_t* self,  uint32_t addr) {
    return 0; 
}
