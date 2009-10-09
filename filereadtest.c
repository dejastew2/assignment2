#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define HASHSIZE 1000

unsigned hashkr(char *token);
struct listnode *locate(char *word);
struct listnode *insert(char *word);
struct listnode *get_largest_tf(void);
void delete(struct listnode *toremove);
void print_hash_table(void);
void print_node(struct listnode *toprint);
void *safe_malloc(size_t size);

struct listnode {
	struct listnode *next;
	char *word;
	int *timesfound;
};

static struct listnode *hashtable[HASHSIZE];
static int totalwords;

int main(int argc, char *argv[]) {

	int reset_n_value = 0;	/* Boolean to catch -n flag */
	int n_value = 10;			/* Number of words to print */
	int i;
	char c;
	struct listnode *curnode;

	totalwords = 0;

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

	/*
	print_hash_table();
	printf("\n");
	*/
	printf("The top %d words (out of %d) are:\n", n_value, totalwords);
	for (i = 0; i < n_value; i ++) {
		curnode = get_largest_tf();
		if (curnode != NULL) {
			print_node(curnode);
			delete(curnode);
		}
	}

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
		totalwords += 1;
	} else {
		*(p->timesfound) += 1;
	}
	return p;
}

struct listnode *get_largest_tf(void) {
	struct listnode *p;
	struct listnode *biggest;

	int i;
	biggest = hashtable[0];
	for (i = 0; i < HASHSIZE; i ++) {
		for (p = hashtable[i]; p != NULL; p = p->next) {
			if (biggest != NULL) {
				if (p->timesfound != NULL) {
					if (*(p->timesfound) > *(biggest->timesfound)) {
						biggest = p;
					}
				}
			} else {
				biggest = p;
			}
		}
	}

	return biggest;
}

void delete(struct listnode *toremove) {
	char *rword;
	struct listnode *curnode;
	struct listnode *lastnode;

	if (toremove) {
		if (toremove->word) {
			rword = toremove->word;

			lastnode = hashtable[hashkr(rword)];
			for (curnode = hashtable[hashkr(rword)]; curnode != NULL; curnode = curnode->next) {
				if (toremove == curnode) {
					if (lastnode != curnode) {
						lastnode->next = curnode->next;
					} else {
						hashtable[hashkr(rword)] = NULL;
					}
					free(curnode);
					break;
				} else {
					lastnode = curnode;
				}
			}
		}
	}
}

void print_hash_table(void) {
	struct listnode *p;
	char *myword;
	int mynum;
	unsigned myhash;
	int i;
	for (i = 0; i < HASHSIZE; i ++) {
		for (p = hashtable[i]; p != NULL; p = p->next) {
			myword = p->word;
			mynum = *(p->timesfound);
			myhash = hashkr(myword);
			printf("%s %d %d\n", myword, mynum, myhash);
		}
	}
}

void print_node(struct listnode *toprint) {
	char *foundword = toprint->word;
	int foundnum = *(toprint->timesfound);
	int k;
	int digit;

	for (k = 100000000; k > 0; k /= 10) {
		if (k > foundnum) {
			printf(" ");
		} else {
			digit = (foundnum % (k * 10)) / k;
			printf("%d", digit);
		}
	}

	printf(" %s\n", foundword);
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

