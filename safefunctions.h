#ifndef SAFEFUNCTIONS_H_INCLUDED
#define SAFEFUNCTIONS_H_INCLUDED

#include <stdlib.h>

void *safe_malloc(size_t size);
char *get_next_word(FILE *pfile);

#endif
