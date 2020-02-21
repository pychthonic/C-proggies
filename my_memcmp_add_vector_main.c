#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "addvector.h"
#include "my_memcmp.h"

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

/* Main function for testing my add_vector functions
 * (inside addvector.c) and my_memcmp (inside
 * my_memcmp.c).
 * */


int main(int argc, char *argv[]) {
    int a[5] = {1, 33, 6, 8, 10};
    int b[5] = {1, 3, 6, 8, 10}; 
    int c[5]; 
    
    addvector2(c, a, b, 5);
    printf("\nAfter calling addvector()...\n\n");
    
    for (int i=0; i<5; i++) {
        printf("%d + %d = c[i]: %d\n", a[i], b[i], c[i]);
    }

    printf("\n\nPart two...\n\n");

    for (int i=0; i<5; i++) {
        printf("a[i]: %2d    b[i]: %2d\n", a[i], b[i]);
    }
    printf("\n");
    
    int ret = my_memcmp((unsigned char *) &a[0], (unsigned char *) &b[0], sizeof(a));
    if (ret > 0) {
        printf("a > b\n\n");
    } else if (ret < 0) {
        printf("a < b\n\n");
    } else {
        printf("a = b\n\n");
    }

    return EXIT_SUCCESS;
}
