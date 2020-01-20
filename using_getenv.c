#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* This program prints out some environment variables for your
 * entertainment. It also prints out program_invocation_name
 * and program_invocation_short_name.
 * */


int main(int argc, char *argv[]) {
    printf("PATH: %s\n", getenv("PATH"));
    printf("HOME: %s\n", getenv("HOME"));
    printf("ROOT: %s\n", getenv("ROOT"));
    printf("TERM: %s\n", getenv("TERM"));
    printf("program_invocation_name: %s\n", program_invocation_name);
    printf("program_invocation_short_name: %s\n", program_invocation_short_name);

    return EXIT_SUCCESS;
}
