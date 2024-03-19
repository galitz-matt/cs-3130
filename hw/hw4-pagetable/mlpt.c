#define _XOPEN_SOURCE 700
#include "config.h"
#include "mlpt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t ptbr;

size_t translate(size_t virtual_address) {
    size_t offset = virtual_address & ((1 << POBITS) - 1);
    int vpn_bits = POBITS - 3; // calc bits for VPN
    int vpn_mask = (1 << vpn_bits) - 1; // mask for VPN extraction
    size_t vpn;
    if (ptbr == 0) {
        return ~0uL; // throw error if ptbr not set
    }
    size_t current_pt = ptbr;
    for (int i = LEVELS - 1; i >= 0; i -= 1) {
        vpn = (virtual_address >> (POBITS + (i * vpn_bits))) & vpn_mask; // compute vpn for current level
        size_t *pte_address = (size_t *) (current_pt + vpn + sizeof(size_t)); // compute address of pte
        if (*pte_address & 0x1) { // check if page is present
            current_pt = *pte_address & (~0x1); // get physical address from pte
        }
        else {
            return ~0uL; // return error if page isn't present
        }
    }
    current_pt += offset; // add offset to phsyical address
    return current_pt;
}

void page_allocate(size_t virtual_address) {
    int pt_size = 1 << POBITS;  // compute size of page table
    int vpn_bits = POBITS - 3; // compute bits for VPN
    int vpn_mask = (1 << vpn_bits) - 1; // create mask for VPN
    size_t vpn;
    if (ptbr == 0) {
        posix_memalign((void **) &ptbr, pt_size, pt_size); // allocate root page table if not already
        memset((size_t *) ptbr, 0, pt_size); // init pt w/ zeros
    }
    size_t current_pt = ptbr;
    for (int i = LEVELS - 1; i >= 0; i -= 1) {
        vpn = (virtual_address >> (POBITS + (i * vpn_bits))) & vpn_mask; // compute VPN for current level
        size_t *pte_address = (size_t *) (current_pt + vpn * sizeof(size_t)); // compute address of pte
        if ((*pte_address & 0x1) == 0) { // check if page not already allocated
            posix_memalign((void **) &current_pt, pt_size, pt_size); // allocate a new page
            *pte_address = current_pt | 0x1; // update pte w/ address of new page and mark as present
        }
        else {
            current_pt = *pte_address & ~0x1; // if page already allocated, get physical address
        }
    }
}
