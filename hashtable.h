#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

/* Return the hash of a string (from K&R book) */
unsigned hashkr(char *token);

/* Locates a node in the hash table, if it exists */
struct listnode *locate(char *word);

/* Inserts a word into the hash table and increments curwords */
struct listnode *insert(char *word, int *curwords);

/* Returns largest valued node from hash table */
struct listnode *get_largest_tf(void);

/* Removes a node from the hash table */
void delete(struct listnode *toremove);

/* Prints the entire hash table (for debugging) */
void print_hash_table(void);

/* Prints a single node in the hash table */
void print_node(struct listnode *toprint);

#endif
