#define _XOPEN_SOURCE 700
#include "config.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdalign.h>


size_t ptbr = 0;


size_t translate(size_t va) {
    if (ptbr == 0) {
        return (size_t)-1;
    }

    size_t page_number = va >> POBITS;
    size_t offset = va & ((1 << POBITS) - 1);
    size_t *page_table = (size_t *)ptbr;
    size_t max_index = 4096 / sizeof(size_t) - 1;

    if (page_number > max_index) {
        return (size_t)-1; // page number is out of bounds return error
    }
    size_t entry = page_table[page_number];

    if ((entry & 1) == 0) {
        return (size_t)-1;
    }

    size_t physical_page_number = entry >> 1;
    size_t physical_address = (physical_page_number << POBITS) | offset;
    return physical_address;
}


void page_allocate(size_t va) {
    if (ptbr == 0) {
        if (posix_memalign((void **)&ptbr, 4096, 4096) != 0) {
            perror("Failed to allocate memory for page table\n");
            exit(EXIT_FAILURE);
        }
        memset((void *)ptbr, 0, 4096);
    }
    
    size_t page_number = va >> POBITS;
    size_t *page_table = (size_t *)ptbr;
    
    if ((page_table[page_number] & 1) == 0) {
        size_t *new_page;
        if (posix_memalign((void **)&new_page, 4096, 4096) != 0) {
            perror("Failed to allocate memory for new page\n");
            exit(EXIT_FAILURE);
        }
        size_t physical_page_number = ((size_t)new_page) >> POBITS;
        page_table[page_number] = (physical_page_number << 1) | 1;
    }
}

