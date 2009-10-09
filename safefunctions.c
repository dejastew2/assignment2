#include <stdio.h>
#include <stdlib.h>

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

char *get_next_word(FILE *pfile) {
	char *myword;
	int startword = 0;
	
	if (pfile) {
		while((c = getc(file_pointer)) != EOF) {
			/* If we've hit a string of alphabetic chars */
			if (startword) {
				if (isalpha(c)) {
					/* Add lowercase char to word */
				} else {
					break;
				}
			} else {
				if (isalpha(c)) {
					/* Add lowercase char to word */
					startword = 1;
				}
			}
			
			if (isalpha(c)) {
				myWord[cPos] = tolower(c);
				isLastChar = 1;
				cPos ++;
			} else {
				if (isLastChar == 1) {
					myWord[cPos] = '\0';
					insert(myWord, ptotalwords);
				}
				isLastChar = 0;
				cPos = 0;
			}
		}		
	} else {
		myword = NULL;
	}
	
	return myword;
}
