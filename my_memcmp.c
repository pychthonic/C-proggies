#ifndef STDDEF_H_
#define STDDEF_H_

#include <stddef.h>

#endif

/* See my_memcmp_add_vector_main.c */ 

int my_memcmp(const unsigned char *a, const unsigned char *b, size_t n) {
    for (int i=0; i<n; i++) {
        if (*a > *b) {
            return 1;
        } else if (*a < *b) {
            return -1;
        }
        a++;
        b++;
    } 
    return 0;
}    
