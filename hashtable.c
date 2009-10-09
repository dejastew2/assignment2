#include "safefunctions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define HASHSIZE 1000

struct listnode {
	struct listnode *next;
	char *word;
	int *timesfound;
};

static struct listnode *hashtable[HASHSIZE];

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

struct listnode *insert(char *word, int *curwords) {
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
		*curwords += 1;
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
					} else if (*(p->timesfound) == *(biggest->timesfound)) {
						if (strcmp(p->word, biggest->word) < 0) {
							biggest = p;
						}
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
