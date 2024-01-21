#include "split.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **string_split(const char *input, const char *sep, int *num_words) {
	char **foo;
	return foo;
}

int main() {
	char *str1 = "first____second__third";
	char *str2 = "_";
	for (; *str1 != '\0';) {
		str1 += strcspn(str1, str2);
		size_t i = strspn(str1, str2);
		printf("%zu\n", i);
		str1 += i;
	}
}
