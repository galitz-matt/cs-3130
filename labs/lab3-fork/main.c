#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_system.h"

int main() {
	char buffer[1024];
	while (1) {
		if (fgets(buffer, 1024, stdin) == NULL) {
			perror("fgets");
			puts("Error loading input\n");
		}	
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strcmp(buffer, ".") == 0) {
			break;
		}
		my_system(buffer);
	}
	return 0;
}

