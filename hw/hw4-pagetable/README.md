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
