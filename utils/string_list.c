/*
 * string_list.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 */
#include <malloc.h>
#include <string.h>
#include "string_list.h"

static int next_size_up(int size) {
// returns the next power of two size that can contain all the current data
// by sizing up in powers of 2, should avoid too many resize operations which
// should improve the speed.
   if (size < DEFAULT_LIST_SIZE) {
       // if we're this small, just set to the default and return
        return DEFAULT_LIST_SIZE;
   } else {
        int new_size = DEFAULT_LIST_SIZE;
        while (new_size <= size) {
            new_size = new_size * 2;
        }
        return new_size;
   }
}


string_list_t * new_string_list(void) {
// creates a new string_list
    string_list_t * s;
    s = malloc(sizeof(string_list_t)); // allocate space for the struct
    if (s != NULL) { // if that is successful, start to assign values
        s->len = 0;
        s->mlen = DEFAULT_LIST_SIZE;
        s->list = malloc(s->mlen*sizeof(char*));
        if (s->list != NULL) { // check we allocated the initial size
            return s;          // if so, return it.
        } else {               // if not, free the array and return NULL
            free(s);
            return NULL;
        }
    } else { // complete failure. return NULL.
        return NULL;
    }
}

void destroy_string_list(string_list_t *s) {
// destroys a string_list, freeing all memory
    int i;
    if (NULL != s) {
        if (s->mlen > 0) {
            if (s->list) {
                for (i = 0; i < s->mlen; ++i) {
                    if (s->list[i]) {
                        free(s->list[i]);
                        s->list[i] = NULL;
                    }
                }
                s->mlen = 0;
            }
            free(s->list);
            s->list = NULL;
        }
        // make sure we free it, even if we somehow had mlen == 0
        if (s->list) {
            free(s->list);
            s->list = NULL;
        }
        free(s);
        s = NULL;
    }

}


int string_list_push(string_list_t *s, const char *c) {
// adds a new string to the list, resizing if needed
    char *string;
    int to_move;

    // allocate space for a new string
    string = malloc(MAX_STRING_LENGTH*sizeof(char));
    if (!string) {
        return -1;
    }

    // figure out how much we need to copy.
    to_move = (strlen(c) >= MAX_STRING_LENGTH) ? MAX_STRING_LENGTH : strlen(c)+1;

    memmove(string, c, to_move);

    // if our array is longer than our available memory
    // we need more memory
    if (s->len >= s->mlen) {
        int size = next_size_up(s->mlen);
        s->list = realloc(s->list, size*sizeof(char*));
        if (NULL == s->list) {
            return -1;
        }
        s->mlen = size;
    }

    s->list[s->len] = string;
    s->len++;
    return s->len;

}
char * string_list_at(string_list_t *s, int index) {
// returns pointer to the string at the given index, or NULL if out of bounds
    if (index < s->len && index > -1) {
        return s->list[index];
    } else {
        return NULL;
    }
}


int string_list_length(string_list_t *s) {
// returns the length of the stringlist.
    return s->len;
}

