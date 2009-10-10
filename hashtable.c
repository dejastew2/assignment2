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

unsigned hashkr(char *token) {
	unsigned hash;

	for (hash = 0; *token != '\0'; token ++) {
		hash = *token + 31 * hash;
	}
	return hash % HASHSIZE;
}

struct listnode *locate(char *word) {
	struct listnode *p;

	/* Moves through hashtable entries for a particular hash */
	for (p = hashtable[hashkr(word)]; p != NULL; p = p->next) {
		/* If the words are the same, return the node */
		if (strcmp(word, p->word) == 0) {
			return p;
		}
	}
	return NULL;
}

struct listnode *insert(char *word, int *curwords) {
	struct listnode *p;
	int wordhash;

	if (word != NULL) {
		/* If word doesn't exist, add it */
		if ((p = locate(word)) == NULL) {
			/* Creates node, then copies the word into it */
			p = (struct listnode*)safe_malloc(sizeof(*p));
			p->word = (char*)safe_malloc(strlen(word) + 1);
			strcpy(p->word, word);
			if (p->word == NULL) {
				return NULL;
			}

			/* Hashes word, then sets next var to current node in hash pos */
			wordhash = hashkr(word);
			p->next = hashtable[wordhash];

			/* Creates count and initializes it */
			p->timesfound = (int*)safe_malloc(sizeof(wordhash));
			*(p->timesfound) = 1;

			/* Points hash table to node */
			hashtable[wordhash] = p;

			/* Increments word counter */
			*curwords += 1;

		/* If word exists increment counter */
		} else {
			*(p->timesfound) += 1;
		}

	/* If no word was passed, return NULL */
	} else {
		p = NULL;
	}
	return p;
}

struct listnode *get_largest_tf(void) {
	struct listnode *p;
	struct listnode *biggest;

	int i;
	biggest = hashtable[0];
	/* Moves through hash table */
	for (i = 0; i < HASHSIZE; i ++) {
		/* Moves through nodes for a given hash */
		for (p = hashtable[i]; p != NULL; p = p->next) {
			/* If we have a current biggest */
			if (biggest != NULL) {
				if (p->timesfound != NULL) {
					/* If we have a new biggest, set it */
					if (*(p->timesfound) > *(biggest->timesfound)) {
						biggest = p;
					/* If we have a new better word, set the biggest var */
					} else if (*(p->timesfound) == *(biggest->timesfound)) {
						if (strcmp(p->word, biggest->word) < 0) {
							biggest = p;
						}
					}
				}
			/* If we don't have a current biggest, make one */
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
			/* Moves through all nodes at the current hash */
			for (curnode = hashtable[hashkr(rword)]; curnode != NULL; curnode = curnode->next) {
				/* If we've found the node to delete */
				if (toremove == curnode) {
					/* If the node is not the only one, set the last node to point at what it was pointing at */
					if (lastnode != curnode) {
						lastnode->next = curnode->next;
					/* If the node is the only one, reset the hash table entry */
					} else {
						hashtable[hashkr(rword)] = NULL;
					}
					free(curnode);
					break;
				/* If we haven't found the node yet, update the lastnode var */
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

	/* Move through the hash table */
	for (i = 0; i < HASHSIZE; i ++) {
		/* Move through a particular hash and print the values */
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
		/* If the number is smaller than k, display nothing here */
		if (k > foundnum) {
			printf(" ");
		/* If the number is smaller than k, isolate the digit of interest and print it */
		} else {
			digit = (foundnum % (k * 10)) / k;
			printf("%d", digit);
		}
	}

	/* Print the word and end the line */
	printf(" %s\n", foundword);
}
