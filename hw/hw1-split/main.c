#define _POSIX_C_SOURCE 200809L
#include "split.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *create_sep(int argc, char* argv[]) {
	if (argc <= 1) {
		return strdup(" \t");
	}
	int totalLength = 1;
	for (int i = 1; i < argc; i++) {
		totalLength += strlen(argv[i]);
	}
	char *sep = malloc(totalLength * sizeof(char));
	if (!sep) {
		return NULL;
	}
	char *ptr = sep;
	for (int i = 1; i < argc; i++) {
		strcpy(ptr, argv[i]);
		ptr += strlen(argv[i]);
	}
	return sep;
}

int main(int argc, char* argv[]) {
	char *sep = create_sep(argc, argv);
	if (sep == NULL) {
		fprintf(stderr, "memory allocation failed\n");
		return 1;
	}
	char input[4000];
	while (1) {
		if (fgets(input, sizeof(input), stdin) == NULL) {
			fprintf(stderr, "error loading input\n");
			continue;
		}
		input[strcspn(input, "\n")] = '\0';
		if (strcmp(input, ".") == 0) {
			break;
		}
		int num_words = 0;
		char **result = string_split(input, sep, &num_words);
		for (int i = 0; i < num_words; i++) {
			printf("[%s]", result[i]);
			free(result[i]);
		}
		printf("\n");
		free(result);
	}
	free(sep);
	return 0;
}

