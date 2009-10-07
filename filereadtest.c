#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define HASHSIZE 1000

unsigned hashkr(char *token);
struct listnode *locate(char *word);
struct listnode *insert(char *word);
struct listnode *get_largest_tf(void);
void *safe_malloc(size_t size);
/*void *safe_free(void *fname);*/

struct listnode {
	struct listnode *next;
	char *word;
	int *timesfound;
};

static struct listnode *hashtable[HASHSIZE];

int main(int argc, char *argv[]) {

	int reset_n_value = 0;	/* Boolean to catch -n flag */
	int n_value = 10;			/* Number of words to print */
	int i;
	int j;
	char c;
	char *foundword;
	int foundnum;
	struct listnode *curnode;

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
							insert(myWord);
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

	printf("Before\n");
	curnode = get_largest_tf();
	printf("After\n");
	if (curnode != NULL) {
		if (curnode->word != NULL) {
			foundword = curnode->word;
			/*printf("Word exists\n");*/
		}
		if (curnode->timesfound != NULL) {
			foundnum = *(curnode->timesfound);
			/*printf("Num exists\n");*/
		}
		/*printf("%s %d\n", foundword, foundnum);*/
	}
	printf("Much later\n");

	return 0;
}

/* Hash function adapted from K&R book */
unsigned hashkr(char *token) {
	unsigned hash;

	for (hash = 0; *token != '\0'; token ++) {
		hash = *token + 31 * hash;
	}
	return hash % HASHSIZE;
}

struct listnode *locate(char *word) {
	struct listnode *p;

	for (p = hashtable[hashkr(word)]; p != NULL; p = p->next) {
		if (strcmp(word, p->word) == 0) {
			return p;
		}
	}
	return NULL;
}

struct listnode *insert(char *word) {
	struct listnode *p;
	int wordhash;

	if ((p = locate(word)) == NULL) {
		p = (struct listnode*)safe_malloc(sizeof(*p));
		p->word = (char*)safe_malloc(strlen(word) + 1);
		strcpy(p->word, word);
		if (p->word == NULL) {
			return NULL;
		}
		wordhash = hashkr(word);
		p->next = hashtable[wordhash];
		p->timesfound = (int*)safe_malloc(sizeof(wordhash));
		*(p->timesfound) = 1;
		hashtable[wordhash] = p;
	} else {
		*(p->timesfound) ++;
	}
	return p;
}

struct listnode *get_largest_tf(void) {
	struct listnode *p;
	struct listnode *biggest;

	int i;
	for (i = 0; i < HASHSIZE; i ++) {

		for (p = hashtable[i]; p != NULL; p = p->next) {
			if (biggest != NULL) {
				if (p->timesfound != NULL) {
					/*
					if (*(p->timesfound) > *(biggest->timesfound)) {
						biggest = p;
					}
					*/
				}
			} else {
				biggest = p;
			}
		}

	}
	return biggest;
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

/*
void *safe_free(void *fname) {
	void* ret;
	ret = free(fname);
	if (ret != NULL) {
		return ret;
	} else {
		perror("safe_free");
		exit(1);
	}
}
*/
