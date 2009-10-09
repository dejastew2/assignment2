#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BLOCKSIZE 10

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
	char c;
	int pos = 0;
	int totalsize = BLOCKSIZE;
	int startword = 0;
	
	/* If the passed file exists */
	if (pfile) {
		
		myword = (char *)safe_malloc(BLOCKSIZE);
		
		while((c = getc(pfile)) != EOF) {
			
			/* If we've hit a char, flag it and add it */
			if (isalpha(c)) {
				if (!startword) {
					startword = 1;
				}
				
				/* Add lowercase char to word */
				if (pos >= totalsize) {
					totalsize += BLOCKSIZE;
					myword = (char *)realloc(myword, totalsize);
				}
				myword[pos] = tolower(c);
				pos ++;
				
			/* If we have a non-alphabetic char */
			} else {
				/* If we just hit the end of a word, break loop */
				if (startword) {
					if (pos >= totalsize) {
						myword = (char *)realloc(myword, totalsize);
					}
					myword[pos] = '\0';
					ungetc(c, pfile);
					break;
				}
			}
			
		}
		
	/* If the passed file is invalid */
	} else {
		myword = NULL;
	}
	
	return myword;
}
