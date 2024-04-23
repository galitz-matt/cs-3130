#include "lab.h"

#include <string.h>     // for memset
#include <stdio.h>      // for printf

/* When complete, this function should fill in "buffer"
 * with a length-character \0-termianted string such that
 * check_passphrase(buffer) is true.
 *
 * The implementation we supplied tries the guesses of
 * 'a', 'b', and 'c' and prints out how long it takes
 * to check each of them.
 *
 * To do so, your implementation should rely on timing
 * how long check_passphrase takes, most likely by using
 * "measure_once" wrapper function.
 *
 * (Your implementation may not examine the memory in which
 *  the passphrase is stored in another way.)
 */
void find_passphrase(char *buffer, int length) {
    memset(buffer, 0, length + 1);  // Ensure null termination
    char test_buffer[length + 1];
    memset(test_buffer, 0, length + 1);
    int result;
    long time;
    for (int i = 0; i < length; i++) {
        long best_time = 0;
        char best_char = 0;
        for (char c = 'a'; c <= 'z'; c++) {
            test_buffer[i] = c;  
            long total_time = 0;
            int measurements = 5;  
            for (int j = 0; j < measurements; j++) {
                time = measure_once(&result, test_buffer, check_passphrase);
                total_time += time;
                if (result == 1) {
                    strcpy(buffer, test_buffer);
                    return;
                }
            }
            long average_time = total_time / measurements;
            if (average_time > best_time) {
                best_time = average_time;
                best_char = c;
            }
            test_buffer[i] = 0;
        }
        test_buffer[i] = best_char;
        buffer[i] = best_char;  
    }
    buffer[length] = '\0';
}

