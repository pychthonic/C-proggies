#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* This program prints out the first address after the end of the text
 * segment (address of external variable etext), the first address past
 * the end of the initialized data segment (address of external
 * variable edata), and the first address past the end of the
 * uninitialized data segment (address of external variable end).
 * */

extern char etext, edata, end;
 

int main(int argc, char *argv[]) {

    printf("etext: %p\n", &etext);
    printf("edata: %p\n", &edata);
    printf("end  : %p\n", &end);

    return EXIT_SUCCESS;
}
