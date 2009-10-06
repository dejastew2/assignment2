#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void *safe_malloc(size_t size);

int main(int argc, char *argv[]) {

	int reset_n_value = 0;	/* Boolean to catch -n flag */
	int n_value = 10;			/* Number of words to print */
	int i;
	char c;

	/* Loops through each argument */
	for (i = 1; i < argc; i ++) {

		char* file_name;	/* Pointer to argument */

		/* Points file_name to argument (after allocating mem) */
		file_name = (char*)safe_malloc(strlen(argv[i] + 1));
		strcpy(file_name, argv[i]);

		/* If first arg is -n set flag and wait for next arg */
		if ((i == 1) && (file_name[0] == '-')) {
			if (file_name[1] == 'n') {
				reset_n_value = 1;
			} else {
				printf("Invalid parameter (-n expected)\n");
				exit(1);
			}

		/* If second arg is new n value, set it (if positive) */
		} else if((i == 2) && (reset_n_value == 1)) {
			int new_n = atoi(file_name);			
			if (new_n > 0) {
				n_value = new_n;
			} else {
				printf("Error reading -n option. Must be a positive int!\n");
				exit(1);
			}

		/* If arg is not a special case, treat as a file name */
		} else {
			/* Attempts to open the file for reading */
			FILE *file_pointer;
			file_pointer = fopen(file_name, "r");

			/* If file was successfully opened */
			if (file_pointer != NULL) {

				char myWord[20];
				int cPos = 0;
				int isLastChar = 0;
				
				while((c = getc(file_pointer)) != EOF) {
					if (isalpha(c)) {
						myWord[cPos] = tolower(c);
						isLastChar = 1;
						cPos ++;
					} else {
						if (isLastChar == 1) {
							myWord[cPos] = '\0';
							printf("%s\n", myWord);
						}
						isLastChar = 0;
						cPos = 0;
					}
				}
				fclose(file_pointer);
			/* If file was not successfully opened, print error */
			} else {
				perror(file_name);
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
		exit(1);
	}
}
