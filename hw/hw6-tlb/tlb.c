#include "tlb.h"

size_t tlbCache[16][4][4];

size_t extract_vpn(size_t va) {
	return va >> POBITS;
}

size_t extract_index(size_t va) {
	return (va >> POBITS) & 0xF;
}

size_t extract_tag(size_t va) {
	return va >> (POBITS + 4);
}

int identify_LRU(size_t index) {
	for (int i = 0; i < 4; i++) {
		if (tlbCache[index][i][2] == 0) {
			return i;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (tlbCache[index][i][2] == 0) {
			return i;
		}
	}
	return 0;
}

void update_LRU(size_t index, size_t way) {
	size_t prev_LRU_value = tlbCache[index][way][1];
	if (prev_LRU_value == 0) {
		tlbCache[index][way][1] = 1;
		for (int i = 0; i < 4; i++) {
			if (tlbCache[index][i][2] == 1 && i != way) {
				tlbCache[index][i][1]++;
			}
		}
		return;
	} 
	tlbCache[index][way][1] = 0;
	for (int i = 0; i < 4; i++) {
		if (tlbCache[index][i][2] == 1 && tlbCache[index][i][1] < prev_LRU_value) {
			tlbCache[index][i][1]++;
		}
	}
}

void tlb_clear() {
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				tlbCache[i][j][k] = 0;
			}
		}
	}
}

int tlb_peek(size_t va) {
	size_t vpn = extract_vpn(va);
	size_t index = extract_index(va);
	size_t tag = extract_tag(va);
	for (int i = 0; i < 4; i++) {
		if (tag == tlbCache[index][i][3] && tlbCache[index][i][2] == 1) {
			return tlbCache[index][i][1];
		}	
	}
	return 0;
}

size_t tlb_translate(size_t va) {
	size_t vpn = extract_vpn(va);
	size_t index = extract_index(va);
	size_t tag = extract_tag(va);
	size_t vpn_mask = (1 << POBITS) - 1;
	size_t offset = va & vpn_mask;
	size_t va_no_offset = vpn << POBITS;
	size_t ppn, pa;
	int is_hit = 0;
	for (int i = 0; i < 4; i++) {
		if (tlbCache[index][i][2] == 1 && tag == tlbCache[index][i][3]) {
			is_hit = 1;
			ppn = (tlbCache[index][i][0] >> POBITS) << POBITS;
			pa = ppn + offset;
			update_LRU(index, i);
			return pa;
		} 
	}
	ppn = translate(va_no_offset);
	if (ppn == -1) {
		return -1;
	}
	ppn = (ppn >> POBITS) << POBITS;
	pa = ppn + offset;
	int chosen_way = identify_LRU(index);
	tlbCache[index][chosen_way][0] = pa;
	tlbCache[index][chosen_way][2] = 1;
	tlbCache[index][chosen_way][3] = tag;
	update_LRU(index, chosen_way);
	return pa; 	
}
