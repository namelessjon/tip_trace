/*
 * string_list.h
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 */

#ifndef STRING_LIST_H
#define STRING_LIST_H

#define DEFAULT_LIST_SIZE (8)
#define MAX_STRING_LENGTH (64)


typedef struct string_list {
    int mlen;
    int len;
    char ** list;
} string_list_t;

string_list_t * new_string_list(void);
// creates a new string_list

void destroy_string_list(string_list_t *s);
// destroys a string_list, freeing all memory

int string_list_push(string_list_t *s, const char *c);
// adds a new string to the list, resizing if needed

char * string_list_at(string_list_t *s, int index);
// returns pointer to the string at the given index, or NULL if out of bounds

int string_list_length(string_list_t *s);
// returns the length of the stringlist.

#endif // STRING_LIST_H
