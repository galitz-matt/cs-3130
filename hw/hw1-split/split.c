#include "split.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char** string_split(const char *input, const char *sep, int *num_words) {
	char **result = NULL;
	if (strchr(sep, *input) != NULL) {
		char *word = malloc(sizeof(char));
		word[0] = '\0';
		result = realloc(result, (*num_words + 1) * sizeof(char *));
		result[*num_words] = word;
		
		(*num_words)++;
	}

	while (*input != '\0') {
		input += strspn(input, sep);
		int spn = strcspn(input, sep);

		char *word = malloc((spn + 1) * sizeof(char));
		strncpy(word, input, spn);
		word[spn] = '\0';

		result = realloc(result, (*num_words + 1) * sizeof(char *));
		result[*num_words] = word;
		(*num_words)++;
		input += spn;
	}

	return result;
}

char *create_sep(int argc, char* argv[]) {
	// If no delim is specified, return \t
	if (argc <= 1) {
		return strdup(" \t");
	}
	// find length of sep
	int totalLength = 1;
	for (int i = 1; i < argc; i++) {
		totalLength += strlen(argv[i]);
	}
	// allocate memory for sep of size totalLength
	char *sep = malloc(totalLength * sizeof(char));
	if (!sep) {
		return NULL;
	}
	// build sep string
	char *ptr = sep;
	for (int i = 1; i < argc; i++) {
		strcpy(ptr, argv[i]);
		ptr += strlen(argv[i]);
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
