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
        // no page table exists
        return (size_t)-1;
    }

    // calculate page number and offset from the virtual address
    size_t page_number = va >> POBITS;
    size_t offset = va & ((1 << POBITS) - 1);

    // max index in table
    size_t max_index = (4096 / sizeof(size_t)) - 1;
    if (page_number > max_index) {
        // page number is out of bounds return error
        return (size_t)-1;
    }

    // access the page table
    size_t *page_table = (size_t *)ptbr;
    size_t entry = page_table[page_number];

    // check if the entry is valid
    if ((entry & 1) == 0) {
        return (size_t)-1;
    }

    // calculate the physical address
    size_t physical_page_number = entry >> 1;
    size_t physical_address = (physical_page_number << POBITS) | offset;
    return physical_address;
}


void page_allocate(size_t va) {
    if (ptbr == 0) {
        // allocate the root page table if doesn't exist
        if (posix_memalign((void **)&ptbr, 4096, 4096) != 0) {
            perror("Failed to allocate memory for page table\n");
            exit(EXIT_FAILURE);
        }
        // initialize the page table w/ zeros
        memset((void *)ptbr, 0, 4096);
    }

    size_t page_number = va >> POBITS;
    size_t *page_table = (size_t *)ptbr;

    size_t max_index = (4096 / sizeof(size_t)) - 1;
    // make sure index isn't out of bounds
    if (page_number > max_index) {
        perror("Virtual address exceeds page table capacity\n");
        exit(EXIT_FAILURE);
    }

    if ((page_table[page_number] & 1) == 0) {
        // entry is invalid, allocate a new page
        size_t *new_page;
        if (posix_memalign((void **)&new_page, 4096, 4096) != 0) {
            perror("Failed to allocate memory for new page\n");
            exit(EXIT_FAILURE);
        }
        // initialize new page w/ zeros
        memset(new_page, 0, 4096);

        size_t physical_page_number = ((size_t)new_page) >> POBITS;
        // mark entry as valid
        page_table[page_number] = (physical_page_number << 1) | 1;
    }
}
