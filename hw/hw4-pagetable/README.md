# Multi-Level Page Table Library (MLPT)

The Multi-Level Page Table Library provides a simulation of a multi-level page table lookup and allocation mechanism, typically used in virtual memory management systems. This implementation allows for the configuration of the page table's levels and page offset sizes, making it adaptable to different virtual memory schemes.

## Customizing config.h

To configure the MLPT library for various page table levels and page offset sizes, adjust the settings in the `config.h` file:

- `LEVELS`: Specifies the number of levels in the page table. Valid values range from 1 to 6.
- `POBITS`: Specifies the size of page offsets. Valid values range from 4 to 18.

Example configuration:

```c
#define LEVELS  1
#define POBITS  12
```

Example usage:

```c
#include "mlpt.h"

int main() {
    // Example virtual address
    size_t va = 0x12345;

    // Allocate a page for the given virtual address
    page_allocate(va);

    // Translate the virtual address to a physical address
    size_t pa = translate(va);

    if (pa == (size_t)-1) {
        printf("Translation failed: Address not allocated.\n");
    } else {
        printf("Translated address: %lx\n", pa);
    }

    return 0;
}
```

## Big-O Analysis
### translate()
- `Time Compleixty`: O(LEVELS)
- `Space Complexity`: O(1)
### page_allocate()
- `Time Complextiy`: O(LEVELS)
- `Space Complexity`: O(LEVELS)
