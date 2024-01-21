#include "split.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void string_split(const char *input, const char *sep, int *num_words) {
	// if (strchr(sep, *input) != NULL) {
	// 	printf("[]");
	// }	
	for (; *input != '\0';) {
		input += strspn(input, sep);
		int spn = strcspn(input, sep);
		printf("[");
		for (int i = 0; i < spn; i++) {
			printf("%c", *input);
			input++;
		}
		printf("]");
	}
	printf("\n");
}

char *create_sep(int argc, char* argv[]) {
	// If no delim is specified, return \t
	if (argc <= 1) {
		char *sep = malloc(2 * sizeof(char));
		if (sep == NULL) {
			return NULL;	
		}
		sep[0] = '\t';
		sep[1] = '\0';
		return sep;
	}
	// find length of sep
	int totalLength = 1;
	for (int i = 1; i < argc; i++) {
		totalLength += strlen(argv[i]);
	}
	// allocate memory for sep of size totalLength
	char *sep = malloc(totalLength * sizeof(char));
	if (sep == NULL) {
		return NULL;
	}
	// build sep string
	sep[0] = '\0';
	for (int i = 1; i < argc; i++) {
		strcat(sep, argv[i]);
	}
	return sep;
}

int main(int argc, char* argv[]) {
	// concatenate elements of argv to create sep
	char *sep = create_sep(argc, argv);
	if (sep == NULL) {
		fprintf(stderr, "memory allocation failed\n");
		return 1;
	}
	// instantiate buffer of 4000 bytes	
	char input[4000];
	// enter main loop
	while (1) {
		// load input
		if (fgets(input, sizeof(input), stdin) == NULL) {
			fprintf(stderr, "error loading input\n");
			continue;
		}
		// replace newline char w/ null terminator
		input[strcspn(input, "\n")] = '\0';
		// check for exit condition
		if (strcmp(input, ".") == 0) {
			break;
		}
		// split string
		int num_words = 0;
		string_split(input, sep, &num_words);
	}
	free(sep);
	return 0;
}
