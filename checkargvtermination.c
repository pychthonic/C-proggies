#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* This program checks that the list of pointers in argv is 
 * terminated by a NULL pointer.
 * */


int main(int argc, char *argv[]) {
    if (argv[argc] == NULL) {
        printf("argv[argc] = NULL\n");
    } else {
        printf("argv[argc] != NULL\n");
    }

    return EXIT_SUCCESS;
}
