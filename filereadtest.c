#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *safe_malloc(size_t size);

int main(int argc, char *argv[]) {

	int reset_n_value;
	int n_value;
	int i;
	char c;

	reset_n_value = 0;
	n_value = 10;

	for (i = 1; i < argc; i ++) {

		char* file_name;

		file_name = (char*)safe_malloc(strlen(argv[i] + 1));
		strcpy(file_name, argv[i]);

		if ((i == 1) && (file_name[0] == '-')) {
			if (file_name[1] == 'n') {
				reset_n_value = 1;
			} else {
				printf("Invalid parameter (-n expected)\n");
				exit(1);
			}
		} else if((i == 2) && (reset_n_value == 1)) {
			int new_n;
			new_n = atoi(file_name);
			if (new_n > 0) {
				n_value = new_n;
			} else {
				printf("Error reading -n option. Must be a positive int!\n");
				exit(1);
			}
		} else {
			FILE *file_pointer;
			file_pointer = fopen(file_name, "r");

			if (file_pointer != NULL) {
				while((c = getc(file_pointer)) != EOF) {
					putchar(c);
				}
				fclose(file_pointer);
			}

		}

		free(file_name);
	}

	printf("N value is now: %d\n", n_value);

	return 0;
}

void *safe_malloc(size_t size) {
	void* ret;
	ret = malloc(size);
	if (ret != NULL) {
		return ret;
	} else {
		perror("safe_malloc");
	}
}
