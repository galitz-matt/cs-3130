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
- Time: O(LEVELS)
- Space: O(1)
### page_allocate()
- Time: O(LEVELS)
- Space: O(LEVELS)

## Deallocation Interface

 We propose a deallocation function:

```c
void page_deallocate(size_t va);
```
Which would free physical memory associated w/ a virtual address. (Essentially a complement for page_allocate()).

### Challenges
- <strong>Resource Counting</strong>: To deallocate pages safely, the library must track the number of references to each page. This would require singificant changes to the current implementation.
- <strong>Memory Fragmentation</strong>: Deallocating individual pages can lead to memory fragmentation. Addressing fragmentation requires implementation of compaction strategies which is non-trivial.
- <strong>Safety</strong>: Ensuring that a page is safe to deallocate is non-trivial. Accidental deallocation of in-use memory can lead to undefined behavior and security vulnerabilities.
