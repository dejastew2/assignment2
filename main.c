#include "hashtable.h"
#include "safefunctions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int add_to_hash_table(FILE *pfile);

struct listnode {
	struct listnode *next;
	char *word;
	int *timesfound;
};

int main(int argc, char *argv[]) {

	int reset_n_value = 0;	/* Boolean to catch -n flag */
	int passed_files = 0;	/* Boolean to switch to stdin */
	int n_value = 10;		/* Number of words to print */
	int i;
	struct listnode *curnode;

	int totalwords = 0;

	/* Loops through each argument */
	for (i = 1; i < argc; i ++) {

		char* file_name;	/* Pointer to argument */

		/* Points file_name to argument (after allocating mem) */
		file_name = (char*)safe_malloc(strlen(argv[i]) + 1);
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
				totalwords += add_to_hash_table(file_pointer);
				fclose(file_pointer);

				if (passed_files == 0) {
					passed_files = 1;
				}

			/* If file was not successfully opened, print error */
			} else {
				perror(file_name);
			}

		}

		free(file_name);
	}

	if (passed_files == 0) {
		totalwords += add_to_hash_table(stdin);
	}

	/* Prints summary line */
	printf("The top %d words (out of %d) are:\n", n_value, totalwords);

	/* Loops through N times printing the largest node, then removing it */
	for (i = 0; i < n_value; i ++) {
		curnode = get_largest_tf();
		if (curnode != NULL) {
			print_node(curnode);
			delete(curnode);
		}
	}

	return 0;
}

int add_to_hash_table(FILE *pfile) {
	char *curword;
	char c;
	int addednodes = 0;
	int *paddednodes = &addednodes;

	/* As long as we aren't EOF, get next word and insert it */
	while((c = getc(pfile)) != EOF) {
		ungetc(c, pfile);
		curword = get_next_word(pfile);
		insert(curword, paddednodes);
	}

	return addednodes;
}
