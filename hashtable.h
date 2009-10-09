#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

unsigned hashkr(char *token);
struct listnode *locate(char *word);
struct listnode *insert(char *word, int *curwords);
struct listnode *get_largest_tf(void);
void delete(struct listnode *toremove);
void print_hash_table(void);
void print_node(struct listnode *toprint);

#endif
