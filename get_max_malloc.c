#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

/* This program finds out how many bytes the largest malloc can
 * return for the heap. On the computer I tested it on, it was
 * about 34 gigabytes.
 * */


int main(int argc, char *argv[]) {
 
    long long int i = 2000000;

    char *charptr = malloc(1);
    
    
    while (charptr != NULL) {
        free(charptr); 
        charptr = malloc(i);
        i += 1;
    }

    printf("\nThe maximum number of bytes assigned with malloc is %lld\n", i);
         
    return EXIT_SUCCESS;
}
