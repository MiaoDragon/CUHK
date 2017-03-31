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
    /* new variables  */
    self->_count = 0;
    uint32_t k, i;
    for (k = 0; k <= frame_size; k++)
        if (frame_size>>k == 1)     break;
    self->_k = k;
    self->_ref_num = (uint32_t*)malloc(sizeof(uint32_t)*page_num);
    memset(self->_ref_num, 0, page_num*sizeof(uint32_t));
    self->_used = (uint8_t*)malloc(sizeof(uint8_t)*frame_num);
    memset(self->_used, 0, frame_num*sizeof(uint8_t));
    self->_page_table = (int32_t*)malloc(sizeof(int32_t)*page_num);
    for (i = 0; i < page_num; i++)
        self->_page_table[i] = -1;
    self->_time = (uint32_t**)malloc(sizeof(uint32_t*)*page_num);
    for (i = 0; i < page_num; i++)
    {
        self->_time[i] = (uint32_t*)malloc(sizeof(uint32_t)*(lru_parameter+1));
        memset(self->_time[i], 0, (lru_parameter+1)*sizeof(uint32_t));
    }
    // TODO: initiate other members you add
    return self;
}

// Free manager_t
void deconstruct_manager(manager_t* self) {
    /* new variables  */
    free(self->_ref_num);
    free(self->_used);
    free(self->_page_table);
    uint32_t i;
    for (i = 0; i < self->_page_num; i++)
        free(self->_time[i]);
    free(self->_time);
    free(self);
    // TODO: free other members you add if in need
}

// TODO: return the physical address of the logical address 
uint32_t access(manager_t* self, uint32_t addr) {
    // get all variables
    self->_count++;  // time now
    uint32_t page_num = self->_page_num;
    uint32_t frame_num = self->_frame_num;
    uint32_t m = self->_lru_parameter;
    uint32_t *ref_num = self->_ref_num;
    uint32_t **time = self->_time;
    uint8_t *used = self->_used;
    int32_t *page_table = self->_page_table;
    // get page_id, offset
    uint32_t k = self->_k;
    uint32_t offset = ((1<<k)-1) & addr;
    uint32_t page_id = addr>>k;
    // update ref_num, time
    ref_num[page_id]++;
    uint32_t n = ref_num[page_id];
    time[page_id][0] = self->_count;
    if (n > m)  n = m;
    uint32_t i;
    for (i = n; i >= 1; i--)    time[page_id][i] = time[page_id][i-1];
    if (n < m)
        for (i = n+1; i <= m; i++)
            time[page_id][i] = time[page_id][1];
    // get frame_id, and return
    uint32_t frame_id = 0;
    // (0): already in memory
    if (page_table[page_id] != -1)
    {
        frame_id = page_table[page_id];
        return (frame_id<<k) | offset;
    }
    // (1): available
    for (i = 0; i < frame_num; i++)
        if (!used[i])
        {
            frame_id = i;
            used[i] = 1;    page_table[page_id] = frame_id;
            return (frame_id<<k) | offset;
        }
    // (2): get min of time
    uint32_t min = self->_count;   uint32_t min_i = 0;
    for (i = 0; i < page_num; i++)
        if (page_table[i] != -1 && time[i][m] < min)
        {
            min = time[i][m];
            min_i = i;
        }
    page_table[page_id] = page_table[min_i];
    frame_id = page_table[min_i];
    page_table[min_i] = -1;
    return (frame_id<<k) | offset;
}
