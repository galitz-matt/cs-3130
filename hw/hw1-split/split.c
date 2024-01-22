#include "split.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **string_split(const char *input, const char *sep, int *num_words) {
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

